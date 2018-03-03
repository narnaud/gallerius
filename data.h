#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QPixmap>
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
    QPixmap thumbnail;
    bool excluded = false;
};

struct Data
{
    QVector<Media> media;
};

#endif // DATA_H
