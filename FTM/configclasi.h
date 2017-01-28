#ifndef CONFIGCLASI_H
#define CONFIGCLASI_H

#include <QWidget>
#include "clasificafig.h"
#include "rectanguloClasi.h"

class conFigClasi : public QWidget
{
	Q_OBJECT

public:
	//figSeleccionada
	clasificaFig *FigSelect;

	conFigClasi(QWidget *parent);
	~conFigClasi();

public slots:
	void setFigActual(clasificaFig *fig);

protected:
	void paintEvent (QPaintEvent *);
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);

private:
	
};

#endif // CONFIGCLASI_H
