#include "mainview.h"

void MainView::renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos)
{
    // OpenGL assignment 1, part 2: create a function to render the sphere
    // Use Model(":/models/sphere.obj") for the model

    // Set the matrices to the unit matrix
    model.setToIdentity();
    view.setToIdentity();

    // Make view matrix translate
    view.translate(-200,-200,-1000);

    // Make model matrix rotate around (200,200,200) in world space
   // model.translate(-200,-200,-200);
    model.translate(200,200,200);
    model.rotate(rotation.x(),200,0,0);
    model.rotate(rotation.y(),0,200,0);
    model.rotate(rotation.z(),0,0,200);
    model.translate(-200,-200,-200);


    // Make the model matrix translate in position "pos"
    model.translate(pos);

    // Scale the model matrix
    model.scale(sc);

    // Obtain the normal matrix
    normal = model.normalMatrix();

    // Obtain light position
    lightpos=model*lightpos;

    // Upload values to the GPU
    glUniform3f(uniformLightpos,lightpos.x(),lightpos.y(),lightpos.z());
    glUniform3f(uniformCamara,-200,-200,-1000);
    glUniform3f(uniformPosition,pos.x(),pos.y(),pos.z());
    glUniform4f(uniformIntensity,material.x(),material.y(),material.z(),material.w());

    glUniformMatrix4fv(uniformModel,1,GL_FALSE,model.data());
    glUniformMatrix4fv(uniformProjection,1,GL_FALSE,projection.data());
    glUniformMatrix4fv(uniformView,1,GL_FALSE,view.data());
    glUniformMatrix3fv(uniformNormal,1,GL_FALSE,normal.data());

    glUniform3f(uniformMaterialColor,color.x(),color.y(),color.z());

    // Draw the model
    glDrawArrays(GL_TRIANGLES,0,sphereModel->getVertices().size());

}

/**
 * Renders a similar scene used for the raytracer:
 * 5 colored spheres with a single light
 */
void MainView::renderRaytracerScene()
{
    QVector3D lightpos = QVector3D(-200,600,1500);

    // Blue sphere
    renderSphere(QVector3D(90,320,100),QVector3D(0,0,1),QVector4D(0.2f,0.7f,0.5f,64),lightpos);

    // Green sphere
    renderSphere(QVector3D(210,270,300),QVector3D(0,1,0),QVector4D(0.2f,0.3f,0.5f,8),lightpos);

    // Red sphere
    renderSphere(QVector3D(290,170,150),QVector3D(1,0,0),QVector4D(0.2f,0.7f,0.8f,32),lightpos);

    // Yellow sphere
    renderSphere(QVector3D(140,220,400),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos);

    // Orange sphere
    renderSphere(QVector3D(110,130,200),QVector3D(1,0.5f,0),QVector4D(0.2f,0.8f,0.5f,32),lightpos);
}
