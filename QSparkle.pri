QT += core gui widgets network webkitwidgets

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD/lib/quazip/quazip
DEPENDPATH += $$PWD/lib/quazip/quazip

INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib

HEADERS += \
	$$PWD/qsparkle.h \
	$$PWD/qsparkleitem.h \
	$$PWD/qsparkleitemversion.h \
	$$PWD/lib/semver/include/semver/version.hpp \
	$$PWD/qsparkledownloaddialog.h \
	$$PWD/qsparklereleasewindow.h

SOURCES += \
	$$PWD/qsparkle.cpp \
	$$PWD/qsparkleitem.cpp \
	$$PWD/qsparkleitemversion.cpp \
	$$PWD/qsparkledownloaddialog.cpp \
	$$PWD/qsparklereleasewindow.cpp

FORMS += \
	$$PWD/qsparklereleasewindow.ui \
	$$PWD/qsparkledownloaddialog.ui

unix {
	LIBS += -L$$PWD/lib/quazip/quazip/ -lquazip
}

win32 {
	LIBS += -L$$PWD/lib/quazip/quazip/release -lquazip

	libquazip.files = $$PWD/lib/quazip/quazip/libquazip.1.dylib
}

macx {
	libquazip.path = Contents/Frameworks
	libquazip.files = $$PWD/lib/quazip/quazip/libquazip.1.dylib
}

QMAKE_BUNDLE_DATA += libquazip
