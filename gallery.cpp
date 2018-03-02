#include "gallery.h"

#include "constants.h"
#include "utility.h"

#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QtConcurrent>

#include <algorithm>

Gallery::Gallery(QObject *parent)
    : QObject(parent)
{
}

QUrl Gallery::rootPath() const
{
    return m_rootPath;
}

QUrl Gallery::path() const
{
    return m_path;
}

QVector<Media> Gallery::media() const
{
    return m_data.media;
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

static Media createMedia(const QFileInfo &fileInfo)
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
    return media;
}

static QVector<Media> loadMedia(const QDir &dir)
{
    auto list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name);

    auto result = QtConcurrent::blockingMapped(list.toVector(), createMedia);
    result.erase(std::remove_if(result.begin(), result.end(),
                                [](const Media &media) { return media.type == Media::NoType; }),
                 result.end());
    return result;
}

void Gallery::loadData()
{
    QDir dir(m_path.toLocalFile());
    if (!dir.exists())
        return;

    Data data;
    data.media = loadMedia(dir);

    m_data = std::move(data);
}
