#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QSlider>
#include <QVector3D>

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

  int    detail;
  double change;

  QVector3D A, B, C, D,
            E, F, G, H,
            I, J, K, L,
            M, N, O, P;

  void initSurface();

  QSlider *slider;

private slots:
  void updateDetailLevel(int _detail);
};

#endif // GLWIDGET_H
