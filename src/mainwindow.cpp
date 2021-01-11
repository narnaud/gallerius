#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gallerydelegate.h"
#include "galleryfilterproxymodel.h"
#include "gallerymodel.h"

#include <QFileSystemModel>
#include <QProgressBar>
#include <QShortcut>

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

    m_filterModel = new GalleryFilterProxyModel(this);
    m_filterModel->setSourceModel(m_galleryModel);
    ui->galleryView->setModel(m_filterModel);
    ui->galleryView->setItemDelegate(new GalleryDelegate(this));
    ui->galleryView->setViewMode(QListView::IconMode);
    ui->galleryView->setResizeMode(QListView::Adjust);
    ui->galleryView->setUniformItemSizes(true);
    ui->galleryView->setSpacing(GalleryDelegate::Margin);

    m_progressBar = new QProgressBar(this);
    statusBar()->addPermanentWidget(m_progressBar, 1);
    connect(m_galleryModel, &GalleryModel::progressChanged, this, &MainWindow::updateProgressBar);
    m_progressBar->hide();

    ui->allButton->setShortcut(QKeySequence("1"));
    connect(ui->allButton, &QToolButton::toggled, this, [this](bool checked) {
        if (checked)
            m_filterModel->setFilter(GalleryFilterProxyModel::All);
    });
    ui->onlyVisibleButton->setShortcut(QKeySequence("2"));
    connect(ui->onlyVisibleButton, &QToolButton::toggled, this, [this](bool checked) {
        if (checked)
            m_filterModel->setFilter(GalleryFilterProxyModel::OnlyVisible);
    });
    ui->onlyHiddenButton->setShortcut(QKeySequence("3"));
    connect(ui->onlyHiddenButton, &QToolButton::toggled, this, [this](bool checked) {
        if (checked)
            m_filterModel->setFilter(GalleryFilterProxyModel::OnlyHidden);
    });

    ui->visibilityButton->setShortcut(QKeySequence("Space"));
    connect(ui->visibilityButton, &QToolButton::clicked, this, &MainWindow::toggleMedia);

    auto deleteShortcut = new QShortcut(QKeySequence(QKeySequence::Delete), this);
    connect(deleteShortcut, &QShortcut::activated, this, &MainWindow::deleteMedia);

    auto goUpShortcut = new QShortcut(QKeySequence("Alt+Up"), this);
    connect(goUpShortcut, &QShortcut::activated, this, &MainWindow::goUp);
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

QModelIndex MainWindow::selectedSourceIndex()
{
    const auto index = ui->galleryView->selectionModel()->currentIndex();
    if (!index.isValid())
        return {};
    return m_filterModel->mapToSource(index);
}

void MainWindow::toggleMedia()
{
    const auto index = selectedSourceIndex();
    if (!index.isValid())
        return;
    m_galleryModel->toggleMedia(index);
}

void MainWindow::deleteMedia()
{
    const auto index = selectedSourceIndex();
    if (!index.isValid())
        return;
    m_galleryModel->deleteMedia(index);
}

void MainWindow::goUp()
{
    QDir dir(m_galleryModel->path());
    dir.cdUp();
    const auto index = m_fileModel->index(dir.path());
    ui->directoryView->selectionModel()->setCurrentIndex(index,
                                                         QItemSelectionModel::ClearAndSelect);
}
