#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>

class Gallery;

class GalleryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role {
        FileNameRole = Qt::UserRole,
        FilePathRole,
        TypeRole,
        ThumbnailRole,
        ExcludedRole,
    };

public:
    explicit GalleryModel(Gallery *gallery = nullptr);
    ~GalleryModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    QPixmap thumbnail(const QString &id);

private:
    Gallery *m_gallery;
};

#endif // GALLERYMODEL_H
