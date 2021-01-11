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
        MediaRole,
    };

    struct Media
    {
        enum Type { Image, Video, Dir, NoType };

        QString fileName;
        QString filePath;
        QPixmap thumbnail;
        Type type = NoType;
        bool filter = false;
    };

    explicit GalleryModel(QObject *parent = nullptr);
    ~GalleryModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void toggleMedia(const QModelIndex &index);
    bool deleteMedia(const QModelIndex &index);

    void setPath(const QString &path);

signals:
    void progressChanged(int progressValue, int total);

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
