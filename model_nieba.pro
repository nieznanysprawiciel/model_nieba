#-------------------------------------------------
#
# Project created by QtCreator 2014-07-25T17:54:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = model_nieba
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    completeskymodel.cpp \
    HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModel.cpp \
    HosekWilkie_SkylightModel_C_Source.1.4a/SolarRadiance_RGB.cpp \
    sky_thread.cpp \
    skydisplayer.cpp \
    spectral_to_RGB/SpectralConversion.cpp \
    SunPosition.cpp

HEADERS  += mainwindow.h \
    HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModel.h \
    HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModelData_CIEXYZ.h \
    HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModelData_RGB.h \
    HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModelData_Spectral.h \
    declarations.h \
    completeskymodel.h \
    HosekWilkie_SkylightModel_C_Source.1.4a/solarRadianceRGB.h \
    HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModelSolarRadiance_RGB.h \
    sky_thread.h \
    skydisplayer.h \
    spectral_to_RGB/CIEXYZ_distribution.h \
    spectral_to_RGB/SpectralConversion.h \
    SunPosition.h


FORMS    += mainwindow.ui
