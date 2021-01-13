#include "mediaview.h"

#include "gallerymodel.h"

#include <QPainter>
#include <QVBoxLayout>

static const int Margin = 20;

MediaView::MediaView(QAbstractItemModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
{
    setFocusPolicy(Qt::NoFocus);
    auto layout = new QVBoxLayout;
    layout->setContentsMargins(Margin, Margin, Margin, Margin);
    setLayout(layout);
}

void MediaView::setCurrentIndex(const QModelIndex &index)
{
    Q_ASSERT(index.isValid());

    m_row = index.row();
    const auto type = index.data(GalleryModel::MediaRole).value<GalleryModel::Type>();
    const auto &fullPath = index.data(GalleryModel::FullPathRole).toString();

    if (type == GalleryModel::Image) {
        m_pixmap.load(fullPath);
    } else {
        m_pixmap = {};
    }
    update();
}

void MediaView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter p(this);

    const bool filtered =
        m_model->data(m_model->index(m_row, 0), GalleryModel::FilterRole).toBool();
    p.fillRect(rect(), filtered ? QColor(Qt::red).lighter() : Qt::white);

    if (m_pixmap.isNull())
        return;

    // Ensure the pixmap is fully drawn and keep aspect ratio
    auto imageRect = rect().marginsRemoved(layout()->contentsMargins());
    const auto imageSize =
        m_pixmap.size().scaled(imageRect.width(), imageRect.height(), Qt::KeepAspectRatio);
    const int hMargin = (imageRect.width() - imageSize.width()) / 2;
    const int vMargin = (imageRect.height() - imageSize.height()) / 2;
    imageRect.adjust(hMargin, vMargin, -hMargin, -vMargin);
    p.drawPixmap(imageRect, m_pixmap);
}
