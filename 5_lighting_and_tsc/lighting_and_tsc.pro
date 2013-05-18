QT += core gui opengl

TARGET = lighting_and_tsc

TEMPLATE = app

SOURCES += main.cpp\
           glwidget.cpp

HEADERS += glwidget.h

INCLUDEPATH += GL\

OTHER_FILES +=\
    fragment_shader.frag \
    vertex_shader.vert \
    lighting_fragment_shader.frag \
    lighting_vertex_shader.vert \
    lighting_lambert_vertex_shader.vert \
    lighting_lambert_fragment_shader.frag \
    lighting_toon_vertex_shader.vert \
    lighting_toon_fragment_shader.frag \
    lighting_ss_fragment_shader.frag \
    lighting_ss_vertex_shader.vert

RESOURCES += resources.qrc
