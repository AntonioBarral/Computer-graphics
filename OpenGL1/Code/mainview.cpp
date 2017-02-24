#include "mainview.h"
#include "math.h"

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    delete cubeModel;

    // Free Buffer Objects before Vertex Arrays
    glDeleteBuffers(1,&BOcoordinates);
    glDeleteBuffers(1,&BOcolors);
    glDeleteVertexArrays(1,&VAO);

    // Free the main shader
    delete mainShaderProg;

    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

/**
 * @brief MainView::createShaderPrograms
 *
 * Creates the shaderprogram for OpenGL rendering
 */
void MainView::createShaderPrograms() {
    // Qt wrapper (way cleaner than using pure OpenGL)
    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    mainShaderProg->link();

    /* Add your other shaders below */

    /* End of custom shaders */

    // Store the locations (pointers in gpu memory) of uniforms in Glint's

}

/**
 * @brief MainView::createBuffers
 *
 * Creates necessary buffers for your application
 */
void MainView::createBuffers() {
    // TODO: implement buffer creation

    //Bind the VAO to the OpenGL statemachine
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    //Generate two buffers for both the coordinates and color pointers
    glGenBuffers(1,&BOcoordinates);
    glGenBuffers(1,&BOcolors);

    //Bind the coordinates BO. The state machine should have now the VAO for the cube, and the BO for the coordinates bound
    glBindBuffer(GL_ARRAY_BUFFER,BOcoordinates);

    //Enable the coordinates attribute location from our shader
    glEnableVertexAttribArray(0);

    //Assign the location to our coordinate BO
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    //Bind the colors BO. The state machine should have now the VAO for the cube, and the BO for the colors bound
    glBindBuffer(GL_ARRAY_BUFFER,BOcolors);

    //Enable the colors attribute location from our shader
    glEnableVertexAttribArray(1);

    //Assign the location to our colors BO
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    //Unbind the VAO
    glBindVertexArray(0);

}

void MainView::loadModel(QString filename, GLuint bufferObject) {

    cubeModel = new Model(filename);
    numTris = cubeModel->getNumTriangles();

    Q_UNUSED(bufferObject);

    // TODO: implement loading of model into Buffer Objects


    //Create dynamic array of vectors to store the vertices of the model.
    vertices = cubeModel->getVertices();
    numVertices=vertices.size();

    //Random generator. Each vector coefficient represents an R, G or B  floating point value between 0 and 1.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);

    //The vertices are  grouped per 3  to create  a  triangle
    for (unsigned int i = 0; i<numTris; i++) {

      R=dis(gen);
      G=dis(gen);
      B=dis(gen);
      QVector3D color = QVector3D(R,G,B);

      colors.append(color);
      colors.append(color);
      colors.append(color);
    }

    //Bind the coordinates BO
    glBindBuffer(GL_ARRAY_BUFFER,BOcoordinates);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*vertices.size(),vertices.data(),GL_STATIC_DRAW);

    //Bind the colors BO
    glBindBuffer(GL_ARRAY_BUFFER,BOcolors);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*colors.size(),colors.data(),GL_STATIC_DRAW);

}

void MainView::updateBuffers() {
    // Change the data inside buffers (if you want)
    // make sure to change GL_STATIC_DRAW to GL_DYNAMIC_DRAW
    // in the call to glBufferData for better performance

}


void MainView::updateUniforms() {
    // TODO: update the uniforms in the shaders using the glUniform<datatype> functions

}

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    //glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* TODO: call your initialization functions here */

    createShaderPrograms();

    createBuffers();

    loadModel(":/models/cube.obj", cubeBO);

    // For animation, you can start your timer here

}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {

    // TODO: Update projection to fit the new aspect ratio
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {

    // Clear the screen before rendering
    glClearColor(0.0f,0.0f,0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShaderProg->bind();

    // TODO: implement your drawing functions

    //Bind the VAO after the shader is bound
    glBindVertexArray(VAO);

    //Call drawing function
    glDrawArrays(GL_TRIANGLES,0,numTris*3);

    //Set the matrices as identity matrices
    model.setToIdentity();
    view.setToIdentity();
    projection.setToIdentity();





    mainShaderProg->release();
}

// Add your function implementations below

// TODO: add your code
