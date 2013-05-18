#include <QMouseEvent>
#include <QWheelEvent>

#include "glwidget.h"

#ifdef WIN32
#include <glext.h>
  PFNGLACTIVETEXTUREPROC pGlActiveTexture = NULL;
#define glActiveTexture pGlActiveTexture
#endif

GlWidget::GlWidget(const QGLFormat& format, QWidget *parent) :
  QGLWidget(format, parent),
  alpha    (25.0f),
  beta     (25.0f),
  distance  (2.5f),
  lightAngle(0.0f) {

  groupBox = new QGroupBox(tr(""));

  radioLambert = new QRadioButton(tr("&Lambert"));
  radioPhong   = new QRadioButton(tr("&Phong"));
  radioToon    = new QRadioButton(tr("&Toon"));
  radioSs      = new QRadioButton(tr("&Subsurface scattering"));

  QTimer *timer     = new QTimer(this);
  QVBoxLayout *vbox = new QVBoxLayout();

  radioLambert->setChecked(true);

  vbox->addWidget (radioLambert);
  vbox->addWidget (radioPhong);
  vbox->addWidget (radioToon);
  vbox->addWidget (radioSs);
  vbox->addStretch(1);

  groupBox->setLayout       (vbox);
  groupBox->setWindowOpacity(0.75f);
  groupBox->setWindowFlags  (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
  groupBox->move            (pos().x() + 9, pos().y() + 28);
  groupBox->setFixedSize    (100, 100);
  groupBox->show();

  connect(radioLambert, SIGNAL(clicked()), this, SLOT(updateGL()));
  connect(radioPhong,   SIGNAL(clicked()), this, SLOT(updateGL()));
  connect(radioToon,    SIGNAL(clicked()), this, SLOT(updateGL()));
  connect(radioSs,      SIGNAL(clicked()), this, SLOT(updateGL()));

  connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

  timer->start(20);
}


void GlWidget::initializeGL() {
#ifdef WIN32
  glActiveTexture =
    (PFNGLACTIVETEXTUREPROC) wglGetProcAddress((LPCSTR) "glActiveTexture");
#endif

  glEnable   (GL_DEPTH_TEST);
  glEnable   (GL_CULL_FACE);
  glEnable   (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  qglClearColor(QColor(Qt::lightGray));

  if (!prepareShaderProgram(":/vertex_shader.vert",
                            ":/fragment_shader.frag",
                            &shaderProgram) ||
      !prepareShaderProgram(":/lighting_vertex_shader.vert",
                            ":/lighting_fragment_shader.frag",
                            &lightingShaderProgram) ||
      !prepareShaderProgram(":/lighting_lambert_vertex_shader.vert",
                            ":/lighting_lambert_fragment_shader.frag",
                            &lightingLamberShaderProgram) ||
      !prepareShaderProgram(":/lighting_toon_vertex_shader.vert",
                            ":/lighting_toon_fragment_shader.frag",
                            &lightingToonShaderProgram) ||
      !prepareShaderProgram(":/lighting_ss_vertex_shader.vert",
                            ":/lighting_ss_fragment_shader.frag",
                            &lightingSsShaderProgram)) {
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

  normals
    << QVector3D( 0.0f,  0.0f,  1.0f) << QVector3D( 0.0f,  0.0f,  1.0f) << QVector3D( 0.0f,  0.0f,  1.0f) // Front
    << QVector3D( 0.0f,  0.0f,  1.0f) << QVector3D( 0.0f,  0.0f,  1.0f) << QVector3D( 0.0f,  0.0f,  1.0f)
    << QVector3D( 0.0f,  0.0f, -1.0f) << QVector3D( 0.0f,  0.0f, -1.0f) << QVector3D( 0.0f,  0.0f, -1.0f) // Back
    << QVector3D( 0.0f,  0.0f, -1.0f) << QVector3D( 0.0f,  0.0f, -1.0f) << QVector3D( 0.0f,  0.0f, -1.0f)
    << QVector3D(-1.0f,  0.0f,  0.0f) << QVector3D(-1.0f,  0.0f,  0.0f) << QVector3D(-1.0f,  0.0f,  0.0f) // Left
    << QVector3D(-1.0f,  0.0f,  0.0f) << QVector3D(-1.0f,  0.0f,  0.0f) << QVector3D(-1.0f,  0.0f,  0.0f)
    << QVector3D( 1.0f,  0.0f,  0.0f) << QVector3D( 1.0f,  0.0f,  0.0f) << QVector3D( 1.0f,  0.0f,  0.0f) // Right
    << QVector3D( 1.0f,  0.0f,  0.0f) << QVector3D( 1.0f,  0.0f,  0.0f) << QVector3D( 1.0f,  0.0f,  0.0f)
    << QVector3D( 0.0f,  1.0f,  0.0f) << QVector3D( 0.0f,  1.0f,  0.0f) << QVector3D( 0.0f,  1.0f,  0.0f) // Top
    << QVector3D( 0.0f,  1.0f,  0.0f) << QVector3D( 0.0f,  1.0f,  0.0f) << QVector3D( 0.0f,  1.0f,  0.0f)
    << QVector3D( 0.0f, -1.0f,  0.0f) << QVector3D( 0.0f, -1.0f,  0.0f) << QVector3D( 0.0f, -1.0f,  0.0f) // Bottom
    << QVector3D( 0.0f, -1.0f,  0.0f) << QVector3D( 0.0f, -1.0f,  0.0f) << QVector3D( 0.0f, -1.0f,  0.0f);

  textureCoordinates
    << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) // Front
    << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
    << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) // Back
    << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
    << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) // Left
    << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
    << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) // Right
    << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
    << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) // Top
    << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
    << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) // Bottom
    << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f);

  texture = bindTexture(QPixmap(":/texture.png"));

  spotlightVertices
    << QVector3D( 0.0f, 1.0f,  0.0f) << QVector3D(-0.5f, 0.0f,  0.5f) << QVector3D( 0.5f, 0.0f,  0.5f) // Front
    << QVector3D( 0.0f, 1.0f,  0.0f) << QVector3D( 0.5f, 0.0f, -0.5f) << QVector3D(-0.5f, 0.0f, -0.5f) // Back
    << QVector3D( 0.0f, 1.0f,  0.0f) << QVector3D(-0.5f, 0.0f, -0.5f) << QVector3D(-0.5f, 0.0f,  0.5f) // Left
    << QVector3D( 0.0f, 1.0f,  0.0f) << QVector3D( 0.5f, 0.0f,  0.5f) << QVector3D( 0.5f, 0.0f, -0.5f) // Right
    << QVector3D(-0.5f, 0.0f, -0.5f) << QVector3D( 0.5f, 0.0f, -0.5f) << QVector3D( 0.5f, 0.0f,  0.5f) // Bottom
    << QVector3D( 0.5f, 0.0f,  0.5f) << QVector3D(-0.5f, 0.0f,  0.5f) << QVector3D(-0.5f, 0.0f, -0.5f);

  spotlightColors
    << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) // Front
    << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) // Back
    << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) // Left
    << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) << QVector3D(0.2f, 0.2f, 0.2f) // Right
    << QVector3D(1.0f, 1.0f, 1.0f) << QVector3D(1.0f, 1.0f, 1.0f) << QVector3D(1.0f, 1.0f, 1.0f) // Bottom
    << QVector3D(1.0f, 1.0f, 1.0f) << QVector3D(1.0f, 1.0f, 1.0f) << QVector3D(1.0f, 1.0f, 1.0f);
}

