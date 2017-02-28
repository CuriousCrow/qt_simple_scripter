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
    qusersqltablemodel.cpp \
    qlookupitemdelegate.cpp \
    qhighlightpatternwindow.cpp \
    qcolorbox.cpp \
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
    sparams.cpp \
    qbasewindow.cpp \
    qsmartdialog.cpp \
    qdatabaseupdater.cpp \
    ../../../Qt/LSqlTableModel/trunk/lsqllinkedrecordsmodel.cpp \
    qstatementnavigationwindow.cpp \
    qstatementmodel.cpp \
    qtwovaluesinputdialog.cpp \
    qstatementfiltermodel.cpp

HEADERS  += \
    qdatamodule.h \
    qstatementwindow.h \
    qspeakerwindow.h \
    qusersqltablemodel.h \
    qlookupitemdelegate.h \
    qhighlightpatternwindow.h \
    qcolorbox.h \
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
    sparams.h \
    qbasewindow.h \
    qsmartdialog.h \
    qdatabaseupdater.h \    
    ../../../Qt/LSqlTableModel/trunk/lsqllinkedrecordsmodel.h \
    qstatementnavigationwindow.h \
    qstatementmodel.h \
    qtwovaluesinputdialog.h \
    qstatementfiltermodel.h

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
    qsmartdialog.ui \
    qstatementnavigationwindow.ui \
    qtwovaluesinputdialog.ui

RESOURCES += \
    icons.qrc
