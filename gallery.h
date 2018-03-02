#ifndef GALLERY_H
#define GALLERY_H

#include <QObject>
#include <QUrl>

class Gallery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)

public:
    explicit Gallery(QObject *parent = nullptr);

    QUrl rootPath() const;

signals:
    void rootPathChanged(QUrl rootPath);

public slots:
    void setRootPath(QUrl rootPath);

private:
    QUrl m_rootPath;
};

#endif // GALLERY_H
