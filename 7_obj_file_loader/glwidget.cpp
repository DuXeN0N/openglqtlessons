#include <QMouseEvent>
#include <QWheelEvent>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMenu>
#include <QFileDialog>

#include "glwidget.h"

GlWidget::GlWidget(const QGLFormat& format, QWidget *parent) :
  QGLWidget(format, parent), OBJfilename("teapot.obj"),
  alpha  (25.0f),
  beta   (25.0f),
  distance(2.5f) {
  LoadOBJModel = new QAction("Load OBJ model", this);
  LoadOBJModel->setShortcut(QKeySequence::Open);
  LoadOBJModel->setStatusTip(tr("Load new model from *.obj file"));

  connect(LoadOBJModel, SIGNAL(triggered()), this, SLOT(LoadOBJFromFile()));
  connect(LoadOBJModel, SIGNAL(triggered()), this, SLOT(updateGL()));
}

void GlWidget::initializeGL()
{
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);

  qglClearColor(QColor(Qt::lightGray));

  if (!prepareShaderProgram( ":/vertex_shader.vert", ":/fragment_shader.frag")) {
    return;
  }
}

void GlWidget::LoadOBJFromFile() {
  QString filename = QFileDialog::getOpenFileName(this, tr( "Choose a obj file to load" ),
                                                  "/", tr( "Obj Documents (*.obj)" ));
  if (filename.isNull()) {
    return;
  }

  QFile *OBJFile = new QFile(filename);

  if (!modelVertices.isEmpty()) {
    modelVertices.clear();
  }
  if (!modelNormals.isEmpty()) {
    modelNormals.clear();
  }
  if (!modelElements.isEmpty()) {
    modelElements.clear();
  }

  if (OBJFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(OBJFile);
    QString line, header;
    qreal tempVariableForStream;

    while (!stream.atEnd()) {
      stream >> header;

      if ("v" == header) {
        QVector3D v;
        stream >> tempVariableForStream; v.setX(tempVariableForStream);
        stream >> tempVariableForStream; v.setY(tempVariableForStream);
        stream >> tempVariableForStream; v.setZ(tempVariableForStream);

        modelVertices.push_back(v);  // TODO: Think about reallocation!

      } else if ("f" == header) {
        GLushort a,b,c;
        stream >> a; a--;
        stream >> b; b--;
        stream >> c; c--;

        modelElements.push_back(a);  // TODO: Think about reallocation!
        modelElements.push_back(b);  // TODO: Think about reallocation!
        modelElements.push_back(c);  // TODO: Think about reallocation!

      } else if ("vn" == header) {
        QVector3D n;
        stream >> tempVariableForStream; n.setX(tempVariableForStream);
        stream >> tempVariableForStream; n.setY(tempVariableForStream);
        stream >> tempVariableForStream; n.setZ(tempVariableForStream);

        modelNormals.push_back(n);  // TODO: Think about reallocation!
      } else if ("#"  == line.at(0)) {
        // It's comments, maybe use this stuff leater
      } else {
        // Do nothing
      }
    }

    OBJFile->close();
  } else {
      QMessageBox *msgBox = new QMessageBox();
      msgBox->setText(OBJFile->error() + " in " + filename);
      msgBox->setIcon(QMessageBox::Warning);
      msgBox->setStandardButtons(QMessageBox::Ok);
      msgBox->show();
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
  glClear    (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable   (GL_CULL_FACE);
  glEnable   (GL_BLEND);
  glEnable   (GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

  shaderProgram.setAttributeArray   ("vertex", modelVertices.constData());
  shaderProgram.enableAttributeArray("vertex");

  shaderProgram.setUniformValue     ("color", QVector4D(0.25f, 0.25f, 0.25f, 0.75f));

  glDrawElements(GL_TRIANGLES, modelElements.size(), GL_UNSIGNED_SHORT, modelElements.constData());

  shaderProgram.setUniformValue     ("color",  QColor(Qt::red));

  glDrawElements(GL_LINES, modelElements.size(), GL_UNSIGNED_SHORT, modelElements.constData());

  shaderProgram.disableAttributeArray("vertex");
  shaderProgram.release();

  glDisable(GL_BLEND);
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

void GlWidget::contextMenuEvent(QContextMenuEvent *cme) {
  QMenu menu(this);
  menu.addAction(LoadOBJModel);
  menu.exec(cme->globalPos());
}

