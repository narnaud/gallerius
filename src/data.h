#ifndef DATA_H
#define DATA_H

#include <QImage>
#include <QMetaType>
#include <QObject>

#include <vector>

struct Media
{
    Q_GADGET
    Q_PROPERTY(Type type MEMBER type)
    Q_PROPERTY(QString fileName MEMBER fileName)
    Q_PROPERTY(QString filePath MEMBER filePath)

public:
    enum Type { Dir, Image, Video, NoType };
    Q_ENUM(Type);

    QString fileName;
    QString filePath;
    Type type = NoType;
    QImage thumbnail;
    bool filter = false;
};

Q_DECLARE_METATYPE(Media)

struct Data
{
    std::vector<Media> media;
};

#endif // DATA_H
