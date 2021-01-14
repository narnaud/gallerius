#include "mediaview.h"

#include "gallerymodel.h"

#include <QGridLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QPainter>
#include <QProcess>
#include <QSlider>
#include <QStackedWidget>
#include <QStyle>
#include <QTime>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVideoWidget>

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

class VideoView : public QWidget
{
public:
    VideoView(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setFocusPolicy(Qt::NoFocus);
        auto grid = new QGridLayout;
        setLayout(grid);

        m_player = new QMediaPlayer(this);

        auto createToolButton = [this](const QString &shortcut, QStyle::StandardPixmap icon) {
            auto button = new QToolButton;
            button->setFocusPolicy(Qt::NoFocus);
            button->setShortcut(QKeySequence(shortcut));
            button->setIcon(style()->standardIcon(icon));
            return button;
        };

        auto video = new QVideoWidget;
        video->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_player->setVideoOutput(video);
        grid->addWidget(video, 0, 0, 1, 3);

        m_playButton = createToolButton("P", QStyle::SP_MediaPlay);
        grid->addWidget(m_playButton, 1, 0);
        m_slider = new QSlider(Qt::Horizontal);
        m_slider->setFocusPolicy(Qt::NoFocus);
        m_slider->setRange(0, m_player->duration() / 1000);
        grid->addWidget(m_slider, 1, 1);
        m_label = new QLabel;
        grid->addWidget(m_label, 1, 2);

        auto cutLayout = new QHBoxLayout;
        grid->addLayout(cutLayout, 2, 1);
        auto fromButton = createToolButton("Alt+Left", QStyle::SP_MediaSeekForward);
        cutLayout->addWidget(fromButton);
        m_from = new QLabel;
        cutLayout->addWidget(m_from);
        cutLayout->addStretch(1);
        auto saveButton = createToolButton("Ctrl+S", QStyle::SP_DialogSaveButton);
        cutLayout->addWidget(saveButton);
        cutLayout->addStretch(1);
        m_to = new QLabel;
        cutLayout->addWidget(m_to);
        auto toButton = createToolButton("Alt+Right", QStyle::SP_MediaSeekBackward);
        cutLayout->addWidget(toButton);

        connect(m_player, &QMediaPlayer::durationChanged, this, &VideoView::durationChanged);
        connect(m_player, &QMediaPlayer::positionChanged, this, &VideoView::positionChanged);
        connect(m_player, &QMediaPlayer::stateChanged, this, &VideoView::setState);
        connect(m_slider, &QSlider::sliderMoved, this,
                [this](int seconds) { m_player->setPosition(seconds * 1000); });
        connect(m_playButton, &QAbstractButton::clicked, this, &VideoView::playClicked);
        connect(fromButton, &QToolButton::clicked, this, [this]() {
            m_from->setText(QString::number(double(m_player->position()) / 1000));
        });
        connect(saveButton, &QToolButton::clicked, this, &VideoView::save);
        connect(toButton, &QToolButton::clicked, this,
                [this]() { m_to->setText(QString::number(double(m_player->position()) / 1000)); });
    }

    void setPath(const QString &path)
    {
        m_path = path;
        m_from->clear();
        m_to->clear();
        m_player->setMedia(QMediaContent(path));
    }

    void play() { m_player->play(); }

private:
    void durationChanged(qint64 duration)
    {
        m_duration = duration / 1000;
        m_slider->setMaximum(m_duration);
    }
    void positionChanged(qint64 progress)
    {
        if (!m_slider->isSliderDown())
            m_slider->setValue(progress / 1000);

        updateDurationInfo(progress / 1000);
    }
    void updateDurationInfo(qint64 currentInfo)
    {
        QString tStr;
        if (currentInfo || m_duration) {
            QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60, currentInfo % 60,
                              (currentInfo * 1000) % 1000);
            QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60, m_duration % 60,
                            (m_duration * 1000) % 1000);
            QString format = "mm:ss";
            if (m_duration > 3600)
                format = "hh:mm:ss";
            tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
        }
        m_label->setText(tStr);
    }
    void setState(QMediaPlayer::State state)
    {
        switch (state) {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        }
    }
    void playClicked()
    {
        switch (m_player->state()) {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            m_player->play();
            break;
        case QMediaPlayer::PlayingState:
            m_player->pause();
            break;
        }
    }
    void save()
    {
        m_player->stop();
        m_player->setMedia({});
        if (m_from->text().isEmpty() && m_to->text().isEmpty())
            return;
        QFileInfo fi(m_path);
        const QString tmpFile =
            fi.absolutePath() + "/" + fi.baseName() + "-tmp." + fi.completeSuffix();
        if (m_from->text().isEmpty()) {
            QProcess::execute(QString("ffmpeg -i \"%1\" -to %2 -c copy -y \"%3\"")
                                  .arg(QDir::toNativeSeparators(m_path))
                                  .arg(m_to->text())
                                  .arg(QDir::toNativeSeparators(tmpFile)));
        } else if (m_to->text().isEmpty()) {
            QProcess::execute(QString("ffmpeg -i \"%1\" -ss %2 -c copy -y \"%3\"")
                                  .arg(QDir::toNativeSeparators(m_path))
                                  .arg(m_from->text())
                                  .arg(QDir::toNativeSeparators(tmpFile)));
        } else {
            QProcess::execute(QString("ffmpeg -i \"%1\" -ss %2 -to %3 -c copy -y \"%4\"")
                                  .arg(QDir::toNativeSeparators(m_path))
                                  .arg(m_from->text())
                                  .arg(m_to->text())
                                  .arg(QDir::toNativeSeparators(tmpFile)));
        }
        if (QFile::exists(tmpFile)) {
            QFile::remove(m_path);
            QFile::rename(tmpFile, m_path);
        }
        setPath(m_path);
    }

private:
    QMediaPlayer *m_player;
    QSlider *m_slider;
    QLabel *m_label;
    qint64 m_duration = 0;
    QToolButton *m_playButton;
    QLabel *m_from;
    QLabel *m_to;
    QString m_path;
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
    m_videoView = new VideoView;
    m_stack->addWidget(m_videoView);
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
    } else if (type == GalleryModel::Video) {
        m_videoView->setPath(fullPath);
        m_stack->setCurrentWidget(m_videoView);
        if (isVisible())
            m_videoView->play();
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

void MediaView::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (isVisible() && m_stack->currentWidget() == m_videoView)
        m_videoView->play();
}
