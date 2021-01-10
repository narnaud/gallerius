#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

class GalleryModel;
class GalleryFilterProxyModel;
class QFileSystemModel;
class QProgressBar;

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

private:
    void selectDirectory(const QModelIndex &index);
    void updateProgressBar(int value, int total);
    void toggleMedia();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QFileSystemModel *m_fileModel = nullptr;
    GalleryModel *m_galleryModel = nullptr;
    GalleryFilterProxyModel *m_filterModel = nullptr;
    QProgressBar *m_progressBar = nullptr;
};

#endif // MAINWINDOW_H
