#include "gallerymodel.h"

#include <QDebug>
#include <QDir>

GalleryModel::GalleryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

GalleryModel::~GalleryModel() {}

int GalleryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_files.count();
}

QVariant GalleryModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_files.value(index.row());
    }

    return {};
}

void GalleryModel::setPath(const QString &path)
{
    beginResetModel();
    QDir dir(path);
    m_files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    endResetModel();
}
