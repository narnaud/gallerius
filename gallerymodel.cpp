#include "gallerymodel.h"

GalleryModel::GalleryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

GalleryModel::~GalleryModel() {}

int GalleryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_media.size();
}

QVariant GalleryModel::data(const QModelIndex &index, int role) const
{
    const Media &media = m_media.value(index.row());
    switch (role) {
    case FileNameRole:
        return media.fileName;
    case FilePathRole:
        return media.filePath;
    case TypeRole:
        return media.type;
    case ImageRole:
        // We need to make sure the id is unique
        if (media.type == Media::Image)
            return "image://thumbnail/" + QString::number(index.row()) + "+" + media.fileName;
        break;
    case ExcludedRole:
        return media.excluded;
    }

    return {};
}

QHash<int, QByteArray> GalleryModel::roleNames() const
{
    auto result = QAbstractListModel::roleNames();
    result[FileNameRole] = "fileName";
    result[FilePathRole] = "filePath";
    result[TypeRole] = "type";
    result[ImageRole] = "image";
    result[ExcludedRole] = "excluded";
    return result;
}

QPixmap GalleryModel::thumbnail(const QString &id)
{
    int row = id.split('+').first().toInt();
    return m_media[row].thumbnail;
}

void GalleryModel::setMedia(const QVector<Media> &media)
{
    beginResetModel();
    m_media = media;
    endResetModel();
}
