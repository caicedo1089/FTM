#include "ejeLinea.h"

ejeLinea::ejeLinea(QWidget *parent)
	: QWidget(parent)
{
	linea.setLine(0,3,258,3);
	
	lapiz.setWidth(5);
	colorLinea = Qt::green;
}

ejeLinea::~ejeLinea()
{
	

}

void ejeLinea::paintEvent (QPaintEvent *)
{
	 QPainter pintor(this);
	 lapiz.setBrush(colorLinea);
	 pintor.setPen(lapiz);
	 pintor.drawLine(linea);
}
void ejeLinea::mousePressEvent(QMouseEvent * mouseEvent)
{
	//printf("Presiono x:%d, y:%d\n",mouseEvent->x(),mouseEvent->y());
	//para cambiar el color
}
void ejeLinea::mouseReleaseEvent(QMouseEvent * mouseEvent)
{

}
void ejeLinea::mouseMoveEvent(QMouseEvent * mouseEvent)
{

}
