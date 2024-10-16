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
    basewindow.cpp \
    datamodule.cpp \
    highlightpatternwindow.cpp \
    lookupitemdelegate.cpp \
    patternschemewindow.cpp \
    projecteditwindow.cpp \
    projectlistwindow.cpp \
    regexphighlighter.cpp \
    regexptestwindow.cpp \
    replacepatternwindow.cpp \
    speakerwindow.cpp \
    statementhistorydialog.cpp \
    statementnavigationwindow.cpp \
    statementwindow.cpp \
    tablehighlighter.cpp \
    textprocessor.cpp \
    twovaluesinputdialog.cpp \
    widgets/dictcombobox.cpp \
    widgets/qcolorbox.cpp \
    models/lsqltablemodel.cpp \
    widgets/qcheckboxcombo.cpp \
    mainscripterwindow.cpp \
    utils/sparams.cpp \
    widgets/qsmartdialog.cpp \
    utils/qdatabaseupdater.cpp \
    models/lsqllinkedrecordsmodel.cpp \
    models/qstatementmodel.cpp \
    models/qstatementfiltermodel.cpp \
    utils/appsettings.cpp \
    utils/qsqlqueryhelper.cpp \
    utils/slogger.cpp \
    utils/qfileutils.cpp \
    utils/strutils.cpp \
    widgets/colormonitor.cpp

HEADERS  += \
    basewindow.h \
    datamodule.h \
    highlightpatternwindow.h \
    lookupitemdelegate.h \
    patternschemewindow.h \
    projecteditwindow.h \
    projectlistwindow.h \
    regexphighlighter.h \
    regexptestwindow.h \
    replacepatternwindow.h \
    speakerwindow.h \
    statementhistorydialog.h \
    statementnavigationwindow.h \
    statementwindow.h \
    tablehighlighter.h \
    textprocessor.h \
    twovaluesinputdialog.h \
    widgets/dictcombobox.h \
    widgets/qcolorbox.h \
    models/lsqltablemodel.h \
    widgets/qcheckboxcombo.h \
    mainscripterwindow.h \
    utils/sparams.h \
    widgets/qsmartdialog.h \
    utils/qdatabaseupdater.h \
    models/lsqllinkedrecordsmodel.h \
    models/qstatementmodel.h \
    models/qstatementfiltermodel.h \
    utils/appconst.h \
    utils/appsettings.h \
    utils/qsqlqueryhelper.h \
    utils/slogger.h \
    utils/qfileutils.h \
    utils/strutils.h \
    widgets/colormonitor.h

FORMS    += \
    basewindow.ui \
    highlightpatternwindow.ui \
    patternschemewindow.ui \
    projecteditwindow.ui \
    projectlistwindow.ui \
    mainscripterwindow.ui \
    regexptestwindow.ui \
    replacepatternwindow.ui \
    speakerwindow.ui \
    statementhistorydialog.ui \
    statementnavigationwindow.ui \
    statementwindow.ui \
    twovaluesinputdialog.ui \
    widgets/qsmartdialog.ui

RESOURCES += \
    icons.qrc
