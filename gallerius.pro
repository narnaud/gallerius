QT += quick concurrent multimedia
CONFIG += c++1x

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/gallery.cpp \
    src/gallerymodel.cpp \
    src/thumbnailprovider.cpp \
    src/galleryfilterproxymodel.cpp \
    src/mediaproxymodel.cpp

HEADERS += \
    src/constants.h \
    src/gallery.h \
    src/utility.h \
    src/gallerymodel.h \
    src/data.h \
    src/thumbnailprovider.h \
    src/galleryfilterproxymodel.h \
    src/mediaproxymodel.h

RESOURCES += \
    qml/qml.qrc \
    assets/assets.qrc
