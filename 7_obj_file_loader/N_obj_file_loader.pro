QT += core gui opengl

TARGET = N_obj_file_loader

TEMPLATE = app

SOURCES += main.cpp\
           glwidget.cpp

HEADERS += glwidget.h

OTHER_FILES +=\
    fragment_shader.frag \
    vertex_shader.vert

RESOURCES += resources.qrc
