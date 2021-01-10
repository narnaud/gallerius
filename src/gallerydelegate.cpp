#include "gallerydelegate.h"

#include "gallerymodel.h"

#include <QPainter>
#include <QStyleOptionViewItem>

static constexpr int TextMargin = 18;

GalleryDelegate::GalleryDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void GalleryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    const bool selected = option.state & QStyle::State_Selected;
    const bool filter = index.data(GalleryModel::FilterRole).toBool();
    const QString &text = index.data().toString();
    const QPixmap &pixmap = index.data(Qt::DecorationRole).value<QPixmap>();

    QColor color = filter ? (selected ? QColor(Qt::magenta).lighter() : QColor(Qt::red).lighter())
                          : (selected ? QColor(Qt::blue).lighter() : Qt::lightGray);
    painter->fillRect(option.rect, color);

    if (!pixmap.isNull()) {
        const int hMargin = (option.rect.width() - pixmap.width()) / 2;
        const int vMargin = (option.rect.height() - pixmap.height() - TextMargin + Margin) / 2;
        const QRect rect = option.rect.adjusted(hMargin, vMargin, -hMargin, -vMargin - TextMargin);
        ;
        painter->drawPixmap(rect, pixmap);
    }

    const QRect textRect = {option.rect.left(), option.rect.bottom() - TextMargin,
                            option.rect.width(), TextMargin};
    color.setAlpha(128);
    painter->fillRect(textRect, color);
    painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, text);
}

QSize GalleryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(GalleryModel::ThumbnailSize + 2 * Margin,
                 GalleryModel::ThumbnailSize + Margin + TextMargin);
}
