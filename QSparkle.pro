QT += core gui widgets network webkitwidgets testlib

HEADERS += \
    qsparkle.h \
    qsparkleitem.h \
    qsparkleitemversion.h \
    lib/semver/include/semver/version.hpp \
    qsparkledownloaddialog.h \
    qsparklereleasewindow.h

SOURCES += \
    qsparkle.cpp \
    main.cpp \
    qsparkleitem.cpp \
    qsparkleitemversion.cpp \
    qsparkledownloaddialog.cpp \
    qsparklereleasewindow.cpp

OTHER_FILES += \
    .gitignore \
    testfile.txt

FORMS += \
    qsparkledialog.ui \
    qsparkledownloaddialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/quazip/quazip/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/quazip/quazip/debug/ -lquazip
else:unix: LIBS += -L$$PWD/lib/quazip/quazip/ -lquazip

macx {
	libquazip.path = Contents/Frameworks
	libquazip.files = $$PWD/lib/quazip/quazip/libquazip.1.dylib

	testfile.path = Contents/MacOS
	testfile.files = $$PWD/testfile.txt

	QMAKE_BUNDLE_DATA += libquazip
}

QMAKE_BUNDLE_DATA += testfile

INCLUDEPATH += $$PWD/lib/quazip/quazip
DEPENDPATH += $$PWD/lib/quazip/quazip
