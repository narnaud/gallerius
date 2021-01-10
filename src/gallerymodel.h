#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>
#include <QPixmap>
#include <QTemporaryDir>
#include <QVector>

template <typename T>
class QFutureWatcher;

class GalleryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    static constexpr int ThumbnailSize = 160;

    enum Roles {
        FilterRole = Qt::UserRole + 1,
    };

    struct Media
    {
        enum Type { Image, Video, NoType };

        QString fileName;
        QString filePath;
        QPixmap thumbnail;
        Type type = NoType;
        bool filter = false;
    };

    explicit GalleryModel(QObject *parent = nullptr);
    ~GalleryModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void setPath(const QString &path);

private:
    void loadData();
    void writeData();

private:
    bool m_hasChanged = false;
    QString m_path;
    QVector<Media> m_media;

    QFutureWatcher<void> *m_watcher;
    QTemporaryDir m_tempDirectory;
};

#endif // GALLERYMODEL_H
