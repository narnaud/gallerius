#include "mediaview.h"

#include "gallerymodel.h"

#include <QPainter>
#include <QStackedWidget>
#include <QVBoxLayout>

static const int Margin = 20;

class ImageView : public QWidget
{
public:
    ImageView(QWidget *parent = nullptr)
        : QWidget(parent)
    {
    }

    void setPath(const QString &path)
    {
        if (path.isEmpty())
            m_pixmap = {};
        else
            m_pixmap.load(path);
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event)

        if (m_pixmap.isNull())
            return;

        QPainter p(this);
        // Ensure the pixmap is fully drawn and keep aspect ratio
        auto imageRect = rect();
        const auto imageSize =
            m_pixmap.size().scaled(imageRect.width(), imageRect.height(), Qt::KeepAspectRatio);
        const int hMargin = (imageRect.width() - imageSize.width()) / 2;
        const int vMargin = (imageRect.height() - imageSize.height()) / 2;
        imageRect.adjust(hMargin, vMargin, -hMargin, -vMargin);
        p.drawPixmap(imageRect, m_pixmap);
    }

private:
    QPixmap m_pixmap;
};

MediaView::MediaView(QAbstractItemModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
{
    setFocusPolicy(Qt::NoFocus);
    auto layout = new QVBoxLayout;
    layout->setContentsMargins(Margin, Margin, Margin, Margin);
    setLayout(layout);

    m_stack = new QStackedWidget;
    layout->addWidget(m_stack);

    m_imageView = new ImageView;
    m_stack->addWidget(m_imageView);
}

void MediaView::setCurrentIndex(const QModelIndex &index)
{
    Q_ASSERT(index.isValid());

    m_row = index.row();
    const auto type = index.data(GalleryModel::MediaRole).value<GalleryModel::Type>();
    const auto &fullPath = index.data(GalleryModel::FullPathRole).toString();

    if (type == GalleryModel::Image) {
        m_imageView->setPath(fullPath);
        m_stack->setCurrentWidget(m_imageView);
    } else {
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
}
