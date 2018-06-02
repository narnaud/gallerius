#include "constants.h"
#include "data.h"
#include "gallery.h"
#include "galleryfilterproxymodel.h"
#include "gallerymodel.h"
#include "mediaproxymodel.h"
#include "thumbnailprovider.h"

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
    GalleryModel model(&gallery);
    GalleryFilterProxyModel proxyModel;
    proxyModel.setSourceModel(&model);
    MediaProxyModel mediaProxyModel;
    mediaProxyModel.setSourceModel(&proxyModel);

    QSettings settings;
    QUrl savedRootPath = settings.value(Constants::RootPathKey).toUrl();

    QQmlApplicationEngine engine;
    engine.addImageProvider("thumbnail", new ThumbnailProvider(&model));
    engine.rootContext()->setContextProperty("_gallery", &gallery);
    engine.rootContext()->setContextProperty("_savedRootPath", savedRootPath);
    engine.rootContext()->setContextProperty("_model", &proxyModel);
    engine.rootContext()->setContextProperty("_mediaModel", &mediaProxyModel);

    qmlRegisterUncreatableType<Media>("Gallerius", 1, 0, "Media",
                                      "You cannot create an instance of Media.");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}