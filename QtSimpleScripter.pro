#-------------------------------------------------
#
# Project created by QtCreator 2014-05-19T20:54:51
#
#-------------------------------------------------

QT       += core gui sql widgets core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSimpleScripter
TEMPLATE = app
CONFIG += c++17

RC_ICONS = SimpleScriptor.ico

SOURCES += main.cpp\
    forms/basewindow.cpp \
    core/datamodule.cpp \
    forms/highlightpatternwindow.cpp \
    lookupitemdelegate.cpp \
    forms/patternschemewindow.cpp \
    forms/projecteditwindow.cpp \
    forms/projectlistwindow.cpp \
    core/regexphighlighter.cpp \
    forms/regexptestwindow.cpp \
    forms/replacepatternwindow.cpp \
    forms/speakerwindow.cpp \
    forms/statementhistorydialog.cpp \
    forms/statementnavigationwindow.cpp \
    forms/statementwindow.cpp \
    core/tablehighlighter.cpp \
    core/textprocessor.cpp \
    forms/twovaluesinputdialog.cpp \
    models/statementfiltermodel.cpp \
    models/statementmodel.cpp \
    utils/fileutils.cpp \
    utils/sqlqueryhelper.cpp \
    widgets/dictcombobox.cpp \
    widgets/qcolorbox.cpp \
    models/lsqltablemodel.cpp \
    widgets/qcheckboxcombo.cpp \
    mainscripterwindow.cpp \
    utils/sparams.cpp \
    widgets/qsmartdialog.cpp \
    core/qdatabaseupdater.cpp \
    models/lsqllinkedrecordsmodel.cpp \
    core/appsettings.cpp \
    utils/slogger.cpp \
    utils/strutils.cpp \
    widgets/colormonitor.cpp

HEADERS  += \
    forms/basewindow.h \
    core/datamodule.h \
    forms/highlightpatternwindow.h \
    lookupitemdelegate.h \
    forms/patternschemewindow.h \
    forms/projecteditwindow.h \
    forms/projectlistwindow.h \
    core/regexphighlighter.h \
    forms/regexptestwindow.h \
    forms/replacepatternwindow.h \
    forms/speakerwindow.h \
    forms/statementhistorydialog.h \
    forms/statementnavigationwindow.h \
    forms/statementwindow.h \
    core/tablehighlighter.h \
    core/textprocessor.h \
    forms/twovaluesinputdialog.h \
    models/statementfiltermodel.h \
    models/statementmodel.h \
    utils/fileutils.h \
    utils/sqlqueryhelper.h \
    widgets/dictcombobox.h \
    widgets/qcolorbox.h \
    models/lsqltablemodel.h \
    widgets/qcheckboxcombo.h \
    mainscripterwindow.h \
    utils/sparams.h \
    widgets/qsmartdialog.h \
    core/qdatabaseupdater.h \
    models/lsqllinkedrecordsmodel.h \
    core/appconst.h \
    core/appsettings.h \
    utils/slogger.h \
    utils/strutils.h \
    widgets/colormonitor.h

FORMS    += \
    forms/basewindow.ui \
    forms/highlightpatternwindow.ui \
    forms/patternschemewindow.ui \
    forms/projecteditwindow.ui \
    forms/projectlistwindow.ui \
    mainscripterwindow.ui \
    forms/regexptestwindow.ui \
    forms/replacepatternwindow.ui \
    forms/speakerwindow.ui \
    forms/statementhistorydialog.ui \
    forms/statementnavigationwindow.ui \
    forms/statementwindow.ui \
    forms/twovaluesinputdialog.ui \
    widgets/qsmartdialog.ui

RESOURCES += \
    icons.qrc
