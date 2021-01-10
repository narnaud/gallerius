#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gallerydelegate.h"
#include "galleryfilterproxymodel.h"
#include "gallerymodel.h"

#include <QFileSystemModel>
#include <QProgressBar>

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
    connect(ui->directoryView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MainWindow::selectDirectory);

    auto filterModel = new GalleryFilterProxyModel(this);
    filterModel->setSourceModel(m_galleryModel);
    ui->galleryView->setModel(filterModel);
    ui->galleryView->setItemDelegate(new GalleryDelegate(this));
    ui->galleryView->setViewMode(QListView::IconMode);
    ui->galleryView->setResizeMode(QListView::Adjust);
    ui->galleryView->setUniformItemSizes(true);
    ui->galleryView->setSpacing(GalleryDelegate::Margin);

    m_progressBar = new QProgressBar(this);
    statusBar()->addPermanentWidget(m_progressBar, 1);
    connect(m_galleryModel, &GalleryModel::progressChanged, this, &MainWindow::updateProgressBar);
    m_progressBar->hide();

    ui->allButton->setShortcut(QKeySequence("Ctrl+A"));
    connect(ui->allButton, &QToolButton::toggled, this, [filterModel](bool checked) {
        if (checked)
            filterModel->setFilter(GalleryFilterProxyModel::All);
    });
    ui->onlyVisibleButton->setShortcut(QKeySequence("Ctrl+V"));
    connect(ui->onlyVisibleButton, &QToolButton::toggled, this, [filterModel](bool checked) {
        if (checked)
            filterModel->setFilter(GalleryFilterProxyModel::OnlyVisible);
    });
    ui->onlyHiddenButton->setShortcut(QKeySequence("Ctrl+H"));
    connect(ui->onlyHiddenButton, &QToolButton::toggled, this, [filterModel](bool checked) {
        if (checked)
            filterModel->setFilter(GalleryFilterProxyModel::OnlyHidden);
    });
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

void MainWindow::updateProgressBar(int value, int total)
{
    const bool hide = total != 0 && value != total;
    m_progressBar->setVisible(hide);
    m_progressBar->setMaximum(total);
    m_progressBar->setValue(value);
}
