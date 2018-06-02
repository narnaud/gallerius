#include "gallerymodel.h"

#include "gallery.h"

#include <QPixmap>
#include <QUrl>

GalleryModel::GalleryModel(Gallery *gallery)
    : QAbstractListModel(gallery)
    , m_gallery(gallery)
{
    Q_ASSERT(m_gallery);

    connect(m_gallery, &Gallery::dataAboutToChange, this, &GalleryModel::beginResetModel);
    connect(m_gallery, &Gallery::dataChanged, this, &GalleryModel::endResetModel);
    connect(m_gallery, &Gallery::thumbnailsDone, this, [this]() {
        emit dataChanged(index(0), index(m_gallery->mediaCount() - 1), {ThumbnailRole});
    });
    connect(m_gallery, &Gallery::mediaChanged, this,
            [this](int row) { emit dataChanged(index(row), index(row), {FilterRole}); });
}

GalleryModel::~GalleryModel() {}

int GalleryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_gallery->mediaCount();
}

QVariant GalleryModel::data(const QModelIndex &index, int role) const
{
    const Media &media = m_gallery->media(index.row());
    switch (role) {
    case FileNameRole:
        return media.fileName;
    case FilePathRole:
        return QUrl::fromLocalFile(media.filePath);
    case TypeRole:
        return media.type;
    case ThumbnailRole:
        // We need to make sure the id is unique
        if (!media.thumbnail.isNull())
            return "image://thumbnail/" + QString::number(index.row()) + "+" + media.fileName;
        return "";
    case FilterRole:
        return media.filter;
    case MediaRole:
        return QVariant::fromValue(media);
    }

    return {};
}

QHash<int, QByteArray> GalleryModel::roleNames() const
{
    auto result = QAbstractListModel::roleNames();
    result[FileNameRole] = "fileName";
    result[FilePathRole] = "filePath";
    result[TypeRole] = "type";
    result[ThumbnailRole] = "thumbnail";
    result[FilterRole] = "filter";
    return result;
}

QPixmap GalleryModel::thumbnail(const QString &id)
{
    int row = id.split('+').first().toInt();
    auto thumb = m_gallery->media(row).thumbnail;
    return QPixmap::fromImage(thumb);
}