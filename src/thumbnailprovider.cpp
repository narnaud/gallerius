#include "thumbnailprovider.h"

#include "gallerymodel.h"

ThumbnailProvider::ThumbnailProvider(GalleryModel *model)
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
    , m_model(model)
{
    Q_ASSERT(m_model);
}

QPixmap ThumbnailProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);
    return m_model->thumbnail(id);
}
