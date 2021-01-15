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
    case Qt::DecorationRole: {
        const auto &media = m_media.at(index.row());
        if (media.type == Dir)
            return QPixmap(":/assets/folder-open-solid.png");
        else
            return media.thumbnail;
    }
    case FilterRole:
        return m_media.at(index.row()).filter;
    case MediaRole:
        return QVariant::fromValue(m_media.at(index.row()).type);
    case FullPathRole:
        return m_media.at(index.row()).fullPath;
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
    if (!index.isValid())
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
    if (QFile::remove(m_media.at(index.row()).fullPath)) {
        beginRemoveRows({}, index.row(), index.row());
        m_media.remove(index.row());
        endRemoveRows();
        return true;
    }
    return false;
}

QString GalleryModel::path() const
{
    return m_path;
}

void GalleryModel::setPath(const QString &path)
{
    if (m_path == path)
        return;
    writeData();
    m_path = path;
    loadData();
}

QString GalleryModel::title() const
{
    return m_title;
}

void GalleryModel::setTitle(const QString &title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(title);
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

static QString parseTitle(const QString &path)
{
    QFile indexFile(path + "/index.md");
    if (indexFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&indexFile);
        while (!stream.atEnd()) {
            auto line = stream.readLine().simplified();
            if (line.startsWith("title:"))
                return line.remove("title:").simplified();
        }
    }
    QFileInfo fi(path);
    return fi.baseName();
}

static GalleryModel::Media createMedia(const QFileInfo &fileInfo, const QStringList &nomedia)
{
    static std::vector<QString> imageSuffix = {"jpg", "jpeg", "png", "gif"};
    static std::vector<QString> videoSuffix = {"MOV", "mov", "avi", "mp4", "webm", "ogv", "3gp"};

    GalleryModel::Media media;
    if (fileInfo.isDir()) {
        media.type = GalleryModel::Dir;
    } else if (Utility::contains(imageSuffix, fileInfo.suffix())) {
        media.type = GalleryModel::Image;
    } else if (Utility::contains(videoSuffix, fileInfo.suffix())) {
        media.type = GalleryModel::Video;
    } else {
        return media;
    }

    media.fileName = fileInfo.fileName();
    media.fullPath = fileInfo.absoluteFilePath();
    media.filter = nomedia.contains(media.fileName);

    return media;
}

static QVector<GalleryModel::Media> loadMedia(const QDir &dir)
{
    const QStringList nomedia = parseNomedia(dir.path());
    auto list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    QVector<GalleryModel::Media> result;
    result.reserve(list.size());
    for (const auto &info : list) {
        auto media = createMedia(info, nomedia);
        if (media.type != GalleryModel::NoType) {
            QPixmapCache::find(media.fullPath, &media.thumbnail);
            result.push_back(std::move(media));
        }
    }

    return result;
}

static void computeThumbnail(GalleryModel::Media &media, const QString &tempPath)
{
    if (media.type == GalleryModel::Dir || !media.thumbnail.isNull())
        return;

    if (media.type == GalleryModel::Image) {
        QPixmap thumbnail(media.fullPath);
        media.thumbnail = thumbnail.scaled(GalleryModel::ThumbnailSize, GalleryModel::ThumbnailSize,
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else if (media.type == GalleryModel::Video) {
        const QString outName = tempPath + '/' + media.fileName + ".jpg";
        if (!QFile::exists(outName)) {
            int code =
                QProcess::execute(QString("ffmpeg -loglevel quiet -i \"%1\" -vframes 1 \"%2\"")
                                      .arg(media.fullPath)
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

    const QString title = parseTitle(m_path);
    setTitle(title);

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
