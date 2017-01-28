#ifndef PANTALLA_H
#define PANTALLA_H
//para abrir pvm
#include "ddsbase.h" 
//usar extenciones opengl
#include "glew/glew.h"
//matemática no hace falta por ddsbase
//#include <math.h>
//usar quaternios para rotación
#include "quaternion.h"
#include "matrix.h"
//usar función de transferencia
#include "FunctionTrans.h"
#include "pintaFT.h"
#include "pintaFTSeg.h"
#include "pintaFTUni.h"
#include "colorLabel.h"

//shaders
#include "shader.h"

#pragma comment(lib, "glew/glew32.lib")

#include <QtGui>
#include <QGLWidget>
#include <QString>
#include <QVector3D>
#include <QVector2D>

using namespace std;

class pantalla : public QGLWidget
{
	Q_OBJECT

public:
	pantalla(QWidget *parent);
	~pantalla();
	// variables para tiempo y fps
	QTimer *m_timer;
	QTime m_time;
	float sec,fps;
	double ratio;
	//volumen
	Textura3d T3d;
	//box o caja
	Box Caja;
	//rotación
	QVector2D PosVieja;
	QVector2D PerpendMouseVec,mouseVec;
	Quaternion Rotacion;
	Matrix quatMatrix;

	//shader
	Shader *s;
	
	//función de transferencia
	FunctionTrans * MyFT;
	pintaFT * drawFT;
	pintaFTSeg * drawFTSeg;
	pintaFTUni * drawFTUni;
	colorLabel * ejeValorColor;
	colorLabel * ejeMagColor;
	colorLabel * ejeSegDeriColor;
	long truncaFtmWidget;
	
	//color de cubo FTM
	QColor *ejeX;
	QColor *ejeY;
	QColor *ejeZ;
	//el fondo del render
	QColor fondoRender;

	//cubo del render etc...
	float minTex[4];
	float maxTex[4];
	float ray_step;
	//ids de las texturas
	GLuint FTexture1d;
	GLuint FTexture3d;
	GLuint MagSegTexture3d;
	GLuint SegTexture3d;

	//iluminaci'on
	GLfloat pos0[4];
	GLfloat Amb_color[4];
    GLfloat Dif_color[4];
    GLfloat Esp_color[4];
	bool moverIlumina;
	int opcionIlumina;
	float shininessIlumina;

	//cargar función de transferencia
	void loadFT();
	//recargar la seg y la primera derivada
	void reloadFT();
	
	void loadVolumenMagSeg(unsigned int * datos);
	void loadVolumenSeg(unsigned char * datos);
	void loadVolumenFTM(unsigned char * datos);
	//actulizar los histo FTM widget
	void updateHistoFTMWidget();

	void loadTexture3D(char * imgName);
	void deleteTexture3D(Textura3d * miTD3);
	float gradosARadianes(float grados);

	//Pinta las lineas guias o el bounding box
	void drawCubo(Box *box);
	void drawGuiaFTM();

	//procedimiento para ver errores de opengl
	void printOpenglError();

	//iluminaci'on
	void configIlumina();

	//Re Inicializo todo
	void reInicializoTodo();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

public slots:

	//actualizar, esta como slot para poder accederlo desde mis drawFTM
	void updateVolumenFTM();
	void loadFTUni();
	
};

#endif // PANTALLA_H
