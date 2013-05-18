// Add new headers for mouse events!
#include <QMouseEvent>
#include <QWheelEvent>

#include "glwidget.h"

void GlWidget::initializeGL()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  qglClearColor(QColor(Qt::lightGray));

  if (!prepareShaderProgram( ":/vertex_shader.vert", ":/fragment_shader.frag")) {
    return;
  }

  vertices
    << QVector3D(-0.5f, -0.5f,  0.5f) << QVector3D( 0.5f, -0.5f,  0.5f) << QVector3D( 0.5f,  0.5f,  0.5f) // Front
    << QVector3D( 0.5f,  0.5f,  0.5f) << QVector3D(-0.5f,  0.5f,  0.5f) << QVector3D(-0.5f, -0.5f,  0.5f)
    << QVector3D( 0.5f, -0.5f, -0.5f) << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D(-0.5f,  0.5f, -0.5f) // Back
    << QVector3D(-0.5f,  0.5f, -0.5f) << QVector3D( 0.5f,  0.5f, -0.5f) << QVector3D( 0.5f, -0.5f, -0.5f)
    << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D(-0.5f, -0.5f,  0.5f) << QVector3D(-0.5f,  0.5f,  0.5f) // Left
    << QVector3D(-0.5f,  0.5f,  0.5f) << QVector3D(-0.5f,  0.5f, -0.5f) << QVector3D(-0.5f, -0.5f, -0.5f)
    << QVector3D( 0.5f, -0.5f,  0.5f) << QVector3D( 0.5f, -0.5f, -0.5f) << QVector3D( 0.5f,  0.5f, -0.5f) // Right
    << QVector3D( 0.5f,  0.5f, -0.5f) << QVector3D( 0.5f,  0.5f,  0.5f) << QVector3D( 0.5f, -0.5f,  0.5f)
    << QVector3D(-0.5f,  0.5f,  0.5f) << QVector3D( 0.5f,  0.5f,  0.5f) << QVector3D( 0.5f,  0.5f, -0.5f) // Top
    << QVector3D( 0.5f,  0.5f, -0.5f) << QVector3D(-0.5f,  0.5f, -0.5f) << QVector3D(-0.5f,  0.5f,  0.5f)
    << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D( 0.5f, -0.5f, -0.5f) << QVector3D( 0.5f, -0.5f,  0.5f) // Bottom
    << QVector3D( 0.5f, -0.5f,  0.5f) << QVector3D(-0.5f, -0.5f,  0.5f) << QVector3D(-0.5f, -0.5f, -0.5f);

  colors
    << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f) // Front
    << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f)
    << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f) // Back
    << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f) << QVector3D(0.5f, 0.3f, 0.3f)
    << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f) // Left
    << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f)
    << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f) // Right
    << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f) << QVector3D(0.1f, 0.5f, 0.3f)
    << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f) // Top
    << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f)
    << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f) // Bottom
    << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f) << QVector3D(0.1f, 0.3f, 0.5f);
}

bool GlWidget::prepareShaderProgram(const QString& vertexShaderPath,
                                    const QString& fragmentShaderPath ) {
  bool result = shaderProgram.addShaderFromSourceFile(QGLShader::Vertex,
                                                      vertexShaderPath );

  if (!result) {
    qWarning() << shaderProgram.log();
  }

  result = shaderProgram.addShaderFromSourceFile(QGLShader::Fragment,
                                                 fragmentShaderPath );
  if (!result) {
    qWarning() << shaderProgram.log();
  }

  result = shaderProgram.link();
  if (!result) {
    qWarning() << "Could not link shader program:"
               << shaderProgram.log();
  }

  return result;
}

void GlWidget::resizeGL(int width, int height) {
  if (height == 0) {
      height = 1;
  }

  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(60.0,
                               (float) width / (float) height,
                               0.001,
                               1000);

  glViewport(0, 0, width, height);
}

void GlWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 modelMatrix,
             viewMatrix,
             cameraTransformation;

  cameraTransformation.rotate(alpha, 0, 1, 0);
  cameraTransformation.rotate(beta,  1, 0, 0);

  QVector3D cameraPosition,
            cameraUpDirection;

  cameraPosition    = cameraTransformation * QVector3D(0, 0, distance);
  cameraUpDirection = cameraTransformation * QVector3D(0.0f, 1.0f, 0.0f);

  viewMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

  shaderProgram.bind();

  shaderProgram.setUniformValue("mvpMatrix", projectionMatrix *
                                             viewMatrix *
                                             modelMatrix);

  shaderProgram.setAttributeArray   ("vertex", vertices.constData());
  shaderProgram.enableAttributeArray("vertex");
  shaderProgram.setAttributeArray   ("color",  colors.constData());
  shaderProgram.enableAttributeArray("color");

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  shaderProgram.disableAttributeArray("vertex");
  shaderProgram.disableAttributeArray("color");
  shaderProgram.release();
}

void GlWidget::mousePressEvent(QMouseEvent *mpe) {
  lastMousePosition = mpe->pos();
  mpe->accept();
}


void GlWidget::mouseMoveEvent(QMouseEvent *mme) {
  int deltaX = mme->x() - lastMousePosition.x(),
      deltaY = mme->y() - lastMousePosition.y();

  if (mme->buttons() & Qt::LeftButton) {
    alpha -= deltaX;
    if (alpha < 0.0f) {
      alpha += 360.0f;
    }
    if (alpha >= 360.0f) {
      alpha -= 360.0f;
    }

    beta -= deltaY;
    if (beta < -180.0f) {
      beta = -180.0f;
    }
    if (beta > 180.0f) {
      beta = 180.0f;
    }

    updateGL();
  }

  lastMousePosition = mme->pos();
  mme->accept();
}

// Increase or decrease the viewers distance
// by 0.1 and update our GlWidget.
void GlWidget::wheelEvent(QWheelEvent *we) {
  int delta = we->delta();

  if (we->orientation() == Qt::Vertical) {
    if (delta < 0.0f) {
      distance *= 1.1f;
    } else if (delta > 0.0f) {
      distance *= 0.9f;
    }

    updateGL();
  }
  we->accept();
}
