#include "edge.h"
#include "node.h"

#include <math.h>

#include <QPainter>
#include <QGraphicsTextItem>


static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode,double weight)
    : arrowSize(10)
{
    setAcceptedMouseButtons(0);
    state=0;
    source = sourceNode;
    dest = destNode;
    cost=weight;
sourceNode->add_successor_node(destNode); // adding the destination to the list of sucessors of the source node
    source->addEdge(this);
   dest->addEdge(this);
   source->add_successor_edge(this);
    adjust();

}

Node* Edge::sourceNode() const
{
    return source;
}

Node* Edge::destNode() const
{
    return dest;
}

Edge* Edge::edge_pointer() // an accessor to the pointer "this"
{
    return this;
}
void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

//accessor to the cost
double Edge::edge_cost() const
{
    return cost;
}



// drawing stuffs
QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    if (state==1)
    {
        painter->setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else if (state==-1)
    {
        painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else
    {
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);
    // drawing the cost of the edge
   painter->drawText( QPoint((sourcePoint.x()+destPoint.x())/2,(destPoint.y()+sourcePoint.y())/2),QString::number(cost)  );
    // Draw the arrows , I deleted the arrows from the source point to add them check the project elastic nodes
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    if (state==1){
        painter->setBrush(Qt::green);
    }
    else if (state==-1){
        painter->setBrush(Qt::red);
    }
    else{
        painter->setBrush(Qt::black);
    }
    //painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
