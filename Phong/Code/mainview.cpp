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
    delete sphereModel;

    // Free Buffer Objects before Vertex Arrays
    glDeleteBuffers(1,&sphereBO);
    glDeleteBuffers(1,&normalBO);
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
    // Store the locations (pointers in gpu memory) of uniforms in Glint's

    uniformModel = glGetUniformLocation(mainShaderProg->programId(),"model");
    uniformView = glGetUniformLocation(mainShaderProg->programId(),"view");
    uniformProjection = glGetUniformLocation(mainShaderProg->programId(),"projection");
    uniformNormal = glGetUniformLocation(mainShaderProg->programId(),"normal");
    uniformMaterialColor = glGetUniformLocation(mainShaderProg->programId(),"materialColor");
    uniformLightpos = glGetUniformLocation(mainShaderProg->programId(),"lightpos");
    uniformIntensity = glGetUniformLocation(mainShaderProg->programId(),"intensity");
    uniformPosition = glGetUniformLocation(mainShaderProg->programId(),"pos");
    uniformCamara = glGetUniformLocation(mainShaderProg->programId(),"camara");

    /* End of custom shaders */



}

/**
 * @brief MainView::createBuffers
 *
 * Creates necessary buffers for your application
 */
void MainView::createBuffers() {
    //Implement buffer creation
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    //Generating/Binding shape buffer
    glGenBuffers(1,&sphereBO);
    glBindBuffer(GL_ARRAY_BUFFER,sphereBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);


    //Generating/Binding normal buffer
    glGenBuffers(1,&normalBO);
    glBindBuffer(GL_ARRAY_BUFFER,normalBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);


    //Unbinding VertexArray buffer
    glBindVertexArray(0);
}

void MainView::loadModel(QString filename, GLuint bufferObject) {

    sphereModel = new Model(filename);
    vertices = sphereModel->getVertices();
    QVector<QVector3D> normals;

    //pushing of data to GPU using shape buffer pointer
    glBindBuffer(GL_ARRAY_BUFFER,bufferObject);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vertices.size()*3,vertices.data(),GL_DYNAMIC_DRAW);

    //Get the normals of the model
    normals = sphereModel->getNormals();

    //pushing of data to GPU using normal pointer
    glBindBuffer(GL_ARRAY_BUFFER,normalBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*normals.size()*3,normals.data(),GL_DYNAMIC_DRAW);

}

void MainView::updateBuffers() {
    // Change the data inside buffers (if you want)
    // make sure to change GL_STATIC_DRAW to GL_DYNAMIC_DRAW
    // in the call to glBufferData for better performance
    qDebug()<<"Update Buffers";
}


void MainView::updateUniforms() {
    // TODO: update the uniforms in the shaders using the glUniform<datatype> functions
    qDebug()<<"Update uniforms";
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
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* TODO: call your initialization functions here */

    createShaderPrograms();

    createBuffers();

    loadModel(":/models/sphere.obj", sphereBO);

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
    //Update projection to fit the new aspect ratio
    projection.setToIdentity();
    projection.perspective(30.0,(float)newWidth/(float)newHeight,400.0,1200.0);
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

    //Bind the Vertex Array before drawing
    glBindVertexArray(VAO);

    // Implement your drawing functions
    renderRaytracerScene();

    mainShaderProg->release();
}

