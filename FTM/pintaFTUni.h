#ifndef PINTAFTUNI_H
#define PINTAFTUNI_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <algorithm>
#include <QColor>
#include <QPair>
#include <QLinearGradient>
#include <QColorDialog>
#include <QImage>
#include <math.h>

class pintaFTUni : public QWidget
{
	Q_OBJECT

public:
	pintaFTUni();
	~pintaFTUni();

	//Atributos
	QVector<QPair<int,QColor>> puntosControlFT;
	QPair<int,QColor> *actualPunto,*antesPunto,*despuesPunto;
	QImage *texturaFTUni;
	unsigned int texUni[256];

	//pintar en el arreglo
	void drawFTUni();

protected:
	void paintEvent (QPaintEvent *);
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);
	void mouseDoubleClickEvent (QMouseEvent * mouseEvent);

signals:
	void loadFT1();
	
};

#endif // PINTAFTUNI_H
