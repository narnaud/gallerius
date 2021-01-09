#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gallerymodel.h"

#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_fileModel(new QFileSystemModel(this))
    , m_galleryModel(new GalleryModel(this))
{
    ui->setupUi(this);
    menuBar()->hide();

    m_fileModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->directoryView->setModel(m_fileModel);
    ui->directoryView->hideColumn(1);
    ui->directoryView->hideColumn(2);
    ui->directoryView->hideColumn(3);

    ui->galleryView->setModel(m_galleryModel);
    ui->galleryView->setViewMode(QListView::IconMode);

    connect(ui->directoryView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MainWindow::selectDirectory);
}

MainWindow::~MainWindow() { }

void MainWindow::setRootPath(const QString &rootPath)
{
    m_fileModel->setRootPath(rootPath);
    ui->directoryView->setRootIndex(m_fileModel->index(rootPath));
}

void MainWindow::selectDirectory(const QModelIndex &index)
{
    const QString &path = m_fileModel->filePath(index);
    m_galleryModel->setPath(path);
}
