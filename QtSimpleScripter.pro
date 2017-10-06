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
    models/qusersqltablemodel.cpp \
    qlookupitemdelegate.cpp \
    qhighlightpatternwindow.cpp \
    widgets/qcolorbox.cpp \
    qpatternschemewindow.cpp \
    ../../../Qt/LSqlTableModel/trunk/lsqltablemodel.cpp \
    qtablehighlighter.cpp \
    qprojecteditwindow.cpp \
    qtextprocessor.cpp \
    ../../../Qt/QCheckboxCombo/trunk/qcheckboxcombo.cpp \
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
    ../../../Qt/LSqlTableModel/trunk/lsqllinkedrecordsmodel.cpp \
    qstatementnavigationwindow.cpp \
    models/qstatementmodel.cpp \
    qtwovaluesinputdialog.cpp \
    models/qstatementfiltermodel.cpp \
    utils/appsettings.cpp \
    utils/qsqlqueryhelper.cpp \
    utils/slogger.cpp

HEADERS  += \
    qdatamodule.h \
    qstatementwindow.h \
    qspeakerwindow.h \
    models/qusersqltablemodel.h \
    qlookupitemdelegate.h \
    qhighlightpatternwindow.h \
    widgets/qcolorbox.h \
    qpatternschemewindow.h \
    ../../../Qt/LSqlTableModel/trunk/lsqltablemodel.h \
    qtablehighlighter.h \
    qprojecteditwindow.h \
    qtextprocessor.h \
    ../../../Qt/QCheckboxCombo/trunk/qcheckboxcombo.h \
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
    ../../../Qt/LSqlTableModel/trunk/lsqllinkedrecordsmodel.h \
    qstatementnavigationwindow.h \
    models/qstatementmodel.h \
    qtwovaluesinputdialog.h \
    models/qstatementfiltermodel.h \
    utils/appconst.h \
    utils/appsettings.h \
    utils/qsqlqueryhelper.h \
    utils/slogger.h

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
