#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

class QFileSystemModel;
class GalleryModel;

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

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QFileSystemModel *m_fileModel = nullptr;
    GalleryModel *m_galleryModel = nullptr;
};

#endif // MAINWINDOW_H
