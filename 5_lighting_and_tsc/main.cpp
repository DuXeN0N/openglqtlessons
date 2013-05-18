#include <QApplication>
#include "glwidget.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QGLFormat glFormat;

    // glFormat.setVersion(4, 2);

    glFormat.setProfile      (QGLFormat::CoreProfile); // Qt > 4.8.0
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
