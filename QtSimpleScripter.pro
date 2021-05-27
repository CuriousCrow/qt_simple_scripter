#-------------------------------------------------
#
# Project created by QtCreator 2014-05-19T20:54:51
#
#-------------------------------------------------

QT       += core gui sql script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSimpleScripter
TEMPLATE = app
CONFIG += c++11

RC_ICONS = SimpleScriptor.ico

SOURCES += main.cpp\
    qdatamodule.cpp \
    qstatementwindow.cpp \
    qspeakerwindow.cpp \
    qlookupitemdelegate.cpp \
    qhighlightpatternwindow.cpp \
    widgets/dictcombobox.cpp \
    widgets/qcolorbox.cpp \
    qpatternschemewindow.cpp \
    models/lsqltablemodel.cpp \
    qtablehighlighter.cpp \
    qprojecteditwindow.cpp \
    qtextprocessor.cpp \
    ../QCheckboxCombo/qcheckboxcombo.cpp \
    qprojectlistwindow.cpp \
    mainscripterwindow.cpp \
    qreplacepatternwindow.cpp \
    qregexptestwindow.cpp \
    qregexphighlighter.cpp \
    qstatementhistorydialog.cpp \
    utils/sparams.cpp \
    qbasewindow.cpp \
    widgets/qsmartdialog.cpp \
    utils/qdatabaseupdater.cpp \
    models/lsqllinkedrecordsmodel.cpp \
    qstatementnavigationwindow.cpp \
    models/qstatementmodel.cpp \
    qtwovaluesinputdialog.cpp \
    models/qstatementfiltermodel.cpp \
    utils/appsettings.cpp \
    utils/qsqlqueryhelper.cpp \
    utils/slogger.cpp \
    utils/qfileutils.cpp \
    utils/strutils.cpp \
    widgets/colormonitor.cpp

HEADERS  += \
    qdatamodule.h \
    qstatementwindow.h \
    qspeakerwindow.h \
    qlookupitemdelegate.h \
    qhighlightpatternwindow.h \
    widgets/dictcombobox.h \
    widgets/qcolorbox.h \
    qpatternschemewindow.h \
    models/lsqltablemodel.h \
    qtablehighlighter.h \
    qprojecteditwindow.h \
    qtextprocessor.h \
    ../QCheckboxCombo/qcheckboxcombo.h \
    qprojectlistwindow.h \
    mainscripterwindow.h \
    qreplacepatternwindow.h \
    qregexptestwindow.h \
    qregexphighlighter.h \
    qstatementhistorydialog.h \
    utils/sparams.h \
    qbasewindow.h \
    widgets/qsmartdialog.h \
    utils/qdatabaseupdater.h \
    models/lsqllinkedrecordsmodel.h \
    qstatementnavigationwindow.h \
    models/qstatementmodel.h \
    qtwovaluesinputdialog.h \
    models/qstatementfiltermodel.h \
    utils/appconst.h \
    utils/appsettings.h \
    utils/qsqlqueryhelper.h \
    utils/slogger.h \
    utils/qfileutils.h \
    utils/strutils.h \
    widgets/colormonitor.h

FORMS    += \
    qstatementwindow.ui \
    qspeakerwindow.ui \
    qhighlightpatternwindow.ui \
    qpatternschemewindow.ui \
    qprojecteditwindow.ui \
    qprojectlistwindow.ui \
    mainscripterwindow.ui \
    qreplacepatternwindow.ui \
    qregexptestwindow.ui \
    qstatementhistorydialog.ui \
    qbasewindow.ui \
    widgets/qsmartdialog.ui \
    qstatementnavigationwindow.ui \
    qtwovaluesinputdialog.ui

RESOURCES += \
    icons.qrc
