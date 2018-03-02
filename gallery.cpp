#include "gallery.h"

#include "constants.h"

#include <QDebug>
#include <QSettings>

Gallery::Gallery(QObject *parent)
    : QObject(parent)
{
}

QUrl Gallery::rootPath() const
{
    return m_rootPath;
}

void Gallery::setRootPath(QUrl rootPath)
{
    if (m_rootPath == rootPath)
        return;

    m_rootPath = rootPath;
    qInfo() << "Root path: " << m_rootPath;

    // Save the new root path
    QSettings settings;
    settings.setValue(Constants::RootPathKey, m_rootPath);

    emit rootPathChanged(m_rootPath);
}
