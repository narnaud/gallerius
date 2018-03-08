#ifndef GALLERYFILTERPROXYMODEL_H
#define GALLERYFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class GalleryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool showAll READ isAllVisible WRITE showAll NOTIFY showAllChanged)

public:
    GalleryFilterProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool isAllVisible() const;

public slots:
    void showAll(bool showAll);

signals:
    void showAllChanged(bool showAll);

private:
    bool m_showAll = true;
};

#endif // GALLERYFILTERPROXYMODEL_H
