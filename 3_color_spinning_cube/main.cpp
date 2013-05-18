#include <QApplication>
#include "glwidget.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QGLFormat glFormat;

    // glFormat.setVersion(3, 3); // need VAO and glew

    glFormat.setProfile      (QGLFormat::CoreProfile); // Requires Qt v >= 4.8.0
    glFormat.setSampleBuffers(true);

    GlWidget w(glFormat);
    w.show();

    qDebug() << "GL_VENDOR:\t"
             << QString((char*)glGetString(GL_VENDOR)).trimmed()
             << "\nGL_RENDERER:\t"
             << QString((char*)glGetString(GL_RENDERER)).trimmed()
             << "\nGL_VERSION:\t"
             << QString((char*)glGetString(GL_VERSION)).trimmed();
            //<<  "GL_SHADING_LANGUAGE_VERSION" << glGetString(GL_);
    return a.exec();
}
