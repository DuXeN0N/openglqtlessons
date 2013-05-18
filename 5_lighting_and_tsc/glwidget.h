#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTimer>

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
                            const QString& fragmentShaderPath,
                            QGLShaderProgram* loadingShaderProgram);

  // Control by mouse
  void mousePressEvent(QMouseEvent *mpe);
  void mouseMoveEvent (QMouseEvent *mme);
  void wheelEvent     (QWheelEvent *we);
  void closeEvent     (QCloseEvent *ce);

private:
  double alpha,
         beta,
         distance;

  QPoint lastMousePosition;

  QVector3D lightPosition,
            cameraPosition,
            cameraUpDirection;

  QMatrix4x4 projectionMatrix;
  QMatrix3x3 normalMatrix;

  QGLShaderProgram   shaderProgram;
  QVector<QVector3D> vertices,
                     normals;

  QVector<QVector2D> textureCoordinates;
  GLuint             texture;

  QGLShaderProgram lightingShaderProgram,
                   lightingLamberShaderProgram,
                   lightingToonShaderProgram,
                   lightingSsShaderProgram;  // Subsurface scattering

  QVector<QVector3D> spotlightVertices,
                     spotlightColors;
  double lightAngle;

  QGroupBox    *groupBox;
  QRadioButton *radioLambert,
               *radioPhong,
               *radioToon,
               *radioSs;

private Q_SLOTS:
  void timeout();
};

#endif // GLWIDGET_H
