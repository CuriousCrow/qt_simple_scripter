#-------------------------------------------------
#
# Project created by QtCreator 2014-05-19T20:54:51
#
#-------------------------------------------------

QT       += core gui network core5compat qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSimpleScripter
TEMPLATE = app
CONFIG += c++11

RC_ICONS = SimpleScriptor.ico

SOURCES += main.cpp\
    core/datamodule.cpp \
    core/regexphighlighter.cpp \
    core/tablehighlighter.cpp \
    core/textprocessor.cpp \
    forms/basewindow.cpp \
    forms/highlightpatternwindow.cpp \
    forms/patternschemewindow.cpp \
    forms/projecteditwindow.cpp \
    forms/projectlistwindow.cpp \
    forms/regexptestwindow.cpp \
    forms/replacepatternwindow.cpp \
    forms/speakerwindow.cpp \
    forms/statementhistorydialog.cpp \
    forms/statementnavigationwindow.cpp \
    forms/statementwindow.cpp \
    qlookupitemdelegate.cpp \
    utils/fileutils.cpp \
    widgets/dictcombobox.cpp \
    widgets/qcolorbox.cpp \
    widgets/qcheckboxcombo.cpp \
    forms/mainscripterwindow.cpp \
    utils/sparams.cpp \
    widgets/qsmartdialog.cpp \
    dialogs/qtwovaluesinputdialog.cpp \
    utils/appsettings.cpp \
    utils/slogger.cpp \
    utils/strutils.cpp \
    widgets/colormonitor.cpp \
    models/abstractresttablemodel.cpp \
    models/basejsonaction.cpp \
    models/baseresttablemodel.cpp \
    models/cachedresttablemodel.cpp \
    models/readonlyresttablemodel.cpp

HEADERS  += \
    core/datamodule.h \
    core/regexphighlighter.h \
    core/tablehighlighter.h \
    core/textprocessor.h \
    forms/basewindow.h \
    forms/highlightpatternwindow.h \
    forms/patternschemewindow.h \
    forms/projecteditwindow.h \
    forms/projectlistwindow.h \
    forms/regexptestwindow.h \
    forms/replacepatternwindow.h \
    forms/speakerwindow.h \
    forms/statementhistorydialog.h \
    forms/statementnavigationwindow.h \
    forms/statementwindow.h \
    qlookupitemdelegate.h \
    utils/fileutils.h \
    widgets/dictcombobox.h \
    widgets/qcolorbox.h \
    widgets/qcheckboxcombo.h \
    forms/mainscripterwindow.h \
    utils/sparams.h \
    widgets/qsmartdialog.h \
    dialogs/qtwovaluesinputdialog.h \
    core/appconst.h \
    utils/appsettings.h \
    utils/slogger.h \
    utils/strutils.h \
    widgets/colormonitor.h \
    models/abstractresttablemodel.h \
    models/basejsonaction.h \
    models/baseresttablemodel.h \
    models/cachedresttablemodel.h \
    models/readonlyresttablemodel.h

FORMS    += \
    forms/basewindow.ui \
    forms/highlightpatternwindow.ui \
    forms/patternschemewindow.ui \
    forms/projecteditwindow.ui \
    forms/projectlistwindow.ui \
    forms/mainscripterwindow.ui \
    forms/regexptestwindow.ui \
    forms/replacepatternwindow.ui \
    forms/speakerwindow.ui \
    forms/statementhistorydialog.ui \
    forms/statementnavigationwindow.ui \
    forms/statementwindow.ui \
    widgets/qsmartdialog.ui \
    dialogs/qtwovaluesinputdialog.ui

RESOURCES += \
    icons.qrc
