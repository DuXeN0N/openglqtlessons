QT += core gui opengl

TARGET = spinning_cube

TEMPLATE = app

SOURCES += main.cpp\
           glwidget.cpp

HEADERS += glwidget.h

OTHER_FILES +=\
    fragment_shader.frag \
    vertex_shader.vert

RESOURCES += resources.qrc
