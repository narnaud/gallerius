#ifndef GALLERYDELEGATE_H
#define GALLERYDELEGATE_H

#include <QItemDelegate>

class GalleryDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    static constexpr int Margin = 5;

    GalleryDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // GALLERYDELEGATE_H
