#include <QMouseEvent>
#include <QWheelEvent>
#include <QSizePolicy>
#include "glwidget.h"

GlWidget::GlWidget(const QGLFormat& format, QWidget *parent) :
  QGLWidget(format, parent),
  alpha  (25.0f),
  beta   (25.0f),
  distance(2.5f) {

  initSurface();

  slider = new QSlider(Qt::Horizontal);

  slider->setWindowOpacity(0.75f);
  slider->setWindowFlags  (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
  slider->move            (pos().x() + 9, pos().y() + 28);
  slider->setFocusPolicy  (Qt::StrongFocus);
  slider->setTickPosition (QSlider::TicksBothSides);
  slider->setRange        (20, 140);
  slider->setValue        (20);
  slider->setTickInterval (10);
  slider->setSingleStep   (1);

  slider->show();

  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateDetailLevel(int)));
}

void GlWidget::initSurface() {
  A.setX(-2.0f), A.setY( 2.0f), A.setZ( 1.0f),
  B.setX(-1.0f), B.setY( 3.0f), B.setZ(-1.0f),
  C.setX( 1.0f), C.setY( 3.0f), C.setZ( 1.0f),
  D.setX( 2.0f), D.setY( 2.0f), D.setZ(-1.0f),

  E.setX(-1.5f), E.setY( 1.0f), E.setZ( 1.0f),
  F.setX(-0.5f), F.setY( 1.5f), F.setZ(-1.0f),
  G.setX( 1.5f), G.setY( 1.5f), G.setZ( 1.0f),
  H.setX( 2.5f), H.setY( 1.0f), H.setZ(-1.0f),

  I.setX(-2.5f), I.setY(-1.0f), I.setZ( 1.0f),
  J.setX(-1.5f), J.setY(-0.5f), J.setZ(-1.0f),
  K.setX( 0.5f), K.setY(-0.5f), K.setZ( 1.0f),
  L.setX( 1.5f), L.setY(-1.0f), L.setZ(-1.0f),

  M.setX(-2.0f), M.setY(-2.0f), M.setZ( 1.0f),
  N.setX(-1.0f), N.setY(-1.0f), N.setZ(-1.0f),
  O.setX( 1.0f), O.setY(-1.0f), O.setZ( 1.0f),
  P.setX( 2.0f), P.setY(-2.0f), P.setZ(-1.0f);

  detail = 20;
  change = 1.0 / (double)detail;

  double a = 1.0f,
         b = 1.0f - a,
         c = 1.0f,
         d = 1.0f - c;

  vertices.resize((detail + 1) * (detail + 1));

  for(int V = 0; V <= detail; ++V) {
    for(int U = 0; U <= detail; ++U) {
      vertices[(U * detail) + V].setX(
        A.x()*a*a*a*c*c*c   + B.x()*3*a*a*a*c*c*d +
        C.x()*3*a*a*a*c*d*d + D.x()*a*a*a*d*d*d   +
        E.x()*3*a*a*b*c*c*c + F.x()*9*a*a*b*c*c*d +
        G.x()*9*a*a*b*c*d*d + H.x()*3*a*a*b*d*d*d +
        I.x()*3*a*b*b*c*c*c + J.x()*9*a*b*b*c*c*d +
        K.x()*9*a*b*b*c*d*d + L.x()*3*a*b*b*d*d*d +
        M.x()*b*b*b*c*c*c   + N.x()*3*b*b*b*c*c*d +
        O.x()*3*b*b*b*c*d*d + P.x()*b*b*b*d*d*d);

      vertices[(U * detail) + V].setY(
        A.y()*a*a*a*c*c*c   + B.y()*3*a*a*a*c*c*d +
        C.y()*3*a*a*a*c*d*d + D.y()*a*a*a*d*d*d   +
        E.y()*3*a*a*b*c*c*c + F.y()*9*a*a*b*c*c*d +
        G.y()*9*a*a*b*c*d*d + H.y()*3*a*a*b*d*d*d +
        I.y()*3*a*b*b*c*c*c + J.y()*9*a*b*b*c*c*d +
        K.y()*9*a*b*b*c*d*d + L.y()*3*a*b*b*d*d*d +
        M.y()*b*b*b*c*c*c   + N.y()*3*b*b*b*c*c*d +
        O.y()*3*b*b*b*c*d*d + P.y()*b*b*b*d*d*d);

      vertices[(U * detail) + V].setZ(
        A.z()*a*a*a*c*c*c   + B.z()*3*a*a*a*c*c*d +
        C.z()*3*a*a*a*c*d*d + D.z()*a*a*a*d*d*d   +
        E.z()*3*a*a*b*c*c*c + F.z()*9*a*a*b*c*c*d +
        G.z()*9*a*a*b*c*d*d + H.z()*3*a*a*b*d*d*d +
        I.z()*3*a*b*b*c*c*c + J.z()*9*a*b*b*c*c*d +
        K.z()*9*a*b*b*c*d*d + L.z()*3*a*b*b*d*d*d +
        M.z()*b*b*b*c*c*c   + N.z()*3*b*b*b*c*c*d +
        O.z()*3*b*b*b*c*d*d + P.z()*b*b*b*d*d*d);
      qDebug() << (U + (detail - 1)) + V;
      c -= change;
      d  = 1.0f - c;
    }

    a -= change;
    b  = 1.0f - a;
    c = 1.0;
    d = 1.0f - c;
  }
}
void GlWidget::initializeGL()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  qglClearColor(QColor(Qt::lightGray));

  if (!prepareShaderProgram( ":/vertex_shader.vert", ":/fragment_shader.frag")) {
    return;
  }
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
  cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

  viewMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

  shaderProgram.bind();

  shaderProgram.setUniformValue("mvpMatrix", projectionMatrix *
                                             viewMatrix *
                                             modelMatrix);

  shaderProgram.setUniformValue     ("color",  QColor(Qt::red));
  shaderProgram.setAttributeArray   ("vertex", vertices.constData());
  shaderProgram.enableAttributeArray("vertex");

  glPointSize(2.0f);
  glDrawArrays(GL_POINTS, 0, vertices.size());

  shaderProgram.disableAttributeArray("vertex");
  shaderProgram.release();
}

