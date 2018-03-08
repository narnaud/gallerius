#ifndef DATA_H
#define DATA_H

#include <QImage>
#include <QObject>

#include <vector>

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
    bool filter = false;
};

struct Data
{
    std::vector<Media> media;
};

#endif // DATA_H
