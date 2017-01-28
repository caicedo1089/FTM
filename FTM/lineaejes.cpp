#include "lineaejes.h"

lineaEjes::lineaEjes(QWidget *parent)
{
	 printf("estoy creandoooo");
	 this->setPalette(QPalette(QColor(0,0,1)));
}

lineaEjes::~lineaEjes()
{

}

void lineaEjes::paintEvent (QPaintEvent *)
{
	QRectF rectangle(10.0, 20.0, 80.0, 60.0);

 QPainter painter(this);
 painter.drawRect(rectangle);

	printf("se esta ejecutando");
	
}
void lineaEjes::mousePressEvent(QMouseEvent * mouseEvent)
{
	//printf("estoy en widget, x:%d, y:%d\n",mouseEvent->x(),mouseEvent->y());

	printf("Periosne");
}
void lineaEjes::mouseReleaseEvent(QMouseEvent * mouseEvent)
{

}
void lineaEjes::mouseMoveEvent(QMouseEvent * mouseEvent)
{
}