void GlWidget::mousePressEvent(QMouseEvent *mpe) {
  lastMousePosition = mpe->pos();
  mpe->accept();
}

void GlWidget::closeEvent(QCloseEvent *ce) {
  if(ce != NULL) {
    slider->close();
  }
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

void GlWidget::updateDetailLevel(int _detail) {
  vertices.clear();

  detail = _detail;
  change = 1.0 / (double)detail;

  double a = 1.0f,
         b = 1.0f - a,
         c = 1.0f,
         d = 1.0f - c;

  vertices.resize((detail + 1) * (detail + 1));

  for(int V = 0; V <= detail; ++V) {
    for(int U = 0; U <= detail; ++U) {
      vertices[(U * detail) + V].setX(
        A.x()*a*a*a*c*c*c   + B.x()*3*a*a*a*c*c*d +
        C.x()*3*a*a*a*c*d*d + D.x()*a*a*a*d*d*d   +
        E.x()*3*a*a*b*c*c*c + F.x()*9*a*a*b*c*c*d +
        G.x()*9*a*a*b*c*d*d + H.x()*3*a*a*b*d*d*d +
        I.x()*3*a*b*b*c*c*c + J.x()*9*a*b*b*c*c*d +
        K.x()*9*a*b*b*c*d*d + L.x()*3*a*b*b*d*d*d +
        M.x()*b*b*b*c*c*c   + N.x()*3*b*b*b*c*c*d +
        O.x()*3*b*b*b*c*d*d + P.x()*b*b*b*d*d*d);

      vertices[(U * detail) + V].setY(
        A.y()*a*a*a*c*c*c   + B.y()*3*a*a*a*c*c*d +
        C.y()*3*a*a*a*c*d*d + D.y()*a*a*a*d*d*d   +
        E.y()*3*a*a*b*c*c*c + F.y()*9*a*a*b*c*c*d +
        G.y()*9*a*a*b*c*d*d + H.y()*3*a*a*b*d*d*d +
        I.y()*3*a*b*b*c*c*c + J.y()*9*a*b*b*c*c*d +
        K.y()*9*a*b*b*c*d*d + L.y()*3*a*b*b*d*d*d +
        M.y()*b*b*b*c*c*c   + N.y()*3*b*b*b*c*c*d +
        O.y()*3*b*b*b*c*d*d + P.y()*b*b*b*d*d*d);

      vertices[(U * detail) + V].setZ(
        A.z()*a*a*a*c*c*c   + B.z()*3*a*a*a*c*c*d +
        C.z()*3*a*a*a*c*d*d + D.z()*a*a*a*d*d*d   +
        E.z()*3*a*a*b*c*c*c + F.z()*9*a*a*b*c*c*d +
        G.z()*9*a*a*b*c*d*d + H.z()*3*a*a*b*d*d*d +
        I.z()*3*a*b*b*c*c*c + J.z()*9*a*b*b*c*c*d +
        K.z()*9*a*b*b*c*d*d + L.z()*3*a*b*b*d*d*d +
        M.z()*b*b*b*c*c*c   + N.z()*3*b*b*b*c*c*d +
        O.z()*3*b*b*b*c*d*d + P.z()*b*b*b*d*d*d);
      qDebug() << (U + (detail - 1)) + V;
      c -= change;
      d  = 1.0f - c;
    }
    a -= change;
    b  = 1.0f - a;
    c = 1.0;
    d = 1.0f - c;
  }

  updateGL();
}
