#ifndef MEDIAVIEW_H
#define MEDIAVIEW_H

#include <QWidget>

class QAbstractItemModel;
class QStackedWidget;
class ImageView;
class VideoView;

class MediaView : public QWidget
{
    Q_OBJECT
public:
    explicit MediaView(QAbstractItemModel *model, QWidget *parent = nullptr);

public slots:
    void setCurrentIndex(const QModelIndex &index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    const QAbstractItemModel *m_model;
    int m_row = -1;
    ImageView *m_imageView = nullptr;
    VideoView *m_videoView = nullptr;
    QStackedWidget *m_stack = nullptr;
};

#endif // MEDIAVIEW_H
