/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

// Add new headers for mouse events!
#include <QMouseEvent>
#include <QWheelEvent>

#include "glwidget.h"

//if WIN32
#ifdef WIN32
  #include <GL/glext.h>
  PFNGLACTIVETEXTUREPROC pGlActiveTexture = NULL;
  #define glActiveTexture pGlActiveTexture
#endif

void GlWidget::initializeGL()
{
   #ifdef WIN32
    glActiveTexture =
      (PFNGLACTIVETEXTUREPROC) wglGetProcAddress((LPCSTR) "glActiveTexture");
   #endif

  glEnable   (GL_DEPTH_TEST);
  glEnable   (GL_CULL_FACE);
  glEnable   (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  qglClearColor(QColor(Qt::lightGray));

//  lightingShaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/lighting_vertex_shader.vert");
//  lightingShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/lighting_fragment_shader.frag");
//  lightingShaderProgram.link();

  // load and link shaders for cube
  if (!prepareShaderProgram(":/vertex_shader.vert",
                            ":/fragment_shader.frag",
                            &shaderProgram) ||
      !prepareShaderProgram(":/lighting_vertex_shader.vert",
                            ":/lighting_fragment_shader.frag",
                            &lightingShaderProgram)) {
    return;
  }

  // list of cube's vertices
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


//  shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/vertex_shader.vert");
//  shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/fragment_shader.frag");
//  shaderProgram.link();

  //lighting position and color
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
  // 1). Load and compile the vertex shader
  bool result = loadingShaderProgram->addShaderFromSourceFile(QGLShader::Vertex,
                                                              vertexShaderPath );

  if (!result) {
    qWarning() << loadingShaderProgram->log();
  }

  // 2). Load and compile the fragment shader
  result = loadingShaderProgram->addShaderFromSourceFile(QGLShader::Fragment,
                                                         fragmentShaderPath );
  if (!result) {
    qWarning() << loadingShaderProgram->log();
  }

  // 3). Finally we link them to resolve any references
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
  projectionMatrix.perspective(60.0,                           // angle
                               (float) width / (float) height, // aspect ratio
                               0.001,                          // nearPlane
                               1000);                          // farPlane

  glViewport(0, 0, width, height);  // set viewport
}

void GlWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QVector3D lightPosition;

  QMatrix3x3 normalMatrix;

  QMatrix4x4 modelMatrix,
             viewMatrix,
             modelViewMatrix,
             cameraTransformation,
             lightTransformation;

  cameraTransformation.rotate(alpha, 0, 1, 0);
  cameraTransformation.rotate(beta,  1, 0, 0);

  QVector3D cameraPosition,
            cameraUpDirection;

  cameraPosition    = cameraTransformation * QVector3D(0.0f, 0.0f, distance);
  cameraUpDirection = cameraTransformation * QVector3D(0.0f, 1.0f, 0.0f);

  viewMatrix.lookAt(cameraPosition, QVector3D(0.0f, 0.0f, 0.0f), cameraUpDirection);

  modelMatrix.setToIdentity();
  modelViewMatrix = viewMatrix * modelMatrix;
  normalMatrix    = modelViewMatrix.normalMatrix();

  lightTransformation.rotate(lightAngle, 1.0f, 1.0f, 0.0f);
  lightPosition = lightTransformation * QVector3D(0.0f, 1.0f, 1.0f);

  lightingShaderProgram.bind();

  lightingShaderProgram.setUniformValue("mvpMatrix",     projectionMatrix * modelViewMatrix);
  lightingShaderProgram.setUniformValue("mvMatrix",      modelViewMatrix);
  lightingShaderProgram.setUniformValue("normalMatrix",  normalMatrix);
  lightingShaderProgram.setUniformValue("lightPosition", viewMatrix * lightPosition);

  lightingShaderProgram.setUniformValue("ambientColor",      QColor( 32,  32,  32));
  lightingShaderProgram.setUniformValue("diffuseColor",      QColor(128, 128, 0));
  lightingShaderProgram.setUniformValue("specularColor",     QColor(255, 255, 0));
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
  lightingShaderProgram.setAttributeArray   ("textureCoordinate", textureCoordinates.constData());
  lightingShaderProgram.enableAttributeArray("textureCoordinate");

  shaderProgram.setUniformValue("mvpMatrix", projectionMatrix *
                                             viewMatrix *
                                             modelMatrix);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  lightingShaderProgram.disableAttributeArray("vertex");
  lightingShaderProgram.disableAttributeArray("normal");
  lightingShaderProgram.disableAttributeArray("textureCoordinate");

  lightingShaderProgram.release();


  modelMatrix.setToIdentity();
  modelMatrix.translate    (lightPosition);
  modelMatrix.rotate       (lightAngle, 0.0f, 1.0f, 0.0f);
  modelMatrix.rotate       (45.0f, 1.0f, 0.0f, 0.0f);
  modelMatrix.scale        (0.1f);

  shaderProgram.bind();

  shaderProgram.setUniformValue     ("mvpMatrix", projectionMatrix * viewMatrix * modelMatrix);
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

// Increase or decrease the viewers distance
// by 0.1 and update our GlWidget.
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
