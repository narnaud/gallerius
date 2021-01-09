#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

class QFileSystemModel;

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
    std::unique_ptr<Ui::MainWindow> ui;
    QFileSystemModel *m_fileModel = nullptr;
};

#endif // MAINWINDOW_H
