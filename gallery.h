#ifndef GALLERY_H
#define GALLERY_H

#include "data.h"

#include <QObject>
#include <QTemporaryDir>
#include <QUrl>
#include <QVector>

template <typename T>
class QFutureWatcher;

class Gallery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)
    Q_PROPERTY(QUrl path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool isRoot READ isRoot NOTIFY isRootChanged)
    Q_PROPERTY(int mediaCount READ mediaCount NOTIFY dataChanged)
    Q_PROPERTY(int progressValue READ progressValue NOTIFY progressValueChanged)

public:
    explicit Gallery(QObject *parent = nullptr);

    QUrl rootPath() const;
    QUrl path() const;
    bool isRoot() const;

    int mediaCount() const;
    const Media &media(int index) const;

    int progressValue() const;

signals:
    void rootPathChanged(QUrl rootPath);
    void pathChanged(QUrl path);
    void isRootChanged(bool isRoot);

    void dataAboutToChange();
    void dataChanged();

    void progressValueChanged(int value);
    void thumbnailsDone();

public slots:
    void setRootPath(QUrl rootPath);
    void setPath(QUrl path);

private:
    void loadData();

private:
    QUrl m_rootPath;
    Data m_data;
    QUrl m_path;
    QFutureWatcher<void> *m_watcher;
    QTemporaryDir m_tempDirectory;
};

#endif // GALLERY_H
