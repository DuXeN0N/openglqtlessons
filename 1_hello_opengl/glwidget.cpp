#include "glwidget.h"
#include <QGridLayout>
#include <QPushButton>

GlWidget::GlWidget(const QGLFormat& format, QWidget *parent) :
    QGLWidget(format, parent),
    triangleColor(Qt::red),
    A( 1.0f, 0.0f, -2.0f),
    B( 0.0f, 1.0f, -2.0f),
    C(-1.0f, 0.0f, -2.0f) {
  CreateWidgets();
}

void GlWidget::CreateWidgets() {
  settings    = new QWidget;
  colorDialog = new QColorDialog();

  QPushButton *buttonColor  = new QPushButton("&Change color");
  QPushButton *buttonUpdate = new QPushButton("&Update");
  QGridLayout *layout       = new QGridLayout;

  axEdit = new QLineEdit("1.0");
  bxEdit = new QLineEdit("0.0");
  cxEdit = new QLineEdit("-1.0");
  ayEdit = new QLineEdit("0.0");
  byEdit = new QLineEdit("1.0");
  cyEdit = new QLineEdit("0.0");

  layout->addWidget(axEdit, 0, 0);
  layout->addWidget(ayEdit, 0, 1);
  layout->addWidget(bxEdit, 1, 0);
  layout->addWidget(byEdit, 1, 1);
  layout->addWidget(cxEdit, 2, 0);
  layout->addWidget(cyEdit, 2, 1);
  layout->addWidget(buttonColor, 3, 0);
  layout->addWidget(buttonUpdate, 3, 1);

  settings->setLayout       (layout);
  settings->setWindowFlags  (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
  settings->setWindowOpacity(0.75f);
  settings->setFixedSize    (180, 160);
  settings->move            (x() + 9, y() + 29);
  settings->show();

  connect(colorDialog,  SIGNAL(currentColorChanged(QColor)), this, SLOT(changeColor(QColor)));
  connect(buttonColor,  SIGNAL(clicked()),                   this, SLOT(showColorDialog()));
  connect(buttonUpdate, SIGNAL(clicked()),                   this, SLOT(updateCoordinates()));
  connect(buttonUpdate, SIGNAL(clicked()),                   this, SLOT(updateGL()));
}

void GlWidget::showColorDialog() {
  colorDialog->show();
}

void GlWidget::changeColor(QColor newColor) {
  triangleColor = newColor;
}

void GlWidget::updateCoordinates() {
    vertices[0].setX(axEdit->text().toFloat());
    vertices[0].setY(ayEdit->text().toFloat());
    vertices[1].setX(bxEdit->text().toFloat());
    vertices[1].setY(byEdit->text().toFloat());
    vertices[2].setX(cxEdit->text().toFloat());
    vertices[2].setY(cyEdit->text().toFloat());

  qDebug() << vertices;
}
void GlWidget::initializeGL()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  qglClearColor(QColor(Qt::lightGray));

  if (!prepareShaderProgram( ":/vertex_shader.vert", ":/fragment_shader.frag")) {
    return;
  }

  vertices << A << B << C;
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
                               0.001f,
                               1000);

  glViewport(0, 0, width, height);
}

void GlWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 modelMatrix, viewMatrix;

  shaderProgram.bind();

  shaderProgram.setUniformValue("mvpMatrix", projectionMatrix *
                                             viewMatrix *
                                             modelMatrix);

  shaderProgram.setUniformValue     ("color",  triangleColor);
  shaderProgram.setAttributeArray   ("vertex", vertices.constData());
  shaderProgram.enableAttributeArray("vertex");

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  shaderProgram.disableAttributeArray("vertex");
  shaderProgram.release();
}

void GlWidget::closeEvent(QCloseEvent *ce) {
  if(ce != NULL) {
    settings->close();

    if (colorDialog != NULL) {
      colorDialog->close();
    }
  }
}
