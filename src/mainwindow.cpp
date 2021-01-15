#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gallerydelegate.h"
#include "galleryfilterproxymodel.h"
#include "gallerymodel.h"
#include "mediaview.h"

#include <QFileSystemModel>
#include <QProgressBar>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , m_fileModel(new QFileSystemModel(this))
    , m_galleryModel(new GalleryModel(this))
    , m_mediaView(new MediaView(m_galleryModel, this))
{
    ui->setupUi(this);
    m_mediaView->hide();

    m_fileModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->directoryView->setModel(m_fileModel);
    ui->directoryView->hideColumn(1);
    ui->directoryView->hideColumn(2);
    ui->directoryView->hideColumn(3);
    connect(ui->directoryView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            [this](const QModelIndex &index) { openPath(m_fileModel->filePath(index)); });

    m_filterModel = new GalleryFilterProxyModel(this);
    m_filterModel->setSourceModel(m_galleryModel);
    ui->galleryView->setModel(m_filterModel);
    ui->galleryView->setItemDelegate(new GalleryDelegate(this));
    ui->galleryView->setViewMode(QListView::IconMode);
    ui->galleryView->setResizeMode(QListView::Adjust);
    ui->galleryView->setUniformItemSizes(true);
    ui->galleryView->setSpacing(GalleryDelegate::Margin);
    // Ensure to select something
    connect(m_filterModel, &GalleryFilterProxyModel::modelReset, this, [this]() {
        m_mediaView->hide();
        ui->galleryView->selectionModel()->setCurrentIndex(m_filterModel->index(0, 0),
                                                           QItemSelectionModel::ClearAndSelect);
    });
    // Connection for the media view
    connect(ui->galleryView->selectionModel(), &QItemSelectionModel::currentChanged, m_mediaView,
            &MediaView::setCurrentIndex);
    connect(m_filterModel, &GalleryFilterProxyModel::dataChanged, m_mediaView,
            qOverload<>(&MediaView::update));

    connect(m_galleryModel, &GalleryModel::titleChanged, ui->titleEdit, &QLineEdit::setText);
    connect(ui->titleEdit, &QLineEdit::editingFinished, m_galleryModel,
            [this]() { m_galleryModel->setTitle(ui->titleEdit->text()); });

    connect(m_galleryModel, &GalleryModel::progressChanged, this, &MainWindow::updateProgressBar);
    ui->progressBar->hide();

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

    auto cdUpShortcut = new QShortcut(QKeySequence("Alt+Up"), this);
    connect(cdUpShortcut, &QShortcut::activated, this, &MainWindow::cdUp);

    auto goDownShortcut = new QShortcut(QKeySequence(Qt::Key_PageDown), this);
    connect(goDownShortcut, &QShortcut::activated, this, &MainWindow::goDown);

    auto goUpShortcut = new QShortcut(QKeySequence(Qt::Key_PageUp), this);
    connect(goUpShortcut, &QShortcut::activated, this, &MainWindow::goUp);

    auto focusShortcut = new QShortcut(QKeySequence("Esc"), this);
    connect(focusShortcut, &QShortcut::activated, this, [this]() {
        ui->galleryView->setFocus();
        m_mediaView->hide();
    });

    auto actionShortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(actionShortcut, &QShortcut::activated, this,
            [this]() { doAction(ui->galleryView->currentIndex()); });
    connect(ui->galleryView, &QListView::doubleClicked, this, &MainWindow::doAction);
}

MainWindow::~MainWindow() { }

void MainWindow::setRootPath(const QString &rootPath)
{
    m_rootPath = rootPath;
    m_fileModel->setRootPath(rootPath);
    ui->directoryView->setRootIndex(m_fileModel->index(rootPath));
    ui->directoryView->selectionModel()->setCurrentIndex(ui->directoryView->rootIndex(),
                                                         QItemSelectionModel::ClearAndSelect);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_mediaView->setGeometry(0, 0, width(), height());
    QWidget::resizeEvent(event);
}

void MainWindow::updateProgressBar(int value, int total)
{
    const bool hide = total != 0 && value != total;
    ui->progressBar->setVisible(hide);
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(value);
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

void MainWindow::cdUp()
{
    QDir dir(m_galleryModel->path());
    dir.cdUp();
    if (dir.path() < m_rootPath)
        return;

    const auto index = m_fileModel->index(dir.path());
    ui->directoryView->selectionModel()->setCurrentIndex(index,
                                                         QItemSelectionModel::ClearAndSelect);
}

void MainWindow::goDown()
{
    navigate(1);
}

void MainWindow::goUp()
{
    navigate(-1);
}

void MainWindow::navigate(int delta)
{
    const auto &current = ui->directoryView->currentIndex();
    if (!current.isValid())
        return;

    const auto index = current.sibling(current.row() + delta, 0);
    if (index.isValid()) {
        ui->directoryView->selectionModel()->setCurrentIndex(index,
                                                             QItemSelectionModel::ClearAndSelect);
        ui->directoryView->expand(ui->directoryView->currentIndex().parent());
    }
}

void MainWindow::doAction(const QModelIndex &index)
{
    Q_ASSERT(index.isValid());

    auto type = index.data(GalleryModel::MediaRole).value<GalleryModel::Type>();
    if (type == GalleryModel::Dir) {
        openPath(index.data(GalleryModel::FullPathRole).toString());
    } else {
        m_mediaView->show();
        m_mediaView->raise();
    }
}

void MainWindow::openPath(const QString &path)
{
    m_galleryModel->setPath(path);
    ui->directoryView->setCurrentIndex(m_fileModel->index(path));
    ui->directoryView->expand(ui->directoryView->currentIndex().parent());
}
