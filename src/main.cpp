// g++ src/main.cpp -lglut -lGLEW
#include <GL/glew.h>
//#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>
#include "io/Window.hpp"
#include "renderer/Renderer.hpp"
#include <assert.h>
#include "kiwi/core/all.hpp"
#include "renderer/Shader.hpp"
#include <QApplication>
#include <QGLFormat>
#include <QtUiTools>
#include <QGridLayout>
#include <QWidget>
#include <QFrame>
#include <assert.h>

#define WINDOW_TITLE_PREFIX "Raymarcher Shader"
#define WIDTH     400
#define HEIGHT    400
#include <iostream>

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);

void InitKiwi();

int main(int argc, char* argv[])
{

    InitKiwi();
    QApplication raymarcher( argc, argv );

    // Specify an OpenGL 3.3 format using the Core profile.
    // That is, no old-school fixed pipeline functionality
    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    //glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setAlpha( true );
    glFormat.setSampleBuffers( true );

    glewExperimental = GL_TRUE;
    renderer::Renderer* _renderer = new renderer::Renderer(WIDTH, HEIGHT);
    io::GLWidget glsection (glFormat);
    //glsection.show();

    QUiLoader loader;

    QFile uiFile("../mainwindow.ui");
    uiFile.open(QFile::ReadOnly);

    QWidget *mainUi = loader.load(&uiFile);
    uiFile.close();

    auto renderFrame = mainUi->findChild<QFrame*>("renderFrame");
    assert(renderFrame);
    QGridLayout renderFrameLayout;
    renderFrameLayout.addWidget( &glsection  );
    renderFrame->setLayout( &renderFrameLayout );

    mainUi->show();

    glsection.setRenderer(_renderer);

    return (raymarcher.exec());
}

