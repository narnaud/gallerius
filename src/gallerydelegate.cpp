#include "gallerydelegate.h"

#include "gallerymodel.h"

#include <QPainter>
#include <QStyleOptionViewItem>

static constexpr int TextMargin = 18;
static constexpr int MovieRadius = 10;

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
    const auto role =
        static_cast<GalleryModel::Media::Type>(index.data(GalleryModel::MediaRole).toInt());

    QColor color = filter ? (selected ? QColor(Qt::magenta).lighter() : QColor(Qt::red).lighter())
                          : (selected ? QColor(Qt::blue).lighter() : Qt::lightGray);
    painter->fillRect(option.rect, color);

    if (!pixmap.isNull()) {
        const int hMargin = (option.rect.width() - pixmap.width()) / 2;
        const int vMargin = (option.rect.height() - pixmap.height() - TextMargin + Margin) / 2;
        const QRect rect = option.rect.adjusted(hMargin, vMargin, -hMargin, -vMargin - TextMargin);
        painter->drawPixmap(rect, pixmap);

        // Draw movie ribbon on the left and right
        if (role == GalleryModel::Media::Video) {
            painter->fillRect(QRect {rect.left(), rect.top(), MovieRadius + 4, rect.height()},
                              Qt::darkGray);
            painter->fillRect(
                QRect {rect.right() - MovieRadius - 3, rect.top(), MovieRadius + 4, rect.height()},
                Qt::darkGray);
            for (int pos = 0; pos < rect.height(); pos += Margin + MovieRadius) {
                const QRect holeLeft = {rect.left() + 2, rect.top() + 2 + pos, MovieRadius,
                                        std::min(MovieRadius, rect.height() - pos - 2)};
                painter->fillRect(holeLeft, Qt::lightGray);
                const QRect holeRight = {rect.right() - 1 - MovieRadius, rect.top() + 2 + pos,
                                         MovieRadius,
                                         std::min(MovieRadius, rect.height() - pos - 2)};
                painter->fillRect(holeRight, Qt::lightGray);
            }
        }
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
