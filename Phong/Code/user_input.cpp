#include "mainview.h"

#include <QDebug>

void MainView::updateRotation(int x, int y, int z)
{
    qDebug() << "updateRotation(" << x << "," << y << "," << z << ");";
    rotation.setX(x);
    rotation.setY(y);
    rotation.setZ(z);

    update();
}

void MainView::updateModel(QString name)
{
    qDebug() << "updateModel(" << name << ")";
    // TODO: load new model, based on name

    update();
}

void MainView::updateShader(QString name)
{
    qDebug() << "updateShader(" << name << ")";
    // TODO: switch shaders/renderingprocesses, based on name

    update();
}

void MainView::updateScale(float scale)
{
    qDebug() << "updateScale(" << scale << ")";
    //update model scale

    sc=scale;
    if(sc<0.2){ //If it is too small
        sc=0.2;
    }

    if(sc>1.2){ //If it is too big to fit in the screen
       sc=1.2;
    }
    update();
}


// Triggered by pressing a key
void MainView::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case 'A': qDebug() << "A pressed"; break;
    default:
        // ev->key() is an integer. For alpha numeric characters keys it equivalent with the char value ('A' == 65, '1' == 49)
        // Alternatively, you could use Qt Key enums, see http://doc.qt.io/qt-5/qt.html#Key-enum
        qDebug() << ev->key() << "pressed";
        break;
    }

    // Used to update the screen after changes
    update();
}

// Triggered by releasing a key
void MainView::keyReleaseEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case 'A': qDebug() << "A released"; break;
    default:
        qDebug() << ev->key() << "released";
        break;
    }

    update();
}

// Triggered by clicking two subsequent times on any mouse button
// It also fires two mousePress and mouseRelease events!
void MainView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse double clicked:" << ev->button();

    update();
}

// Triggered when moving the mouse inside the window (only when the mouse is clicked!)
void MainView::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << "x" << ev->x() << "y" << ev->y();

    float X = 0;
    float Y = 0;
    X = startingXMouse - ev->x();
    startingXMouse =  ev->x();
    Y = startingYMouse - ev->y();
    startingYMouse = ev->y();
    rotation.setX(rotation.x() - Y);
    rotation.setY(rotation.y() - X);

    update();
}

// Triggered when pressing any mouse button
void MainView::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button pressed:" << ev->button();
    startingXMouse = ev->x();
    startingYMouse = ev->y();
    update();
    // Do not remove the line below, clicking must focus on this widget!
    this->setFocus();
}

// Triggered when releasing any mouse button
void MainView::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button released" << ev->button();

    update();
}

// Triggered when clicking scrolling with the scroll wheel on the mouse
void MainView::wheelEvent(QWheelEvent *ev)
{
    // Implement something
    qDebug() << "Mouse wheel:" << ev->delta();
    if(ev->delta()<0){
        sc -= 0.2;
        if(sc<0.2){
            sc = 0.2;
        }
        update();
    }
    else if(ev->delta()>0){
        sc += 0.1;
        if(sc>1.2){
            sc = 1.2;
        }
        update();
    }

}
