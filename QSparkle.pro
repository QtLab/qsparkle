QT += core gui widgets network webkitwidgets

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
    .gitignore

FORMS += \
    qsparkledialog.ui \
    qsparkledownloaddialog.ui
