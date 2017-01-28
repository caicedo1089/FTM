#pragma once

//volumen de la textura
#include "FunctionTrans.h"

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include "clasificaFig.h"
#include "rectanguloClasi.h"
#include "circuloClasi.h"
#include "trianguloClasi.h"
#include <QVector>
#include <QTableWidget>
#include <QComboBox>
#include <algorithm>


class pintaFT :
	public QWidget
{
	Q_OBJECT
public:
	//Atributos
	QImage *MyImagenAct;
	QImage *MyImagenBaseAct;
	QImage *MyImagenBase;

	//Volumen de la FTM
	QVector<QImage*> volumeFTM;

	//FTM
	FunctionTrans *volumenPinta;

	//mis figuras de selección
	clasificaFig *FigSelect;
	QVector<clasificaFig*> vectorOfFig;
	QTableWidget *tablaFig; 

	//flat
	bool soloUnaVez;
	bool moverFigClasi;
	int escalaFigClasi;
	QPoint postAct;

	//auto refrescar
	bool autoRefresh;

	pintaFT();
	~pintaFT(void);

	void setMyImagen(QImage * Image);
	void paintInImage(QImage * Image);
	void paintInFTM(QImage * Image);

	//estan como slot para poderlas acceder desde mi drawFTMseg con una senal
	void crearVolumenofClasificacion();
	unsigned char* bitsVolumenClasi();
	
	
public slots:
	//selecciono la FigClasi
	void Selecciono();
	//actualizar
	void actualizarFTM(clasificaFig *Fig);
	//cre el combo box de la edicion de fig
	void creaComboBox(int fil, int col);
	//actualiza figClasi
	void actulizarFigClasi(int fig);
	//ativa o desactiva la figClasi
	void activaFigClasi(int fig);
	//para devolver todo a la normalidad despues de tener un combobox
	void cambioCellSelect(int fil,int col,int filAnt,int colAnt);
	

signals:
	void cambioFig(clasificaFig *Fig);
	void updateVol();
	void creaFig();

protected:
	void paintEvent (QPaintEvent *);
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);


};
