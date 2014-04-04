include(QSparkle.pri)

SOURCES += \
	$$PWD/main.cpp

OTHER_FILES += \
    .gitignore \
    testfile.txt \
    QSparkle.pri

macx {
	testfile.path = Contents/MacOS
	testfile.files = $$PWD/testfile.txt
}

QMAKE_BUNDLE_DATA += testfile
