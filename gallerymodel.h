#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include "data.h"

#include <QAbstractListModel>

class GalleryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role {
        FileNameRole = Qt::UserRole,
        FilePathRole,
        TypeRole,
        ImageRole,
    };

public:
    explicit GalleryModel(QObject *parent = nullptr);
    ~GalleryModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setMedia(const QVector<Media> &media);

    QPixmap thumbnail(const QString &id);

private:
    QVector<Media> m_media;
};

#endif // GALLERYMODEL_H
