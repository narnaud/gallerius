#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include "data.h"

#include <QAbstractListModel>

class Gallery;

class GalleryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GalleryModel(QObject *parent = nullptr);
    ~GalleryModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void setPath(const QString &path);

private:
    QStringList m_files;
};

#endif // GALLERYMODEL_H
