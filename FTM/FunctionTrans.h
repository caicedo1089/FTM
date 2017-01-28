#pragma once
#include "Textura3d.h"
#include <math.h>
#include <QImage>
#include <QVector3D>
#include <QColor>
#include <QVector>
#include <algorithm>
#include <omp.h>

using namespace std;

class FunctionTrans
{
public:
	FunctionTrans(Textura3d * Volumen);
	~FunctionTrans(void);

	//atributos
	//soporte a openMP
	int nroProcesadoresOpenMP;

	//La textura que contiene el Volumen principal
	Textura3d * Volumen;

	//magnitud del gradiente
	unsigned int * MagSegVolumenTextura;//mag y las normales
	unsigned char * SegVolumenTextura;//solo la seg
	double *MagVolumen;
	double MagMax,MagMin;
	double cortePixelMag;
	float *MagxValor;
	QImage * TexturaMagxValor;
	QVector3D *VectoresGrad;

	//segunda derivada direccional
	double *SegDerivada;
	double SegMax,SegMin;
	double cortePixelSeg;
	unsigned int *SegDerxValor;
	QImage * TexturaSegDerxValor;
	//sirve para seleccionar la manera de calcular la seg Derivada
	int tipoSegDerivada;
	
	//bits para la construcción de la TF	
	int bitsTFvalores;
	int bitsTFmagnitud;
	int bitsTFsegDerivada;
	int bitsTFdistZona;
	int valTFvalores;
	int valTFmagnitud;
	int valTFsegDerivada;
	int valTFdistZona;

	//Reconstrucci'on via kernels
	QVector<int> qkerInterpola;
	QVector<int> qkerPrimera;
	QVector<int> qkerSegunda;

	//los sub matrices kernels
	QVector<QVector<QVector<int>>> qkerDerX;
	QVector<QVector<QVector<int>>> qkerDerY;
	QVector<QVector<QVector<int>>> qkerDerZ;

	long long sumakerDerX;
	long long sumakerDerY;
	long long sumakerDerZ;
	//amano impar del kernel
	int tamKer;
	//entero que sirve para transformar los flotantes del kernels a int
	int divKer;
	//selecciono si uso dif centrales o Kernels
	bool selectMetodo;
	//para intercambiar entre dimensiones 1 2 o 3 dimensiones
	int nroDimensiones;
	//Se calcula las tres manera de seg der usando los kernels
	void calculaKerSegundaLaplaciano();
	void calculaKerSegundaGradDelGrad();
	void calculaKerSegundaHessian();
	//Los calculos usando Vectores
	long long qcalculaSubKer(QVector<int> *KerX,QVector<int> *KerY,QVector<int> *KerZ, QVector<QVector<QVector<int>>> *KerSal);
	void qcalculaKerPrimera();

	//carga gradiente
	int loadGradiente();
	//cargo FT 2D ValorxMagGradiente
	int loadFTValorxMagGradiente(float * matrix, int width, int height);
	//cargo la segunda derivada bueno la sencilla la cual es el laplacian
	int loadSegDerivada();
	//carga FT 2D ValorxSegDerivada
	int loadFTValorxSegDer(unsigned int * matrix, int width, int height);
	//calcula seg derivada
	inline QVector3D getGradiente(QVector3D *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);
	inline double getValSegDerivada(QVector3D *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);
	inline double getValSegDerivadaDobleGrad(QVector3D *volume1, double *volume2,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);
	inline double getValSegDerivadaHessian(QVector3D *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);
	inline int getSegDer(double *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);

	inline int getMagGradiente(double *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);

	inline int getMuestra(unsigned short int *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);

	inline double getDistGrad(unsigned short int *volume,
                          unsigned int width,unsigned int height,unsigned int depth,
                          unsigned int i,unsigned int j,unsigned int k);
	inline int getMuestra(unsigned char *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k);

	inline double getDistGrad(unsigned char *volume,
                          unsigned int width,unsigned int height,unsigned int depth,
                          unsigned int i,unsigned int j,unsigned int k);
/////Liberar toda la memoria
	void liberoMemoria();
};
