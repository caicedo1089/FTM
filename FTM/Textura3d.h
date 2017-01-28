#include <QString>
#include <QVector3D>

class Box
{
	public:
		Box(): Color(0.0f,0.0f,1.0f),Tam(0.5f,0.5f,0.5f),activo(false),mayor(0.0f){}
		Box(Box *box): Color(box->Color),Tam(box->Tam),activo(box->activo),mayor(box->mayor){}

		QVector3D Color,Tam;
		bool activo;
		float mayor;
};

class Textura3d{
public:

	Textura3d() : Tex3d(0),TexNombre(0),ancho(0),alto(0),profundidad(0),componentes(0),DSX(0.0f),DSY(0.0f),DSZ(0.0f),escalaTextura(1.0f,1.0f,1.0f),transladaTextura(0.0f,0.0f,-2.0f),verFTM(0.0f),terminarRay(0.95f),medVoxel(0.01){}

	
	unsigned char *Tex3d;
	unsigned int TexNombre;
	unsigned int ancho,alto,profundidad,componentes;
	float DSX,DSY,DSZ;
	QVector3D escalaTextura;
	QVector3D transladaTextura;
	QString NomArchivo;

	float verFTM,terminarRay,medVoxel;
};
