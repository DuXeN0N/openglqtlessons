#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QAction>

class GlWidget : public QGLWidget {
  Q_OBJECT

public:
  GlWidget(const QGLFormat& format, QWidget *parent = 0);

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

  void contextMenuEvent(QContextMenuEvent *cme);

private:
  double alpha,
         beta,
         distance;

  QPoint     lastMousePosition;
  QMatrix4x4 projectionMatrix;

  QGLShaderProgram   shaderProgram;
  QVector<QVector3D> vertices,
                     colors;

  QVector <QVector3D> modelVertices;
  QVector <QVector3D> modelNormals;
  QVector <GLushort>  modelElements;

  QString OBJfilename;

  QAction *LoadOBJModel;

private slots:
  void LoadOBJFromFile();
};

#endif // GLWIDGET_H
