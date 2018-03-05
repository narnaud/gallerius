#include "gallery.h"

#include "constants.h"
#include "utility.h"

#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QTemporaryDir>
#include <QtConcurrent>

#include <algorithm>

static constexpr int ThumbnailSize = 200;

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

bool Gallery::isRoot() const
{
    return m_rootPath == m_path;
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

static Media createMedia(const QFileInfo &fileInfo, const QStringList &nomedia,
                         const QString &tempPath)
{
    static std::vector<QString> imageSuffix = {"jpg", "jpeg", "png", "gif"};
    static std::vector<QString> videoSuffix = {"mov", "avi", "mp4", "webm", "ogv", "3gp"};

    Media media;
    if (fileInfo.isDir()) {
        media.type = Media::Dir;
    } else if (Utility::contains(imageSuffix, fileInfo.suffix())) {
        media.type = Media::Image;
        QImage thumbnail(fileInfo.absoluteFilePath());
        media.thumbnail = thumbnail.scaled(ThumbnailSize, ThumbnailSize, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);
    } else if (Utility::contains(videoSuffix, fileInfo.suffix())) {
        media.type = Media::Video;
        const QString outName = tempPath + '/' + fileInfo.fileName() + ".jpg";
        int code = QProcess::execute(QString("ffmpeg -loglevel quiet -i \"%1\" -vframes 1 \"%2\"")
                                         .arg(fileInfo.absoluteFilePath())
                                         .arg(QDir::toNativeSeparators(outName)));
        if (code == 0) {
            QImage thumbnail(outName);
            media.thumbnail = thumbnail.scaled(ThumbnailSize, ThumbnailSize, Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
        }
    } else {
        return media;
    }

    media.fileName = fileInfo.fileName();
    media.filePath = fileInfo.absoluteFilePath();
    media.excluded = nomedia.contains(media.fileName);

    return media;
}

static QVector<Media> loadMedia(const QDir &dir, const QStringList &nomedia,
                                const QString &tempPath)
{
    auto list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name);

    auto result = QtConcurrent::blockingMapped(
        list.toVector(),
        std::function<Media(const QFileInfo &)>([nomedia, tempPath](const QFileInfo &info) {
            return createMedia(info, nomedia, tempPath);
        }));
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

    QStringList nomedia = parseNomedia(dir.canonicalPath());

    Data data;
    {
        QTemporaryDir tempDir;
        data.media = loadMedia(dir, nomedia, tempDir.isValid() ? tempDir.path() : "");
    }

    m_data = std::move(data);
    emit dataChanged();
}
