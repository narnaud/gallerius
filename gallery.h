#ifndef GALLERY_H
#define GALLERY_H

#include <QObject>
#include <QUrl>
#include <QVector>

struct Media
{
    Q_GADGET
public:
    enum Type { Dir, Image, Video, NoType };
    Q_ENUM(Type);

    QString fileName;
    QString filePath;
    Type type = NoType;
};

struct Data
{
    QVector<Media> media;
};

class Gallery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)
    Q_PROPERTY(QUrl path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit Gallery(QObject *parent = nullptr);

    QUrl rootPath() const;
    QUrl path() const;
    QVector<Media> media() const;

signals:
    void rootPathChanged(QUrl rootPath);
    void pathChanged(QUrl path);
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
