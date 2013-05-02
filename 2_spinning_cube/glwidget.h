#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>

class GlWidget : public QGLWidget {
  Q_OBJECT

public:
   GlWidget(const QGLFormat& format, QWidget *parent = 0);
  ~GlWidget()
   {}

  QSize sizeHint() const {
    return QSize(800, 600);
  }

protected:
  void initializeGL();
  void resizeGL    (int width, int height);
  void paintGL     ();

  bool prepareShaderProgram(const QString& vertexShaderPath,
                            const QString& fragmentShaderPath);

  // Control by mouse
  void mousePressEvent(QMouseEvent *mpe);
  void mouseMoveEvent (QMouseEvent *mme);
  void wheelEvent     (QWheelEvent *we);
  void closeEvent     (QCloseEvent *ce);

private:
  double alpha,
         beta,
         distance;

  QPoint     lastMousePosition;
  QMatrix4x4 projectionMatrix;

  QGLShaderProgram   shaderProgram;
  QVector<QVector3D> vertices;

  QGroupBox    *groupBox;
  QRadioButton *radioPoints,
               *radioWireframe,
               *radioSolid;

};

#endif // GLWIDGET_H
