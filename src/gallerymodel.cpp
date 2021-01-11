#include "gallerymodel.h"

#include "utility.h"

#include <QDir>
#include <QFile>
#include <QFutureWatcher>
#include <QPixmapCache>
#include <QProcess>
#include <QtConcurrent>

GalleryModel::GalleryModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_watcher(new QFutureWatcher<void>(this))
{
    connect(m_watcher, &QFutureWatcher<void>::finished, this, [this]() {
        // For whatever reasons, the dataChanged does not update the thumbnails
        // emit dataChanged(index(0, 0), index(m_media.count(), 0), {Qt::DecorationRole});
        beginResetModel();
        endResetModel();
    });
    connect(m_watcher, &QFutureWatcher<void>::progressValueChanged, this,
            [this](int value) { emit progressChanged(value, m_media.count()); });
}

GalleryModel::~GalleryModel()
{
    m_watcher->cancel();
    m_watcher->waitForFinished();
    writeData();
}

int GalleryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_media.count();
}

QVariant GalleryModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return m_media.at(index.row()).fileName;
    case Qt::DecorationRole:
        return m_media.at(index.row()).thumbnail;
    case FilterRole:
        return m_media.at(index.row()).filter;
    case MediaRole:
        return m_media.at(index.row()).type;
    }

    return {};
}

bool GalleryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
    case FilterRole:
        m_media[index.row()].filter = value.toBool();
        emit dataChanged(index, index, {FilterRole});
        m_hasChanged = true;
        return true;
    }
    return false;
}

void GalleryModel::toggleMedia(const QModelIndex &index)
{
    if (index.isValid())
        return;

    const bool filtered = m_media.at(index.row()).filter;
    setData(index, !filtered, GalleryModel::FilterRole);
}

bool GalleryModel::deleteMedia(const QModelIndex &index)
{
    if (!index.isValid())
        return false;

    // Note that the remove should happen inside the begin/end
    // but that's a good enough shortcut for the current application
    if (QFile::remove(m_media.at(index.row()).filePath)) {
        beginRemoveRows({}, index.row(), index.row());
        m_media.remove(index.row());
        endRemoveRows();
        return true;
    }
    return false;
}

void GalleryModel::setPath(const QString &path)
{
    writeData();
    m_path = path;
    loadData();
}

static QStringList parseNomedia(const QString &path)
{
    QFile nomediaFile(path + "/.nomedia");
    QStringList result;
    if (nomediaFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&nomediaFile);
        while (!stream.atEnd()) {
            auto line = stream.readLine().simplified();
            if (!line.isEmpty())
                result.append(line);
        }
    }
    return result;
}

static GalleryModel::Media createMedia(const QFileInfo &fileInfo, const QStringList &nomedia)
{
    static std::vector<QString> imageSuffix = {"jpg", "jpeg", "png", "gif"};
    static std::vector<QString> videoSuffix = {"MOV", "mov", "avi", "mp4", "webm", "ogv", "3gp"};

    GalleryModel::Media media;
    if (Utility::contains(imageSuffix, fileInfo.suffix())) {
        media.type = GalleryModel::Media::Image;
    } else if (Utility::contains(videoSuffix, fileInfo.suffix())) {
        media.type = GalleryModel::Media::Video;
    } else {
        return media;
    }

    media.fileName = fileInfo.fileName();
    media.filePath = fileInfo.absoluteFilePath();
    media.filter = nomedia.contains(media.fileName);

    return media;
}

static QVector<GalleryModel::Media> loadMedia(const QDir &dir)
{
    const QStringList nomedia = parseNomedia(dir.path());
    auto list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

    QVector<GalleryModel::Media> result;
    result.reserve(list.size());
    for (const auto &info : list) {
        auto media = createMedia(info, nomedia);
        if (media.type != GalleryModel::Media::NoType) {
            QPixmapCache::find(media.filePath, &media.thumbnail);
            result.push_back(std::move(media));
        }
    }

    return result;
}

static void computeThumbnail(GalleryModel::Media &media, const QString &tempPath)
{
    if (!media.thumbnail.isNull())
        return;

    if (media.type == GalleryModel::Media::Image) {
        QPixmap thumbnail(media.filePath);
        media.thumbnail = thumbnail.scaled(GalleryModel::ThumbnailSize, GalleryModel::ThumbnailSize,
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else if (media.type == GalleryModel::Media::Video) {
        const QString outName = tempPath + '/' + media.fileName + ".jpg";
        if (!QFile::exists(outName)) {
            int code =
                QProcess::execute(QString("ffmpeg -loglevel quiet -i \"%1\" -vframes 1 \"%2\"")
                                      .arg(media.filePath)
                                      .arg(QDir::toNativeSeparators(outName)));
            if (code != 0)
                return;
        }
        QPixmap thumbnail(outName);
        media.thumbnail = thumbnail.scaled(GalleryModel::ThumbnailSize, GalleryModel::ThumbnailSize,
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    QPixmapCache::insert(media.fileName, media.thumbnail);
}

void GalleryModel::loadData()
{
    m_watcher->cancel();
    m_watcher->waitForFinished();

    auto data = loadMedia(m_path);

    const QString &tempPath = m_tempDirectory.isValid() ? m_tempDirectory.path() : "";
    m_watcher->setFuture(
        QtConcurrent::map(data, std::function<void(Media &)>([tempPath](Media &media) {
                              computeThumbnail(media, tempPath);
                          })));

    emit beginResetModel();
    m_hasChanged = false;
    m_media = std::move(data);
    emit endResetModel();
}

void GalleryModel::writeData()
{
    if (!m_hasChanged)
        return;

    QStringList list;
    for (const auto &media : m_media) {
        if (media.filter)
            list.append(media.fileName);
    }

    if (list.isEmpty()) {
        // Remove existing nomedia file if any
        QFile::remove(m_path + "/.nomedia");
    } else {
        QFile nomediaFile(m_path + "/.nomedia");
        if (nomediaFile.open(QIODevice::WriteOnly)) {
            QTextStream stream(&nomediaFile);
            stream << list.join('\n');
        }
    }
}
