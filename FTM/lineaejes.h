#ifndef LINEAEJES_H
#define LINEAEJES_H

#include <QWidget>
#include <QPainter>

class lineaEjes : public QWidget
{
	Q_OBJECT

public:
	lineaEjes(QWidget *parent);
	~lineaEjes();

private:

protected:
	void paintEvent (QPaintEvent *);
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);
	
};

#endif // LINEAEJES_H
