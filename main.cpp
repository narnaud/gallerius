#include "constants.h"
#include "gallery.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QUrl>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Gallerius");
    QCoreApplication::setApplicationName("Gallerius");

    QGuiApplication app(argc, argv);

    Gallery gallery;

    QSettings settings;
    QUrl savedRootPath = settings.value(Constants::RootPathKey).toUrl();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("_gallery", &gallery);
    engine.rootContext()->setContextProperty("_savedRootPath", savedRootPath);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
