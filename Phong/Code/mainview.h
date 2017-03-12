#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MainView(QWidget *parent = 0);
    ~MainView();

    void updateRotation(int x, int y, int z);
    void updateModel(QString name);
    void updateShader(QString name);
    void updateScale(float scale);

    /* Add your public members below */
    QVector<QVector3D> vertices;
    QVector3D rotation;
    float sc=1;
    float startingXMouse; //The start position of x
    float startingYMouse; //The start position of y
    /* End of public members */

    QVector3D convertHSLtoRGB(float H, float S, float L);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private:
    QOpenGLDebugLogger *debugLogger;

    void createShaderPrograms();
    void createBuffers();
    void updateBuffers();
    void updateUniforms();

    // Raytracer scene functions
    void renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos);
    void renderRaytracerScene();

    /* Add your private members below */
    void loadModel(QString filename, GLuint bufferObject);

    GLuint sphereBO;
    GLuint VAO;
    GLuint normalBO;

    QMatrix3x3 normal;

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    // Shader programs, GLint for uniforms/buffer objects, other variables
    QOpenGLShaderProgram *mainShaderProg;

    GLint uniformModel;
    GLint uniformView;
    GLint uniformProjection;
    GLint uniformNormal;
    GLint uniformMaterialColor;
    GLint uniformIntensity;
    GLint uniformLightpos;
    GLint uniformPosition;
    GLint uniformCamara;

    QTimer timer; // timer used for animation

    Model *sphereModel;



    /* End of private members */

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
