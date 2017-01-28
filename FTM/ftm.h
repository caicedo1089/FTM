#ifndef FTM_H
#define FTM_H


#include <QtGui/QMainWindow>
#include "ui_ftm.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QPalette>
#include <QColorGroup>
#include <QPoint>
#include <QString>
#include <iostream>

using namespace std;

class FTM : public QMainWindow
{
	Q_OBJECT

public:
	FTM(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FTM();
	void resizeEvent(QResizeEvent * e);
	QString NameImg;
	QLabel *actionStatusBar;
	int mapaSelect;


private:
	Ui::FTMClass ui;
	void keyPressEvent (QKeyEvent * event );
	void keyReleaseEvent (QKeyEvent * event );

private slots:
	//acciones del menú
	void on_actionAbrir_PVM_triggered();
	void on_actionAbrir_FTs_triggered();
	void on_actionAbrir_TexturasVol_triggered();
	void on_actionGuardar_FTs_triggered();
	void on_actionGuardar_TexturasVol_triggered();

	void on_actionMostrarBox_triggered();
	void on_actionColorBox_triggered();
	void on_actionMostrarFTM_triggered();
	void on_AutoRefresh_stateChanged (int estado);

	//Tabla de FigClasi
	void on_crearFigClasi_clicked();
	void on_refrescarVol_clicked();
	void on_subirFigClasi_clicked();
	void on_bajarFigClasi_clicked();
	void on_eliminarFigClasi_clicked();
	void on_tablaFigClasi_cellActivated(int fil, int col);

	//configuración de la fig de clasificación
	void on_sliderConFigIni_valueChanged(int valor);
	void on_sliderConFigFin_valueChanged(int valor);
	void on_sliderAlphaFin_valueChanged(int valor);
	void on_sliderAlphaIni_valueChanged(int valor);
	void on_sliderConFigIni_sliderReleased();
	void on_sliderConFigFin_sliderReleased();
	void on_sliderAlphaFin_sliderReleased();
	void on_sliderAlphaIni_sliderReleased();

	//para el color
	void label_Clicked(int tip);
	//para actulizar el color
	void actualFig(int fil, int col);

	//kernels para las figClasi
	void on_tamKernels_valueChanged(int valor);
	void on_calculaKernels_clicked();
	void on_actionDif_Centrales_triggered();
	void on_actionKernels_triggered();
	void on_actionPropiedades_triggered();
	void on_actionLaplaciano_triggered();
	void on_actionGrad_del_Grad_triggered();
	void on_actionM_Hessian_triggered();


	//activar dimensiones
	void on_actionMostrar_WidgetFTM_triggered(); 
	void on_actionMostrar_WidgetFTuni_triggered();
	void on_Push1D_clicked();
	void on_Push2D_clicked();
	void on_Push3D_clicked();
	void on_Push1DFTUni_clicked();
	void on_Push2Do3DFTUni_clicked();
	void on_sliderTruncaFTMWidget_valueChanged(int valor);
	void on_spinMinFTMhisto2D_valueChanged(int valor);
	void on_spinMaxFTMhisto2D_valueChanged(int valor);
	void on_spinActualFTMhisto2D_valueChanged(int valor);

	

	//para configurar el volumen
	void on_lineEditValor_returnPressed(); 
	void on_spinMinX_valueChanged(int valor);
	void on_spinMinY_valueChanged(int valor);
	void on_spinMinZ_valueChanged(int valor);
	void on_spinMaxX_valueChanged(int valor);
	void on_spinMaxY_valueChanged(int valor);
	void on_spinMaxZ_valueChanged(int valor);
	void on_doubleSpinMuestreo_valueChanged(double valor);
	void on_sliderEscalaX_valueChanged(int valor);
	void on_sliderEscalaY_valueChanged(int valor);
	void on_sliderEscalaZ_valueChanged(int valor);
	void on_actionPropiedadesVol_triggered();

	//para configurar la iluminación
	void on_actionMoverIlu_triggered();
	void on_radioGradiente_clicked();
	void on_radioSegDerivada_clicked();
	void on_radioMagYSeg_clicked();
	void on_radioBlinnPhong_clicked();
	void on_pushAmbiental_clicked();
	void on_pushDifusa_clicked();
	void on_pushEspecular_clicked();
	void on_actionActivarIlu_triggered();
	void on_actionPropiedadesIlu_triggered();
	void on_doubleSpinShinIlumina_valueChanged(double valor);

	//senales de tabla
	void on_tablaFigClasi_itemSelectionChanged();
	
	//para actulizar la confClasi
	void actulizaEstadoConfClasi();

};

#endif // FTM_H
