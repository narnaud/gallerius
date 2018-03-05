#include "gallery.h"

#include "constants.h"
#include "utility.h"

#include <QDebug>
#include <QDir>
#include <QFutureWatcher>
#include <QProcess>
#include <QSettings>
#include <QtConcurrent>

#include <algorithm>

static constexpr int ThumbnailSize = 200;

Gallery::Gallery(QObject *parent)
    : QObject(parent)
    , m_watcher(new QFutureWatcher<void>(this))
{
    connect(m_watcher, &QFutureWatcher<void>::progressValueChanged, this,
            &Gallery::progressValueChanged);
    connect(m_watcher, &QFutureWatcher<void>::finished, this, &Gallery::thumbnailsDone);
}

QUrl Gallery::rootPath() const
{
    return m_rootPath;
}

QUrl Gallery::path() const
{
    return m_path;
}

bool Gallery::isRoot() const
{
    return m_rootPath == m_path;
}

int Gallery::mediaCount() const
{
    return static_cast<int>(m_data.media.size());
}

const Media &Gallery::media(int index) const
{
    return m_data.media[index];
}

int Gallery::progressValue() const
{
    if (m_watcher->isRunning())
        return m_watcher->progressValue();
    else
        return static_cast<int>(m_data.media.size());
}

void Gallery::setRootPath(QUrl rootPath)
{
    if (m_rootPath == rootPath)
        return;

    m_rootPath = rootPath;
    qInfo() << "Root path: " << m_rootPath;

    // Save the new root path
    QSettings settings;
    settings.setValue(Constants::RootPathKey, m_rootPath);

    emit rootPathChanged(m_rootPath);

    setPath(rootPath);
}

void Gallery::setPath(QUrl path)
{
    if (m_path == path)
        return;

    m_path = path;
    loadData();
    emit pathChanged(m_path);
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

static Media createMedia(const QFileInfo &fileInfo, const QStringList &nomedia)
{
    static std::vector<QString> imageSuffix = {"jpg", "jpeg", "png", "gif"};
    static std::vector<QString> videoSuffix = {"mov", "avi", "mp4", "webm", "ogv", "3gp"};

    Media media;
    if (fileInfo.isDir()) {
        media.type = Media::Dir;
    } else if (Utility::contains(imageSuffix, fileInfo.suffix())) {
        media.type = Media::Image;
    } else if (Utility::contains(videoSuffix, fileInfo.suffix())) {
        media.type = Media::Video;
    } else {
        return media;
    }

    media.fileName = fileInfo.fileName();
    media.filePath = fileInfo.absoluteFilePath();
    media.excluded = nomedia.contains(media.fileName);

    return media;
}

static std::vector<Media> loadMedia(const QDir &dir, const QStringList &nomedia)
{
    auto list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name);

    std::vector<Media> result;
    result.reserve(list.size());
    for (const auto &info : list) {
        auto media = createMedia(info, nomedia);
        if (media.type != Media::NoType)
            result.emplace_back(std::move(media));
    }

    return result;
}

static void computeThumbnail(Media &media, const QString &tempPath)
{
    if (media.type == Media::Image) {
        QImage thumbnail(media.filePath);
        media.thumbnail = thumbnail.scaled(ThumbnailSize, ThumbnailSize, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);
    } else if (media.type == Media::Video) {
        const QString outName = tempPath + '/' + media.fileName + ".jpg";
        int code = QProcess::execute(QString("ffmpeg -loglevel quiet -i \"%1\" -vframes 1 \"%2\"")
                                         .arg(media.filePath)
                                         .arg(QDir::toNativeSeparators(outName)));
        if (code == 0) {
            QImage thumbnail(outName);
            media.thumbnail = thumbnail.scaled(ThumbnailSize, ThumbnailSize, Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
        }
    }
}

void Gallery::loadData()
{
    QDir dir(m_path.toLocalFile());
    if (!dir.exists())
        return;

    m_watcher->cancel();

    Data data;
    data.media = loadMedia(dir, parseNomedia(dir.canonicalPath()));

    const QString &tempPath = m_tempDirectory.isValid() ? m_tempDirectory.path() : "";
    m_watcher->setFuture(
        QtConcurrent::map(data.media, std::function<void(Media &)>([tempPath](Media &media) {
                              computeThumbnail(media, tempPath);
                          })));

    emit dataAboutToChange();
    m_data = std::move(data);
    emit dataChanged();
}
