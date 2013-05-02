#include <QMouseEvent>
#include <QWheelEvent>
#include <QSizePolicy>
#include "glwidget.h"

GlWidget::GlWidget(const QGLFormat& format, QWidget *parent) :
  QGLWidget(format, parent),
  alpha  (25.0f),
  beta   (25.0f),
  distance(2.5f) {

  groupBox = new QGroupBox(tr(""));

  radioPoints    = new QRadioButton(tr("&Points"));
  radioWireframe = new QRadioButton(tr("&Wireframe"));
  radioSolid     = new QRadioButton(tr("&Solid"));

  radioSolid->setChecked(true);

  QVBoxLayout *vbox = new QVBoxLayout();

  vbox->addWidget (radioPoints);
  vbox->addWidget (radioWireframe);
  vbox->addWidget (radioSolid);
  vbox->addStretch(1);

  groupBox->setLayout       (vbox);
  groupBox->setWindowOpacity(0.75f);
  groupBox->setWindowFlags  (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
  groupBox->move            (pos().x() + 9, pos().y() + 28);
  groupBox->setFixedSize    (100, 100);
  groupBox->show();
  
  connect(radioPoints,    SIGNAL(clicked()), this, SLOT(updateGL()));
  connect(radioWireframe, SIGNAL(clicked()), this, SLOT(updateGL()));
  connect(radioSolid,     SIGNAL(clicked()), this, SLOT(updateGL()));
}

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
  projectionMatrix.perspective(60.0,                           // angle
                               (float) width / (float) height, // aspect ratio
                               0.001,                          // nearPlane
                               1000);                          // farPlane

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

  // IF POINTS
  if (radioPoints->isChecked()) {
    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, vertices.size());
  }

  // IF WIREFRAME
  if (radioWireframe->isChecked()) {
   glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
  }

  // IF SOLID
  if (radioSolid->isChecked()) {
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  }

  shaderProgram.disableAttributeArray("vertex");
  shaderProgram.release();
}

void GlWidget::mousePressEvent(QMouseEvent *mpe) {
  lastMousePosition = mpe->pos();
  mpe->accept();
}

void GlWidget::closeEvent(QCloseEvent *ce) {
  if(ce != NULL) {
    groupBox->close();
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
