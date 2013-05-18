#-------------------------------------------------
#
# Project created by QtCreator 2012-12-10T08:08:10
#
#-------------------------------------------------

QT += core gui opengl

TARGET = bezier_surfaces

TEMPLATE = app

SOURCES += main.cpp\
           glwidget.cpp

HEADERS += glwidget.h

OTHER_FILES +=\
    fragment_shader.frag \
    vertex_shader.vert

RESOURCES += resources.qrc
