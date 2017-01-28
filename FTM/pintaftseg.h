#ifndef PINTAFTSEG_H
#define PINTAFTSEG_H

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

class pintaFTSeg : public QWidget
{
	Q_OBJECT
public:


	pintaFTSeg();
	~pintaFTSeg();
	
	//atributos
	QImage *MyImagenAct;
	QImage *MyImagenBaseAct;
	QImage *MyImagenBase;

	QVector<QImage*> *volumeFTM;

	FunctionTrans *volumenPinta;

	//mis figuras de selección
	clasificaFig *FigSelect;
	QVector<clasificaFig*> *vectorOfFig;
	QTableWidget *tablaFig; 


	//flat
	bool soloUnaVez;
	bool moverFigClasi;
	int escalaFigClasi;
	QPoint postAct;
	bool escalaFigClasiLado;

	//auto refrescar
	bool autoRefresh;

	void setMyImagen(QImage * Image);
	void paintInImage(QImage * Image);
	void paintInFTM(QImage * Image);

	void insertArriba(int fil,QTableWidget *tablaFig,QVector<clasificaFig*> *vectorOfFig);

	//procedimiento que permite actualizar las FTM cuando se elimina
	void actualizaFtmElimina(clasificaFig *FigSelectNew);

public slots:
	//selecciono la FigClasi
	void SeleccionoFigOFTable();
	//para actualizar cuando presiono en la otra FTM
	void actualizarFTMSeg(clasificaFig *Fig);
	//para cambiar de figura
	void actulizarFigClasi(int fig);
	//para activar la figura o no
	void activaFigClasi(int fig);
	//para crear el comboBox
	void creaComboBox(int fil, int col);
	//para volver todo normaldespues de seleccionar un combo box
	void cambioCellSelect(int fil,int col,int filAnt,int colAnt);
signals:
	void cambioFigSeg(clasificaFig *Fig);
	void updateVol();
	void creaFig();

protected:
	void paintEvent (QPaintEvent *);
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);

	
};

#endif // PINTAFTSEG_H
