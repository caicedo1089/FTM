#include "pintaFTUni.h"
#define MAKE_PIXEL(a,r,g,b) (a << 24) + (r << 16) + (g << 8) + b

pintaFTUni::pintaFTUni()
{
	//identidad
	puntosControlFT.push_back(QPair<int,QColor>(0,QColor(0,0,0,0)));
	puntosControlFT.push_back(QPair<int,QColor>(255,QColor(255,255,255,255)));
	
	actualPunto=antesPunto=despuesPunto=NULL;
	//mi textura
	texturaFTUni = new QImage(256,1,QImage::Format_ARGB32);
	for(int i=0;i<256;++i)
	{
		texturaFTUni->setPixel(i,0,QColor(i,i,i,i).rgba());
		texUni[i] = MAKE_PIXEL(i,i,i,i);
	}
}

pintaFTUni::~pintaFTUni()
{

}
void pintaFTUni::drawFTUni()
{
	for(int i=0; i<puntosControlFT.size(); ++i)
	{
		if(i+1 < puntosControlFT.size())
		{
			int iter = abs(puntosControlFT[i].first - puntosControlFT[i+1].first);
			float dif = 1.0f/(float)iter;
			float interpola = 1.0f;
			int r,g,b,a;
			for(int j= puntosControlFT[i].first; j<=puntosControlFT[i+1].first; ++j)
			{
				r = (int)((interpola)*(float)puntosControlFT[i].second.red() + (1.0f-interpola)*(float)puntosControlFT[i+1].second.red());
				g = (int)((interpola)*(float)puntosControlFT[i].second.green() + (1.0f-interpola)*(float)puntosControlFT[i+1].second.green());
				b = (int)((interpola)*(float)puntosControlFT[i].second.blue() + (1.0f-interpola)*(float)puntosControlFT[i+1].second.blue());
				a = (int)((interpola)*(float)puntosControlFT[i].second.alpha() + (1.0f-interpola)*(float)puntosControlFT[i+1].second.alpha());

				interpola-=dif;
				texturaFTUni->setPixel(j,0,QColor(r,g,b,a).rgba());
				texUni[j] = MAKE_PIXEL(r,g,b,a);
			}
		}
	}

}
void pintaFTUni::paintEvent (QPaintEvent *)
{
	//se pinta la textura
	QPainter painter(this);
	painter.setPen(Qt::NoPen);

	for(int i =0;i<256; ++i)
		painter.drawImage(0,i,*texturaFTUni,0,0,texturaFTUni->width(),texturaFTUni->height());

	//se pinta los puntos de control
	painter.setPen(Qt::blue);
	//painter.setBrush(Qt::blue);
	for(int i=0; i<puntosControlFT.size(); ++i)
	{
		if(i+1 < puntosControlFT.size())
		{
			painter.setPen(Qt::blue);
			painter.setBrush(Qt::blue);
			painter.drawLine(puntosControlFT[i].first,255-puntosControlFT[i].second.alpha(),puntosControlFT[i+1].first,255-puntosControlFT[i+1].second.alpha());
		}
		painter.setPen(Qt::blue);
		painter.setBrush(QColor(puntosControlFT[i].second.red(),puntosControlFT[i].second.green(),puntosControlFT[i].second.blue()));
		painter.drawRect(puntosControlFT[i].first-2,255-puntosControlFT[i].second.alpha()-2,4,4);
	}

}
void pintaFTUni::mousePressEvent(QMouseEvent * mouseEvent)
{
	int x=mouseEvent->x(),y=mouseEvent->y();
	actualPunto=antesPunto=despuesPunto=NULL;

	int antes = -1, elimina = -1;
	//reviso si presiono un punto de control
	for(int i=0; i<puntosControlFT.size(); ++i)
	{
		if(x<=puntosControlFT[i].first+2 && x>=puntosControlFT[i].first-2 && y<=(255-puntosControlFT[i].second.alpha()+2) && y>=(255-puntosControlFT[i].second.alpha()-2)){
			actualPunto = &puntosControlFT[i];
			(i+1<puntosControlFT.size())? despuesPunto = &puntosControlFT[i+1]:despuesPunto = NULL;
			(i-1<0)? antesPunto =NULL: antesPunto = &puntosControlFT[i-1];

			//pos a liminar
			elimina = i; 

		}
		//guardo el antes del x por si me toca crear punto
		if( x < puntosControlFT[i].first && antes ==-1) antes = i;
	}
	//creo punto si no es igual a un punto de control
	if(actualPunto==NULL && antes !=-1 && (mouseEvent->buttons() & Qt::LeftButton))
	{
		QPair<int,QColor> aux(x,QColor(255,255,255,255-y));
		puntosControlFT.insert(antes,aux);
	}
	if(actualPunto!=NULL && (mouseEvent->buttons() & Qt::RightButton))
	{
		if( puntosControlFT[elimina].first != 0 && puntosControlFT[elimina].first != 255){
			puntosControlFT.remove(elimina);
			actualPunto=antesPunto=despuesPunto=NULL;
		}
	}

	drawFTUni();
	update();
	emit loadFT1();

}
void pintaFTUni::mouseReleaseEvent(QMouseEvent * mouseEvent)
{

}
void pintaFTUni::mouseMoveEvent(QMouseEvent * mouseEvent)
{
	if(actualPunto==NULL) return;
	int x=mouseEvent->x(),y=mouseEvent->y();
	if(y>255) y=255;if(y<0) y=0;

	if(antesPunto==NULL || despuesPunto==NULL){
		
		actualPunto->second.setAlpha(255-y);
		drawFTUni();
		update();
		emit loadFT1();
	}else{
		if( x>antesPunto->first && x<despuesPunto->first)
		{
			actualPunto->second.setAlpha(255-y);
			actualPunto->first = x;
			drawFTUni();
			update();
			emit loadFT1();
		}
	}

}
void pintaFTUni::mouseDoubleClickEvent(QMouseEvent * mouseEvent)
{
	int x=mouseEvent->x(),y=mouseEvent->y();

	for(int i=0; i<puntosControlFT.size(); ++i)
	{
		if(x<=puntosControlFT[i].first+2 && x>=puntosControlFT[i].first-2 && y<=(255-puntosControlFT[i].second.alpha()+2) && y>=(255-puntosControlFT[i].second.alpha()-2)){
			puntosControlFT[i].second.setRgba(QColorDialog::getColor(puntosControlFT[i].second,this,tr("Elija el color"),QColorDialog::ShowAlphaChannel).rgba());
			drawFTUni();
			update();
			emit loadFT1();
			return; 
		}
	}
}