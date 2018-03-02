#ifndef DATA_H
#define DATA_H

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
};

struct Data
{
    QVector<Media> media;
};

#endif // DATA_H
