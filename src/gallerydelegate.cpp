#include "gallerydelegate.h"

#include "gallerymodel.h"

#include <QPainter>
#include <QStyleOptionViewItem>

GalleryDelegate::GalleryDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void GalleryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    const bool selected = option.state & QStyle::State_Selected;
    const QString &text = index.data().toString();
    const QPixmap &pixmap = index.data(Qt::DecorationRole).value<QPixmap>();

    QRect rect = option.rect;
    painter->fillRect(rect, selected ? QColor(Qt::blue).lighter() : Qt::lightGray);

    if (!pixmap.isNull()) {
        const int hMargin = (rect.width() - pixmap.width()) / 2;
        const int vMargin = (rect.height() - pixmap.height()) / 2;
        rect.adjust(hMargin, vMargin, -hMargin, -vMargin);
        painter->drawPixmap(rect, pixmap);
    }

    rect = {option.rect.left(), option.rect.bottom() - 20, option.rect.width(), 20};
    painter->drawText(rect, Qt::AlignCenter, text);
}

QSize GalleryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(GalleryModel::ThumbnailSize + 2 * Margin,
                 GalleryModel::ThumbnailSize + 2 * Margin);
}
