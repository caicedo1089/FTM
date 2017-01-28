#include "trianguloClasi.h"

trianguloClasi::trianguloClasi()
{
	Z.setX(0);Z.setY(128);coorIni.setX(0);coorIni.setY(0);coorFin.setX(0);coorFin.setY(0);
	colorIni = QColor(255,255,255,255); colorFin = QColor(0,0,0,0);
	DegradadoFin = 0.0; DegradadoIni = 1.0f;
	nombre="sin nombre";
	penFig.setStyle(Qt::NoPen);
}

trianguloClasi::~trianguloClasi()
{

}

void trianguloClasi::pinta(QImage* tabla, int Tam)const
{
	QPainter painter(tabla);
	
	painter.setPen(penFig);
	QLinearGradient linearGradient(coorIni.x(),0,coorIni.x()+(coorFin.x()/2),0);
	linearGradient.setSpread(QGradient::ReflectSpread);
    linearGradient.setColorAt(0.0,colorFin);
	linearGradient.setColorAt(DegradadoFin,colorFin);
    linearGradient.setColorAt(DegradadoIni,colorIni);
    linearGradient.setColorAt(1.0,colorIni);
	painter.setBrush(linearGradient);

	QPointF points[3] = {
		 QPointF(coorIni.x(), Tam-coorIni.y()),
		 QPointF(coorIni.x() + coorFin.x()/2.0f, Tam-coorIni.y()-coorFin.y()),
		 QPointF(coorIni.x() + coorFin.x(), Tam-coorIni.y())
	};

	painter.drawConvexPolygon(points, 3);

}
void trianguloClasi::pintaInFTM(QImage* tabla)const
{
	QPainter painter(tabla);
	painter.setPen(Qt::NoPen);
	
	QLinearGradient linearGradient(coorIni.x(),0,coorIni.x()+(coorFin.x()/2),0);
	linearGradient.setSpread(QGradient::ReflectSpread);
    linearGradient.setColorAt(0.0,colorFin);
	linearGradient.setColorAt(DegradadoFin,colorFin);
	linearGradient.setColorAt(DegradadoIni,colorIni);
    linearGradient.setColorAt(1.0,colorIni);
	painter.setBrush(linearGradient);
	
	QPointF points[3] = {
		 QPointF(coorIni.x(), coorIni.y()),
		 QPointF(coorIni.x() + coorFin.x()/2.0f, coorIni.y() + coorFin.y()),
		 QPointF(coorIni.x() + coorFin.x(), coorIni.y())
	};

	painter.drawConvexPolygon(points, 3);

}
void trianguloClasi::pintaInFTMSelect(QImage* tabla)const
{
	QPainter pintor(tabla);
	//pinto el rectangulo con raya
	QPen mpen(penFig);
	mpen.setStyle(Qt::DashLine);
	mpen.setBrush(Qt::white);

	pintor.setPen(mpen);
	pintor.drawRect(coorIni.x()-1,coorIni.y()-1,coorFin.x()+1,coorFin.y()+1);

	int x1 = coorIni.x()-3, y1 = coorIni.y()-3,x2 = coorIni.x()+coorFin.x(),y2 = coorIni.y()+coorFin.y();

	//tengo que pintar ahora los puntos de control
	mpen.setStyle(Qt::SolidLine);
	pintor.setBrush(Qt::white);
	pintor.setPen(mpen);
	pintor.drawRect(x1,y1,3,3);
	pintor.drawRect(x1,y2,3,3);
	pintor.drawRect(x2,y1,3,3);
	pintor.drawRect(x2,y2,3,3);
}
void trianguloClasi::pintaSeg(QImage* tabla, int Tam)const
{

}
void trianguloClasi::pintaInFTMSeg(QImage* tabla, int Tam)const
{
	QPainter painter(tabla);
	painter.setPen(Qt::NoPen);
	
	QLinearGradient linearGradient(coorIni.x(),0,coorIni.x()+(coorFin.x()/2),0);
	linearGradient.setSpread(QGradient::ReflectSpread);
    linearGradient.setColorAt(0.0,colorFin);
	linearGradient.setColorAt(DegradadoFin,colorFin);
    linearGradient.setColorAt(DegradadoIni,colorIni);
    linearGradient.setColorAt(1.0,colorIni);
	painter.setBrush(linearGradient);
	
	painter.drawRect(coorIni.x(),Z.x(),coorFin.x(),Z.y());//asi para que corresponda con el FTm volumen

}
void trianguloClasi::pintaInFTMSelectSeg(QImage* tabla, int Tam)const
{
	QPainter pintor(tabla);
	//pinto el rectangulo con raya
	QPen mpen(penFig);
	mpen.setStyle(Qt::DashLine);
	mpen.setBrush(Qt::white);

	pintor.setPen(mpen);
	pintor.drawRect(coorIni.x()-1,Z.x()-1,coorFin.x()+1,Z.y()+1);

	int x1 = coorIni.x()-3, y1 =Z.x()-3,x2 = coorIni.x()+coorFin.x(),y2 = Z.x()+Z.y();

	//tengo que pintar ahora los puntos de control
	mpen.setStyle(Qt::SolidLine);
	pintor.setBrush(Qt::white);
	pintor.setPen(mpen);
	pintor.drawRect(x1,y1,3,3);
	pintor.drawRect(x1,y2,3,3);
	pintor.drawRect(x2,y1,3,3);
	pintor.drawRect(x2,y2,3,3);
}