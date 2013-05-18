#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>

class GlWidget : public QGLWidget {
  Q_OBJECT

public:
  GlWidget(const QGLFormat& format, QWidget *parent = 0) :
    QGLWidget(format, parent),
    alpha  (25.0f),
    beta   (25.0f),
    distance(2.5f) { }

  ~GlWidget() { }

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

private:
  double alpha,
         beta,
         distance;

  QPoint     lastMousePosition;
  QMatrix4x4 projectionMatrix;

  QGLShaderProgram   shaderProgram;
  QVector<QVector3D> vertices,
                     colors;
};

#endif // GLWIDGET_H
