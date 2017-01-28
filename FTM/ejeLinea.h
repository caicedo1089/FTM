#ifndef EJELINEA_H
#define EJELINEA_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QMouseEvent>

class ejeLinea : public QWidget
{
public:
	//atributos
	QColor colorLinea;
	QPen lapiz;
	QLine linea;

	ejeLinea(QWidget *parent);
	~ejeLinea();

protected:
	void paintEvent (QPaintEvent *);
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);

private:
};

#endif // EJELINEA_H
