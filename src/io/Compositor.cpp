#include "Compositor.hpp"

#include <QGraphicsView>
#include <QMenu>
#include <QString>
#include <QList>
#include <QAction>

#include <assert.h>

#include "io/NodeView.hpp"
#include "io/PortView.hpp"
#include "io/CreateNodeAction.hpp"

#include "kiwi/core/NodeTypeManager.hpp"
#include "kiwi/core/OpConnect.hpp"
#include "kiwi/core/Node.hpp"

namespace io{

static Compositor * s_instance = 0;
static QMenu * s_menu = 0;
static QList<QAction*> s_actions;

void Compositor::Create( QGraphicsView * v )
{
    assert(s_instance == 0);
    s_instance = new Compositor(v);
}

Compositor& Compositor::Instance()
{
    assert( s_instance != 0 );
    return *s_instance;
}

Compositor::Compositor(QGraphicsView * v)
{
    assert(v);
    _view = v;
    _view->setScene( &_scene );
}

Compositor::~Compositor()
{
    _view->setScene(0);
}

void Compositor::menu( QPoint pos )
{
    std::cerr << "contextMenuEvent\n";
    QMenu menu; //= new QMenu;
    menu.addSeparator();
    QString s = "Add node...";
    menu.addActions( s_actions );
    menu.exec( pos );
    menu.show();
}

void Compositor::add(NodeView * nv)
{
    nv->addToScene( &_scene );
}

void Compositor::addNodeToMenu( const QString& name, void(*fptr)(const QPointF&) )
{
    s_actions.push_back( new CreateNodeAction(name, fptr) );
}

bool Compositor::tryConnect( PortView* p1, PortView * p2)
{
    if ( !p1 || !p2 ) return false;

    PortView * input;
    PortView * output;

    if ( p1->isInput() && p2->isOutput() )
    {
        input = p1;
        output = p2;
    }
    else if ( p1->isOutput() && p2->isInput() )
    {
        input = p2;
        output = p1;
    }
    else return false;

    int in_i = input->nodeView()->indexOf( input );
    int out_i = output->nodeView()->indexOf( output );

    return output->nodeView()->node()->output(out_i) >> input->nodeView()->node()->input(in_i);
}

} //namespace
