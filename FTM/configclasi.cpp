#include "configclasi.h"

conFigClasi::conFigClasi(QWidget *parent)
	: QWidget(parent)
{
	FigSelect = NULL;
}

conFigClasi::~conFigClasi()
{

}
void conFigClasi::paintEvent (QPaintEvent *)
{
	if(FigSelect == NULL) return;
	
	QPainter painter(this);
	
	//si es un rectangulo
	if(FigSelect->tipoFig == 0){

		//para pintar el borde
		QPen mpen(FigSelect->penFig);
		mpen.setStyle(Qt::DashLine);
		mpen.setBrush(Qt::white);
		painter.setPen(mpen);

		QLinearGradient linearGradient((width()-FigSelect->coorFin.x())/2,0,(width()-FigSelect->coorFin.x())/2+(FigSelect->coorFin.x()/2),0);
		linearGradient.setSpread(QGradient::ReflectSpread);
		linearGradient.setColorAt(0.0,FigSelect->colorFin);
		linearGradient.setColorAt(FigSelect->DegradadoFin,FigSelect->colorFin);
		linearGradient.setColorAt(FigSelect->DegradadoIni,FigSelect->colorIni);
		linearGradient.setColorAt(1.0,FigSelect->colorIni);
		painter.setBrush(linearGradient);
		
		painter.drawRect((width()-FigSelect->coorFin.x())/2,(height()-FigSelect->coorFin.y())/2,FigSelect->coorFin.x(),FigSelect->coorFin.y());//asi para que corresponda con el FTm volumen

	}else{
		if(FigSelect->tipoFig == 2){
			//pinto el rectangulo con raya
			QPen mpen(FigSelect->penFig);
			mpen.setStyle(Qt::DashLine);
			mpen.setBrush(Qt::white);
			painter.setPen(mpen);
			painter.drawRect((width()-FigSelect->coorFin.x())/2,(height()-FigSelect->coorFin.y())/2,FigSelect->coorFin.x(),FigSelect->coorFin.y());

			painter.setPen(Qt::NoPen);

			float p1 = ((width()-FigSelect->coorFin.x())/2.0f), p2 = ((height()-FigSelect->coorFin.y())/2.0f);

			float centrox = ( p1 + (FigSelect->coorFin.x()/2.0f)), centroy = ( p2 + (FigSelect->coorFin.y()/2.0f));
			float radio = sqrtf(((p1-centrox)*(p1-centrox)) + ((p2-centroy)*(p2-centroy)));
			QRadialGradient radialGradient(centrox,centroy,radio);
			radialGradient.setSpread(QGradient::ReflectSpread);
			radialGradient.setColorAt(0.0,FigSelect->colorFin);
			radialGradient.setColorAt(FigSelect->DegradadoFin,FigSelect->colorFin);
			radialGradient.setColorAt(FigSelect->DegradadoIni,FigSelect->colorIni);
			radialGradient.setColorAt(1.0,FigSelect->colorIni);
			painter.setBrush(radialGradient);
			
			painter.drawEllipse(p1,p2,FigSelect->coorFin.x(),FigSelect->coorFin.y());//asi para que corresponda con el FTm volumen
		}else{
			//pinto el rectangulo con raya
			QPen mpen(FigSelect->penFig);
			mpen.setStyle(Qt::DashLine);
			mpen.setBrush(Qt::white);
			painter.setPen(mpen);
			painter.drawRect((width()-FigSelect->coorFin.x())/2,(height()-FigSelect->coorFin.y())/2,FigSelect->coorFin.x(),FigSelect->coorFin.y());

			painter.setPen(Qt::NoPen);

			QLinearGradient linearGradient((width()-FigSelect->coorFin.x())/2.0f,0,(width()-FigSelect->coorFin.x())/2.0f+(FigSelect->coorFin.x()/2.0f),0);
			linearGradient.setSpread(QGradient::ReflectSpread);
			linearGradient.setColorAt(0.0,FigSelect->colorFin);
			linearGradient.setColorAt(FigSelect->DegradadoFin,FigSelect->colorFin);
			linearGradient.setColorAt(FigSelect->DegradadoIni,FigSelect->colorIni);
			linearGradient.setColorAt(1.0,FigSelect->colorIni);
			painter.setBrush(linearGradient);

			QPointF points[3] = {
				 QPointF((width()-FigSelect->coorFin.x())/2,(height()-FigSelect->coorFin.y())/2),
				 QPointF((width()-FigSelect->coorFin.x())/2 + FigSelect->coorFin.x()/2.0f, (height()-FigSelect->coorFin.y())/2 + FigSelect->coorFin.y()),
				 QPointF((width()-FigSelect->coorFin.x())/2+FigSelect->coorFin.x(),(height()-FigSelect->coorFin.y())/2)
			};

			painter.drawConvexPolygon(points, 3);
		}
	
	}
}
void conFigClasi::mousePressEvent(QMouseEvent * mouseEvent)
{

}
void conFigClasi::mouseReleaseEvent(QMouseEvent * mouseEvent)
{

}
void conFigClasi::mouseMoveEvent(QMouseEvent * mouseEvent)
{

}

void conFigClasi::setFigActual(clasificaFig *fig)
{
	//bueno aca puedo crear una fig aux para no trabajar en la actual
	// de tal forma que si quiera hechar para atras lo pueda hacer
	FigSelect = fig;

	//actulizo
	update();
}