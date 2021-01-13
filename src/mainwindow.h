#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

class GalleryModel;
class GalleryFilterProxyModel;
class QFileSystemModel;
class QProgressBar;
class MediaView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setRootPath(const QString &rootPath);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void updateProgressBar(int value, int total);
    QModelIndex selectedSourceIndex();
    void toggleMedia();
    void deleteMedia();
    void cdUp();
    void goDown();
    void goUp();
    void navigate(int delta = 1);
    void doAction(const QModelIndex &index);
    void openPath(const QString &path);

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QFileSystemModel *m_fileModel = nullptr;
    GalleryModel *m_galleryModel = nullptr;
    GalleryFilterProxyModel *m_filterModel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QString m_rootPath;
    MediaView *m_mediaView;
};

#endif // MAINWINDOW_H
