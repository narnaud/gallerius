#include "constants.h"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QSettings>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Gallerius");
    QCoreApplication::setApplicationName("Gallerius");

    QApplication app(argc, argv);

    QSettings settings;
    QString rootPath = settings.value(Constants::RootPathKey).toString();
    if (rootPath.isEmpty()) {
        rootPath = QFileDialog::getExistingDirectory(nullptr, "Choose the root directory",
                                                     QDir::homePath());
        settings.setValue(Constants::RootPathKey, rootPath);
    }

    return app.exec();
}
