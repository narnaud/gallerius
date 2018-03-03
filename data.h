#ifndef DATA_H
#define DATA_H

#include <QImage>
#include <QObject>
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
    QImage thumbnail;
    bool excluded = false;
};

struct Data
{
    QVector<Media> media;
};

#endif // DATA_H
