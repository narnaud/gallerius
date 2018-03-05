#ifndef GALLERY_H
#define GALLERY_H

#include "data.h"

#include <QObject>
#include <QUrl>
#include <QVector>

class Gallery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)
    Q_PROPERTY(QUrl path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool isRoot READ isRoot NOTIFY isRootChanged)

public:
    explicit Gallery(QObject *parent = nullptr);

    QUrl rootPath() const;
    QUrl path() const;
    QVector<Media> media() const;
    bool isRoot() const;

    int mediaCount() const;
    const Media &media(int index) const;

signals:
    void rootPathChanged(QUrl rootPath);
    void pathChanged(QUrl path);
    void isRootChanged(bool isRoot);

    void dataAboutToChange();
    void dataChanged();

public slots:
    void setRootPath(QUrl rootPath);
    void setPath(QUrl path);

private:
    void loadData();

private:
    QUrl m_rootPath;
    Data m_data;
    QUrl m_path;
};

#endif // GALLERY_H
