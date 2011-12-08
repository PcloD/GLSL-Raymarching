#ifndef PORTVIEW_HPP
#define PORTVIEW_HPP

#include <QGraphicsItem>

namespace io{

class NodeView;
class LinkView;

class PortView : public QGraphicsItem
{
public:
    enum{ INPUT, OUTPUT };
    enum{ IDLE = 0, DRAG };
    typedef std::vector<LinkView*> LinkArray;

    PortView( int inputOrOutput, NodeView* nv, int idx );
    ~PortView();
    int index() const
    {
        return _index;
    }

    NodeView * nodeView() const
    {
        return _nodeView;
    }

    int state() const
    {
        return _state;
    }

    bool isCompatible( PortView * p );
    bool connect( PortView * p );

    const LinkArray& connections() const
    {
        return _connections;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    NodeView * _nodeView;
    int _index;
    int _alpha;
    int _io;
    int _state;
    LinkArray _connections;
};

}//namespace

#endif // PORTVIEW_HPP
