#pragma once
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QColor>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QString>
#include <QPen>
#include <math.h>

using namespace std;


class clasificaFig
{
public:
	QPoint Z,coorIni,coorFin;
	QColor colorIni,colorFin;
	float DegradadoIni,DegradadoFin;
	bool bordeFig;
	bool FigSelect;
	QPen penFig;
	QString nombre;
	int tipoFig;


public:
	virtual void pinta(QImage* tabla, int Tam)const=0;
	virtual void pintaInFTM(QImage* tabla)const=0;
	virtual void pintaInFTMSelect(QImage* tabla)const=0;

	virtual void pintaSeg(QImage* tabla, int Tam)const=0;
	virtual void pintaInFTMSeg(QImage* tabla, int Tam)const=0;
	virtual void pintaInFTMSelectSeg(QImage* tabla, int Tam)const=0;
	
	
};
