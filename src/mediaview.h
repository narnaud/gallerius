#ifndef MEDIAVIEW_H
#define MEDIAVIEW_H

#include <QWidget>

class QAbstractItemModel;

class MediaView : public QWidget
{
    Q_OBJECT
public:
    explicit MediaView(QAbstractItemModel *model, QWidget *parent = nullptr);

public slots:
    void setCurrentIndex(const QModelIndex &index);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const QAbstractItemModel *m_model;
    int m_row = -1;
    QPixmap m_pixmap;
};

#endif // MEDIAVIEW_H