bool GlWidget::prepareShaderProgram(const QString& vertexShaderPath,
                                    const QString& fragmentShaderPath,
                                    QGLShaderProgram* loadingShaderProgram) {
  bool result = loadingShaderProgram->addShaderFromSourceFile(QGLShader::Vertex,
                                                              vertexShaderPath);

  if (!result) {
    qWarning() << loadingShaderProgram->log();
  }

  result = loadingShaderProgram->addShaderFromSourceFile(QGLShader::Fragment,
                                                         fragmentShaderPath );
  if (!result) {
    qWarning() << loadingShaderProgram->log();
  }

  result = loadingShaderProgram->link();
  if (!result) {
    qWarning() << "Could not link shader program:"
               << loadingShaderProgram->log();
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
             modelViewMatrix,
             cameraTransformation,
             lightTransformation;

  cameraTransformation.rotate(alpha, 0, 1, 0);
  cameraTransformation.rotate(beta,  1, 0, 0);

  cameraPosition    = cameraTransformation * QVector3D(0.0f, 0.0f, distance);
  cameraUpDirection = cameraTransformation * QVector3D(0.0f, 1.0f, 0.0f);

  viewMatrix.lookAt(cameraPosition,
                    QVector3D(0.0f, 0.0f, 0.0f),
                    cameraUpDirection);

  modelMatrix.setToIdentity();

  modelViewMatrix = viewMatrix * modelMatrix;
  normalMatrix    = modelViewMatrix.normalMatrix();

  lightTransformation.rotate(lightAngle, 1.0f, 1.0f, 0.0f);

  lightPosition = lightTransformation * QVector3D(0.0f, 1.0f, 1.0f);


  if (radioLambert->isChecked()) {
    lightingLamberShaderProgram.bind();
    lightingLamberShaderProgram.setUniformValue("mvpMatrix",      projectionMatrix *
                                                  modelViewMatrix);
    lightingLamberShaderProgram.setUniformValue("mvMatrix",       modelViewMatrix);
    lightingLamberShaderProgram.setUniformValue("normalMatrix",   normalMatrix);
    lightingLamberShaderProgram.setUniformValue("lightPosition",  viewMatrix *
                                                lightPosition);
    lightingLamberShaderProgram.setUniformValue("cameraPosition", cameraPosition);
    lightingLamberShaderProgram.setUniformValue("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture  (GL_TEXTURE_2D, texture);
    glActiveTexture(0);

    lightingLamberShaderProgram.setAttributeArray   ("vertex", vertices.constData());
    lightingLamberShaderProgram.enableAttributeArray("vertex");

    lightingLamberShaderProgram.setAttributeArray   ("normal", normals.constData());
    lightingLamberShaderProgram.enableAttributeArray("normal");

    lightingLamberShaderProgram.setAttributeArray   ("textureCoordinate",
                                                     textureCoordinates.constData());
    lightingLamberShaderProgram.enableAttributeArray("textureCoordinate");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    lightingLamberShaderProgram.disableAttributeArray("vertex");
    lightingLamberShaderProgram.disableAttributeArray("normal");
    lightingLamberShaderProgram.disableAttributeArray("textureCoordinate");

    lightingLamberShaderProgram.release();
  } else if (radioPhong->isChecked()) {
    lightingShaderProgram.bind();

    lightingShaderProgram.setUniformValue("mvpMatrix",     projectionMatrix *
                                                           modelViewMatrix);
    lightingShaderProgram.setUniformValue("mvMatrix",      modelViewMatrix);
    lightingShaderProgram.setUniformValue("normalMatrix",  normalMatrix);
    lightingShaderProgram.setUniformValue("lightPosition", viewMatrix * lightPosition);

    lightingShaderProgram.setUniformValue("ambientColor",  QColor( 32, 0, 0));
    lightingShaderProgram.setUniformValue("diffuseColor",  QColor(128, 0, 0));
    lightingShaderProgram.setUniformValue("specularColor", QColor(255, 0, 0));

    lightingShaderProgram.setUniformValue("ambientReflection",  (GLfloat)  1.00f);
    lightingShaderProgram.setUniformValue("diffuseReflection",  (GLfloat)  0.80f);
    lightingShaderProgram.setUniformValue("specularReflection", (GLfloat)  0.75f);
    lightingShaderProgram.setUniformValue("shininess",          (GLfloat) 90.00f);

    lightingShaderProgram.setUniformValue("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture  (GL_TEXTURE_2D, texture);
    glActiveTexture(0);

    lightingShaderProgram.setAttributeArray   ("vertex", vertices.constData());
    lightingShaderProgram.enableAttributeArray("vertex");

    lightingShaderProgram.setAttributeArray   ("normal", normals.constData());
    lightingShaderProgram.enableAttributeArray("normal");

    lightingShaderProgram.setAttributeArray   ("textureCoordinate",
                                               textureCoordinates.constData());
    lightingShaderProgram.enableAttributeArray("textureCoordinate");

    shaderProgram.setUniformValue("mvpMatrix", projectionMatrix *
                                  viewMatrix *
                                  modelMatrix);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    lightingShaderProgram.disableAttributeArray("vertex");
    lightingShaderProgram.disableAttributeArray("normal");
    lightingShaderProgram.disableAttributeArray("textureCoordinate");

    lightingShaderProgram.release();
  } else if (radioToon->isChecked()) {
    glDisable(GL_BLEND);
    lightingToonShaderProgram.bind();
    lightingToonShaderProgram.setUniformValue("mvpMatrix",      projectionMatrix *
                                                                modelViewMatrix);
    lightingToonShaderProgram.setUniformValue("mvMatrix",       modelViewMatrix);
    lightingToonShaderProgram.setUniformValue("normalMatrix",   normalMatrix);
    lightingToonShaderProgram.setUniformValue("lightPosition",  viewMatrix *
                                                                lightPosition);
    lightingToonShaderProgram.setUniformValue("cameraPosition", cameraPosition);

    lightingToonShaderProgram.setUniformValue("diffuseColor", QColor(0, 128, 0));
    lightingToonShaderProgram.setUniformValue("diffusePower", (GLfloat) 0.75f);
    
    lightingToonShaderProgram.setUniformValue("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture  (GL_TEXTURE_2D, texture);
    glActiveTexture(0);

    lightingToonShaderProgram.setAttributeArray   ("vertex", vertices.constData());
    lightingToonShaderProgram.enableAttributeArray("vertex");

    lightingToonShaderProgram.setAttributeArray   ("normal", normals.constData());
    lightingToonShaderProgram.enableAttributeArray("normal");

    lightingToonShaderProgram.setAttributeArray   ("textureCoordinate",
                                                     textureCoordinates.constData());
    lightingToonShaderProgram.enableAttributeArray("textureCoordinate");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    lightingToonShaderProgram.disableAttributeArray("vertex");
    lightingToonShaderProgram.disableAttributeArray("normal");
    lightingToonShaderProgram.disableAttributeArray("textureCoordinate");

    lightingToonShaderProgram.release();
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else if (radioSs->isChecked()) {
    lightingSsShaderProgram.bind();
    lightingSsShaderProgram.setUniformValue("mvpMatrix",      projectionMatrix *
                                                              modelViewMatrix);
    lightingSsShaderProgram.setUniformValue("mvMatrix",       modelViewMatrix);
    lightingSsShaderProgram.setUniformValue("normalMatrix",   normalMatrix);
    lightingSsShaderProgram.setUniformValue("lightPosition",  viewMatrix *
                                                              lightPosition);
    lightingSsShaderProgram.setUniformValue("cameraPosition", cameraPosition);

    lightingSsShaderProgram.setUniformValue("lightColor",    QColor(0, 0, 255));
    lightingSsShaderProgram.setUniformValue("baseColor",     QColor(0, 0, 128));
    lightingSsShaderProgram.setUniformValue("specularColor", QColor(0, 0, 196));

    lightingSsShaderProgram.setUniformValue("extinction", QVector3D(0.25f, 0.5f, 0.0f));

    lightingSsShaderProgram.setUniformValue("materialThickness", (GLfloat) 0.5f);
    lightingSsShaderProgram.setUniformValue("specularPower",     (GLfloat) 0.75f);
    lightingSsShaderProgram.setUniformValue("rimScalar",         (GLfloat) 0.5f);

    lightingSsShaderProgram.setUniformValue("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture  (GL_TEXTURE_2D, texture);
    glActiveTexture(0);

    lightingSsShaderProgram.setAttributeArray   ("vertex", vertices.constData());
    lightingSsShaderProgram.enableAttributeArray("vertex");

    lightingSsShaderProgram.setAttributeArray   ("normal", normals.constData());
    lightingSsShaderProgram.enableAttributeArray("normal");

    lightingSsShaderProgram.setAttributeArray   ("textureCoordinate",
                                                     textureCoordinates.constData());
    lightingSsShaderProgram.enableAttributeArray("textureCoordinate");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    lightingSsShaderProgram.disableAttributeArray("vertex");
    lightingSsShaderProgram.disableAttributeArray("normal");
    lightingSsShaderProgram.disableAttributeArray("textureCoordinate");

    lightingSsShaderProgram.release();
  } 

  modelMatrix.setToIdentity();
  modelMatrix.translate    (lightPosition);
  modelMatrix.rotate       (lightAngle, 0.0f, 1.0f, 0.0f);
  modelMatrix.rotate       (45.0f, 1.0f, 0.0f, 0.0f);
  modelMatrix.scale        (0.1f);

  shaderProgram.bind();

  shaderProgram.setUniformValue     ("mvpMatrix", projectionMatrix *
                                                  viewMatrix *
                                                  modelMatrix);

  shaderProgram.setAttributeArray   ("vertex", spotlightVertices.constData());
  shaderProgram.enableAttributeArray("vertex");

  shaderProgram.setAttributeArray   ("color", spotlightColors.constData());
  shaderProgram.enableAttributeArray("color");

  glDrawArrays(GL_TRIANGLES, 0, spotlightVertices.size());

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

void GlWidget::timeout() {
  lightAngle += 1;

  while (lightAngle >= 360) {
    lightAngle -= 360;
  }

  updateGL();
}

void GlWidget::closeEvent(QCloseEvent *ce) {
  if(ce != NULL) {
    groupBox->close();
  }
}
