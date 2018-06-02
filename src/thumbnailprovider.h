#ifndef THUMBNAILPROVIDER_H
#define THUMBNAILPROVIDER_H

#include <QQuickImageProvider>

class GalleryModel;

class ThumbnailProvider : public QQuickImageProvider
{
public:
    ThumbnailProvider(GalleryModel *model);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    GalleryModel *m_model;
};

#endif // THUMBNAILPROVIDER_H
