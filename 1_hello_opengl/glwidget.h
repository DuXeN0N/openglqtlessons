#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QColorDialog>
#include <QLineEdit>

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
  void closeEvent(QCloseEvent *ce);

private:
  QMatrix4x4         projectionMatrix;
  QGLShaderProgram   shaderProgram;

  QVector<QVector3D> vertices;
  QVector3D          A, B, C;
  QColor             triangleColor;

  QWidget      *settings;
  QColorDialog *colorDialog;
  QLineEdit    *axEdit, *ayEdit,
               *bxEdit, *byEdit,
               *cxEdit, *cyEdit;

  void CreateWidgets();

private slots:
  void showColorDialog();
  void changeColor(QColor newColor);
  void updateCoordinates();
};

#endif // GLWIDGET_H
