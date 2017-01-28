#include "pantalla.h"
#include <QVector4D>

#define MAKE_PIXEL(a,r,g,b) (a << 24) + (r << 16) + (g << 8) + b

char nTex[60]= /*"T3D/Daisy.pvm";//"T3D/Baby.pvm";//"T3D/Tooth.pvm";////"T3D/CT-Head.pvm";*/"T3D/Tooth.pvm";//Frog.pvm Engine
float multiplica;
pantalla::pantalla(QWidget *parent) 
: QGLWidget(QGLFormat(QGL::AlphaChannel),parent)
{
	//para el render
	minTex[0]=0.0f;minTex[1]=0.0f;minTex[2]=0.0f;minTex[3]=1.0f;
	maxTex[0]=1.0f;maxTex[1]=1.0f;maxTex[2]=1.0f;maxTex[3]=1.0f;
	ray_step=0.2f; //ojo calcular todo esto cuando se carge el volumen

	//inicializo id de texturas
	FTexture1d = NULL;
	FTexture3d = NULL;
	MagSegTexture3d = NULL;
	SegTexture3d = NULL;

	//inicializo el punto de luz
	pos0[0] = 0.6f;
    pos0[1] = 0.6f;
    pos0[2] = 0.6f;
    pos0[3] = 0.0f;

	Amb_color[0] = 0.0f;
    Amb_color[1] = 0.0f;
    Amb_color[2] = 0.0f;
    Amb_color[3] = 1.0f;
	
    Dif_color[0] = 1.0f;
    Dif_color[1] = 1.0f;
    Dif_color[2] = 1.0f;
    Dif_color[3] = 1.0f;
	
    Esp_color[0] = 1.0f;
    Esp_color[1] = 1.0f;
    Esp_color[2] = 1.0f;
    Esp_color[3] = 1.0f;

	moverIlumina =  false;
	opcionIlumina = 0;
	shininessIlumina = 20.0f;

	truncaFtmWidget = 255;

	fondoRender = QColor(0.0f,0.0f,0.0f,0.0f);
}

pantalla::~pantalla()
{

}
void pantalla::loadVolumenMagSeg(unsigned int * datos)
{
	glGenTextures(1, &MagSegTexture3d);	
	glBindTexture(GL_TEXTURE_3D, MagSegTexture3d);
			
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,T3d.ancho,T3d.alto,T3d.profundidad,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,datos);
	//elimino los datos
	//free(datos);
}
void pantalla::loadVolumenSeg(unsigned char * datos)
{
	glGenTextures(1, &SegTexture3d);	
	glBindTexture(GL_TEXTURE_3D, SegTexture3d);
			
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage3D(GL_TEXTURE_3D,0,GL_LUMINANCE,T3d.ancho,T3d.alto,T3d.profundidad,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,datos);
	//elimino los datos
	//free(datos);
}
void pantalla::loadVolumenFTM(unsigned char * datos)
{
	glGenTextures(1, &FTexture3d);	
	glBindTexture(GL_TEXTURE_3D, FTexture3d);
			
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,MyFT->valTFvalores,MyFT->valTFmagnitud,MyFT->valTFsegDerivada,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,datos);
	//elimino los datos
	free(datos);

}
void pantalla::updateVolumenFTM()
{
	drawFT->crearVolumenofClasificacion();
	//auto refresco
	if(drawFT->volumeFTM.size()!=0){
		unsigned char * datos = drawFT->bitsVolumenClasi();
		glBindTexture(GL_TEXTURE_3D, FTexture3d);
		glTexSubImage3D(GL_TEXTURE_3D,0,0,0,0,MyFT->valTFvalores,MyFT->valTFmagnitud,MyFT->valTFsegDerivada,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,datos);
		//elimino los datos
		free(datos);
	}
}
void pantalla::loadTexture3D(char * imgName)
{

	//cargo el volumen
	T3d.Tex3d = readPVMvolume(imgName,&T3d.ancho,&T3d.alto,&T3d.profundidad,&T3d.componentes,&T3d.DSX,&T3d.DSY,&T3d.DSZ);
	T3d.NomArchivo = QString(QLatin1String(imgName));

	//Creo la FTM
	MyFT = new FunctionTrans(&T3d);
	//Les paso a mis FTM Widget la FTM
	drawFT->volumenPinta = MyFT;
	drawFTSeg->volumenPinta = MyFT;
	//cargo el gradiente
	MyFT->loadGradiente();
	//calcula seg derivada
	MyFT->loadSegDerivada();
	//cargo las imagenes de la FT asi como los respectivos histogramas
	loadFT();
	//inicializo su box, viene en el formato del archivo pvm
	Caja.Tam.setX(T3d.DSX/2.0f);Caja.Tam.setY(T3d.DSY/2.0f);Caja.Tam.setZ(T3d.DSZ/2.0f);
	//valor del medio v'oxel
	T3d.medVoxel = 0.00001f;
	//inicializo el tamano del cubo en el cual se realiza el render
	minTex[0]=T3d.medVoxel;minTex[1]=T3d.medVoxel;minTex[2]=T3d.medVoxel;minTex[3]=1.0f;
	maxTex[0]=1.0f-T3d.medVoxel;maxTex[1]=1.0f-T3d.medVoxel;maxTex[2]=1.0f-T3d.medVoxel;maxTex[3]=1.0f;

	//ray step igual a el valor del voxel
	//ray_step = 0.001f;

	//cargo el vol'umen en la tarjeta gr'afica
	glGenTextures(1, &T3d.TexNombre);	
	glBindTexture(GL_TEXTURE_3D, T3d.TexNombre);
			
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);//para que pueda cargar texturas que no son portencias de 2
	glTexImage3D(GL_TEXTURE_3D,0,GL_LUMINANCE,T3d.ancho,T3d.alto,T3d.profundidad,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,T3d.Tex3d);

	//cargo la FTM por primera vez
	drawFT->crearVolumenofClasificacion();
	unsigned char * dat = drawFT->bitsVolumenClasi();
	loadVolumenFTM(dat);

	/*FILE * f = fopen("diente.raw","wb");
	fwrite(T3d.Tex3d,T3d.ancho*T3d.alto*T3d.profundidad,T3d.componentes,f);
	fclose(f);
	printf("Buena carga\n");*/ 
}
void pantalla::loadFT()
{
	//seteo el valor por pixel de la mag
	MyFT->cortePixelMag = (MyFT->MagMax - MyFT->MagMin)/(MyFT->valTFmagnitud-1);

	int a=0;
	if( MyFT->TexturaMagxValor==NULL && MyFT->MagxValor==NULL){
		//creo la imagen y la matrizHisto
		MyFT->TexturaMagxValor = new QImage(MyFT->valTFvalores,MyFT->valTFmagnitud,QImage::Format_ARGB32);
		MyFT->MagxValor = (float *)malloc(MyFT->valTFvalores*MyFT->valTFmagnitud*sizeof(float));
		memset(MyFT->MagxValor,0,MyFT->valTFmagnitud*MyFT->valTFvalores*4);
		//cargo la matrix de mag del volumen
		MyFT->loadFTValorxMagGradiente(MyFT->MagxValor,MyFT->valTFvalores,MyFT->valTFmagnitud);
		int tamY = (MyFT->valTFmagnitud-1);
		for(int j=0;j<MyFT->valTFmagnitud;++j){
			for(int i=0;i<MyFT->valTFvalores;++i){		
				a = (int)(MyFT->MagxValor[i+(j*MyFT->valTFvalores)]*(255.0f/(float)truncaFtmWidget));
				(a>255)? a=255: a=a;
				//pinto
				MyFT->TexturaMagxValor->setPixel(i,tamY-j,qRgba (a, a, a, 255));
			}
		}

		if( MyFT->MagMax < tamY ){
			*MyFT->TexturaMagxValor = MyFT->TexturaMagxValor->copy(0,tamY-MyFT->MagMax,MyFT->valTFvalores,MyFT->MagMax);
			*MyFT->TexturaMagxValor = MyFT->TexturaMagxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
		}

	}
	
	//seteo el valor por pixel del la seg derivada
	MyFT->cortePixelSeg = (MyFT->SegMax - MyFT->SegMin)/(MyFT->valTFsegDerivada-1);
	float distMaxSeg = abs(MyFT->SegMax - MyFT->SegMin);

	if(MyFT->SegDerxValor==NULL && MyFT->TexturaSegDerxValor==NULL){
		//creo la imagen y la matrizHisto
		MyFT->TexturaSegDerxValor = new QImage(MyFT->valTFvalores,MyFT->valTFsegDerivada,QImage::Format_ARGB32);
		MyFT->SegDerxValor = (unsigned int *)malloc(MyFT->valTFvalores*MyFT->valTFsegDerivada*sizeof(unsigned int));
		memset(MyFT->SegDerxValor,0,MyFT->valTFsegDerivada*MyFT->valTFvalores*4);
		//cargo la matrix del volumen segDeri 	
		MyFT->loadFTValorxSegDer(MyFT->SegDerxValor,MyFT->valTFvalores,MyFT->valTFsegDerivada);
		//cargo la matrix a la imagen
		int tamY = (MyFT->valTFsegDerivada-1);
		for(int j=0;j<MyFT->valTFsegDerivada;++j){
			for(int i=0;i<MyFT->valTFvalores;++i){
				a = (int)(MyFT->SegDerxValor[i+(j*MyFT->valTFvalores)]*(255.0f/(float)truncaFtmWidget));
				(a>255)? a=255: a=a;
				MyFT->TexturaSegDerxValor->setPixel(i,tamY-j,qRgba (a, a, a, 255));
			}
		}
		if( distMaxSeg < tamY){ 
			*MyFT->TexturaSegDerxValor = MyFT->TexturaSegDerxValor->copy(0,tamY-distMaxSeg,MyFT->valTFvalores,distMaxSeg);
			*MyFT->TexturaSegDerxValor = MyFT->TexturaSegDerxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
		}
	}


	//paso la imagen
	//pinto mag sel gradiente
	*(drawFT->MyImagenAct) = MyFT->TexturaMagxValor->scaled(MyFT->valTFvalores,MyFT->valTFmagnitud);
	drawFT->setFixedWidth(MyFT->valTFvalores);drawFT->setMinimumHeight(MyFT->valTFmagnitud);
	//para miimagen base
	*(drawFT->MyImagenBase) = *(drawFT->MyImagenAct);
	*(drawFT->MyImagenBaseAct) = *(drawFT->MyImagenAct);
	
	//pinto SegDeri
	*(drawFTSeg->MyImagenAct) = MyFT->TexturaSegDerxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
	drawFTSeg->setFixedWidth(MyFT->valTFvalores);drawFTSeg->setMinimumHeight(MyFT->valTFsegDerivada);
	//para miimagen base
	*(drawFTSeg->MyImagenBase) = *(drawFTSeg->MyImagenAct);
	*(drawFTSeg->MyImagenBaseAct) = *(drawFTSeg->MyImagenAct);

	//intercambio info entre los dos widget FTM
	drawFTSeg->vectorOfFig = &(drawFT->vectorOfFig);
}

void pantalla::loadFTUni(){
	glGenTextures( 1, &FTexture1d );
	glBindTexture( GL_TEXTURE_1D, FTexture1d );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, drawFTUni->texUni);
}
void pantalla::reloadFT()
{
	MyFT->cortePixelMag = (MyFT->MagMax - MyFT->MagMin)/(MyFT->valTFmagnitud-1);
	
	int a=0;
	if( MyFT->TexturaMagxValor!=NULL && MyFT->MagxValor!=NULL){
		//seteo la matriz
		memset(MyFT->MagxValor,0,MyFT->valTFmagnitud*MyFT->valTFvalores*4);
		//cargo la matriz de mag del volumen
		MyFT->loadFTValorxMagGradiente(MyFT->MagxValor,MyFT->valTFvalores,MyFT->valTFmagnitud);
		int tamY = (MyFT->valTFmagnitud-1);
		for(int j=0;j<MyFT->valTFmagnitud;++j){
			for(int i=0;i<MyFT->valTFvalores;++i){
				a = (int)(MyFT->MagxValor[i+(j*MyFT->valTFvalores)]*(255.0f/(float)truncaFtmWidget));
				(a>255)? a=255: a=a;
				MyFT->TexturaMagxValor->setPixel(i,tamY-j,qRgba(a, a, a, 255));
			}
		}
		if( MyFT->MagMax < tamY ){
			*MyFT->TexturaMagxValor = MyFT->TexturaMagxValor->copy(0,tamY-MyFT->MagMax,MyFT->valTFvalores,MyFT->MagMax);
			*MyFT->TexturaMagxValor = MyFT->TexturaMagxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
		}
	}

	//seteo el valor por pixel del la seg derivada
	MyFT->cortePixelSeg = (MyFT->SegMax - MyFT->SegMin)/(MyFT->valTFsegDerivada-1);
	float distMaxSeg = abs(MyFT->SegMax - MyFT->SegMin);

	if(MyFT->SegDerxValor!=NULL && MyFT->TexturaSegDerxValor!=NULL){
		//seteo la matriz
		memset(MyFT->SegDerxValor,0,MyFT->valTFsegDerivada*MyFT->valTFvalores*4);
		//cargo la matriz de la segDeri 	
		MyFT->loadFTValorxSegDer(MyFT->SegDerxValor,MyFT->valTFvalores,MyFT->valTFsegDerivada);
		//cargo la matrix a la imagen
		int tamY = (MyFT->valTFmagnitud-1);
		for(int j=0;j<MyFT->valTFsegDerivada;++j){
			for(int i=0;i<MyFT->valTFvalores;++i){
				a = (int)(MyFT->SegDerxValor[i+(j*MyFT->valTFvalores)]*(255.0f/(float)truncaFtmWidget));
				(a>255)? a=255: a=a;
				MyFT->TexturaSegDerxValor->setPixel(i,tamY-j,qRgba(a, a, a, 255));
			}
		}
		if( distMaxSeg < tamY ){ 
			*MyFT->TexturaSegDerxValor = MyFT->TexturaSegDerxValor->copy(0, tamY-distMaxSeg,MyFT->valTFvalores,distMaxSeg);
			*MyFT->TexturaSegDerxValor = MyFT->TexturaSegDerxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
		}
	}

	
	//paso la imagen
	//pinto mag sel gradiente
	*(drawFT->MyImagenAct) = MyFT->TexturaMagxValor->scaled(MyFT->valTFvalores,MyFT->valTFmagnitud);
	//para miimagen base
	*(drawFT->MyImagenBase) = *(drawFT->MyImagenAct);
	*(drawFT->MyImagenBaseAct) = *(drawFT->MyImagenAct);	
	
	//pinto SegDeri
	*(drawFTSeg->MyImagenAct) = MyFT->TexturaSegDerxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
	//para miimagen base
	*(drawFTSeg->MyImagenBase) = *(drawFTSeg->MyImagenAct);
	*(drawFTSeg->MyImagenBaseAct) = *(drawFTSeg->MyImagenAct);

}
void pantalla::updateHistoFTMWidget()
{
	int a=0;
	if( MyFT->TexturaMagxValor!=NULL && MyFT->MagxValor!=NULL){
		//recalculo el histograma FTM widget
		int tamY = (MyFT->valTFmagnitud-1);
		for(int j=0;j<MyFT->valTFmagnitud;++j){
			for(int i=0;i<MyFT->valTFvalores;++i){
				a = (int)(MyFT->MagxValor[i+(j*MyFT->valTFvalores)]*(255.0f/(float)truncaFtmWidget));
				(a>255)? a=255: a=a;
				MyFT->TexturaMagxValor->setPixel(i,tamY-j,qRgba(a, a, a, 255));
			}
		}
		if( MyFT->MagMax < tamY ){
			*MyFT->TexturaMagxValor = MyFT->TexturaMagxValor->copy(0,tamY-MyFT->MagMax,MyFT->valTFvalores,MyFT->MagMax);
			*MyFT->TexturaMagxValor = MyFT->TexturaMagxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
		}
	}

	//seteo el valor por pixel del la seg derivada
	float distMaxSeg = abs(MyFT->SegMax - MyFT->SegMin);
	if(MyFT->SegDerxValor!=NULL && MyFT->TexturaSegDerxValor!=NULL){
		//cargo la matrix a la imagen
		int tamY = (MyFT->valTFmagnitud-1);
		for(int j=0;j<MyFT->valTFsegDerivada;++j){
			for(int i=0;i<MyFT->valTFvalores;++i){
				a = (int)(MyFT->SegDerxValor[i+(j*MyFT->valTFvalores)]*(255.0f/(float)truncaFtmWidget));
				(a>255)? a=255: a=a;
				MyFT->TexturaSegDerxValor->setPixel(i,tamY-j,qRgba(a, a, a, 255));
			}
		}
		if( distMaxSeg < tamY ){ 
			*MyFT->TexturaSegDerxValor = MyFT->TexturaSegDerxValor->copy(0, tamY-distMaxSeg,MyFT->valTFvalores,distMaxSeg);
			*MyFT->TexturaSegDerxValor = MyFT->TexturaSegDerxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
		}
	}

	//paso la imagen
	//pinto mag sel gradiente
	*(drawFT->MyImagenAct) = MyFT->TexturaMagxValor->scaled(MyFT->valTFvalores,MyFT->valTFmagnitud);
	//para miimagen base
	*(drawFT->MyImagenBase) = *(drawFT->MyImagenAct);
	*(drawFT->MyImagenBaseAct) = *(drawFT->MyImagenAct);	
	
	//pinto SegDeri
	*(drawFTSeg->MyImagenAct) = MyFT->TexturaSegDerxValor->scaled(MyFT->valTFvalores,MyFT->valTFsegDerivada);
	//para miimagen base
	*(drawFTSeg->MyImagenBase) = *(drawFTSeg->MyImagenAct);
	*(drawFTSeg->MyImagenBaseAct) = *(drawFTSeg->MyImagenAct);
}
void pantalla::deleteTexture3D(Textura3d * miTD3)
{
	glDeleteTextures(1, &miTD3->TexNombre);
	miTD3->TexNombre = 0;
	free(miTD3->Tex3d);
	miTD3->Tex3d = 0;miTD3->ancho=0;miTD3->componentes=0;miTD3->DSX=0;miTD3->DSY=0;miTD3->DSZ;miTD3->profundidad=0;
	miTD3->alto = 0; miTD3->TexNombre=0;
}
void pantalla::initializeGL()
{
	glewInit();
	//reviso que soporte  la versi'on 2.0 de opengl
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else 
	{
		printf("OpenGL 2.0 not supported\n");
		//close();
		//enviar mensaje y salir OJOO!!!!!!
	}
	//creo el shader
	s=new Shader("vertRay.glsl","fragRay.glsl");

	//inicializo el timer
	sec=0;
	fps=0;
	m_timer = new QTimer(this);
	m_timer->setInterval(1);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	m_timer->start();	

	//Seteando a Identity para la matriz de Quaternion
	quatMatrix.m11 = 1;
	quatMatrix.m12 = 0;
	quatMatrix.m13 = 0;
	quatMatrix.m14 = 0;

	quatMatrix.m21 = 0;
	quatMatrix.m22 = 1;
	quatMatrix.m23 = 0;
	quatMatrix.m24 = 0;

	quatMatrix.m31 = 0;
	quatMatrix.m32 = 0;
	quatMatrix.m33 = 1;
	quatMatrix.m34 = 0;

	quatMatrix.m41 = 0;
	quatMatrix.m42 = 0;
	quatMatrix.m43 = 0;
	quatMatrix.m44 = 1;

	
	//ver el maximo tamaño de ña textura ojo esto lo tengoq ue revisar antes y si no cuadra tengo que enviar un mensaje y decir que no se soporta
	GLint maxrsmsize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxrsmsize);
	GLint iUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &iUnits);
	printf("Maximo valor de textura %d y maxima unidades de texturas: %d\n",maxrsmsize,iUnits);
}

void pantalla::paintGL()
{
	//verifico que esten cargados los datos
	if( FTexture1d == NULL || FTexture3d == NULL || MagSegTexture3d == NULL || SegTexture3d == NULL) return;

	//color de Fondo y limpio buffers
	glClearColor(fondoRender.redF(), fondoRender.greenF(), fondoRender.blueF(), fondoRender.alphaF());
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//translado la el volumen final y su luz
	glTranslatef(T3d.transladaTextura.x(),T3d.transladaTextura.y(),T3d.transladaTextura.z());
	//paso las luces
	configIlumina();
	//pinto el cubo de iluminaci'on
	if(opcionIlumina != 0.0f){
		static float dif = 0.02;
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
			glVertex3f(pos0[0]-dif,pos0[1]+dif,pos0[2]);
			glVertex3f(pos0[0]+dif,pos0[1]+dif,pos0[2]);
			glVertex3f(pos0[0]+dif,pos0[1]-dif,pos0[2]);
			glVertex3f(pos0[0]-dif,pos0[1]-dif,pos0[2]);
		glEnd();
		glDisable(GL_DEPTH_TEST);
	}

	//Rotacion con Quaternions
	Matrix matrix4x4;
	Quaternion qRotation;
	qRotation.CreateFromAxisAngle(PerpendMouseVec.x(), PerpendMouseVec.y(), 0, -mouseVec.length()*0.6f);
	qRotation.CreateMatrix(&matrix4x4.m11);
	
	//se acumulan las rotaciones
	quatMatrix = quatMatrix * matrix4x4;
	PerpendMouseVec.setX(0);
	PerpendMouseVec.setY(0);
	glMultMatrixf(&quatMatrix.m11);

	//Habilito el blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_DST_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	//pinto el box del volumen
	if(Caja.activo){
		if(T3d.verFTM == 0.0f){
			glEnable(GL_DEPTH_TEST);
			drawCubo(&Caja);
			glDisable(GL_DEPTH_TEST);
		}
	}
	
	//si no estan cargados los volumenes no despliego
	if(FTexture3d!=0){
		
		if(T3d.verFTM == 0.0f)
			//escalo el volumen
			glScalef(T3d.DSX,T3d.DSY,T3d.DSZ);

		//se mueve el centro para rotarlo	
		glTranslatef(-0.5,-0.5,-0.5);

		//dibujo la guias de la FTM si esta activo el FTM
		if(T3d.verFTM > 0.0f){
			glEnable(GL_DEPTH_TEST);
			drawGuiaFTM();
			glDisable(GL_DEPTH_TEST);
		}
		
		//aplico el shader
		s->applyShader();
		//ver FTM o no, al igual que terminación temprana del rayo
		s->setFloat1("opcionilumina",opcionIlumina);
		s->setFloat1("tipoftm",(float)MyFT->nroDimensiones);
		s->setFloat1("verftm",T3d.verFTM);
		s->setFloat1("terminaray",T3d.terminarRay);

		//cubo max min
		s->setVFloat("minTexCoord",minTex);
		s->setVFloat("maxTexCoord",maxTex);
		//muestreao del rayo
		s->setFloat1("ray_step",ray_step);

		//el cubo
		static float vertices[8][4]={minTex[0], maxTex[1], minTex[2], 1.0f
									,minTex[0], maxTex[1], maxTex[2], 1.0f
									,maxTex[0], maxTex[1], minTex[2], 1.0f
									,maxTex[0], maxTex[1], maxTex[2], 1.0f
									,minTex[0], minTex[1], minTex[2], 1.0f
									,minTex[0], minTex[1], maxTex[2], 1.0f
									,maxTex[0], minTex[1], minTex[2], 1.0f
									,maxTex[0], minTex[1], maxTex[2], 1.0f};
		
		printOpenglError();
		//FT unidimensional
		s->setInt1("ftuni",4);
		glActiveTextureARB(GL_TEXTURE4_ARB);
		glBindTexture(GL_TEXTURE_1D, FTexture1d);
		
		printOpenglError();
		//paso las texturas 3Ds 
		s->setInt1("seg",3);
		glActiveTextureARB(GL_TEXTURE3_ARB);
		glBindTexture(GL_TEXTURE_3D, SegTexture3d);
		s->setInt1("funcion",2);
		glActiveTextureARB(GL_TEXTURE2_ARB);
		glBindTexture(GL_TEXTURE_3D, FTexture3d);
		s->setInt1("gradseg",1);
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_3D, MagSegTexture3d);
		s->setInt1("volumen",0);
		glActiveTextureARB(GL_TEXTURE0_ARB); 
		glBindTexture(GL_TEXTURE_3D, T3d.TexNombre);
		
		printOpenglError();
		
		
		glColor4f(1,1,1,1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		printOpenglError();

		glBegin(GL_QUADS);

			 glVertex3fv(vertices[3]);
			 glVertex3fv(vertices[2]);
			 glVertex3fv(vertices[0]);
			 glVertex3fv(vertices[1]);

			 glVertex3fv(vertices[4]);
			 glVertex3fv(vertices[6]);
			 glVertex3fv(vertices[7]);
			 glVertex3fv(vertices[5]);

			 glVertex3fv(vertices[0]);
			 glVertex3fv(vertices[4]);
			 glVertex3fv(vertices[5]);
			 glVertex3fv(vertices[1]);

			 glVertex3fv(vertices[6]);
			 glVertex3fv(vertices[2]);
			 glVertex3fv(vertices[3]);
			 glVertex3fv(vertices[7]);

			 glVertex3fv(vertices[3]);
			 glVertex3fv(vertices[1]);
			 glVertex3fv(vertices[5]);
			 glVertex3fv(vertices[7]);

			 glVertex3fv(vertices[4]);
			 glVertex3fv(vertices[0]);
			 glVertex3fv(vertices[2]);
			 glVertex3fv(vertices[6]);

		glEnd();

		printOpenglError();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		
		//desactivo el shader
		s->unApplyShader();
	}

	glDisable(GL_BLEND);
	//contador FPS 
	++fps;
	const int gamesT=m_time.msecsTo(QTime::currentTime());
	const int secAct=gamesT/1000;
	if(secAct!=sec && fps>0){
		ratio=double(fps)/double(secAct-sec);
		sec=secAct;
		fps=0;	
	}

	// Display Debug Text)
	glColor3f(1.0f,1.0f,1.0f);
	QString debugDisplay=QString("FPS: ")+QString::number(ratio)+QString(" Time: ")+QString::number(gamesT/1000);
	QString TipoFTString = QString(" FT: ")+QString::number(MyFT->nroDimensiones)+QString("D");
	renderText(10,10,debugDisplay);
	renderText(width()-40,10,TipoFTString);
}
void pantalla::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(float)width/(float)height,0.1f,100.0f);
}
float pantalla::gradosARadianes(float grados)
{
	return (grados * PI) / 180.0f;
}
void pantalla::mousePressEvent(QMouseEvent *event)
{
	PosVieja = QVector2D(event->x(),event->y());

	/*float pedazo = 1.0f;
	int nroCeros;
	while(pedazo>ray_step){
		pedazo/=10;
		nroCeros++;
	}
	multiplica=10.0f;
	for(int i=1;i<((int)nroCeros/2);++i) multiplica*=10.0f;
	
	//printf("Nro de ceros: %d\n",nroCeros);
	if (event->buttons() & Qt::LeftButton) ray_step=ray_step*multiplica;*/
}
void pantalla::mouseReleaseEvent(QMouseEvent *event)
{	
	//subo resolusion cuando muevo
		//ray_step=ray_step/multiplica;
}
void pantalla::mouseMoveEvent(QMouseEvent *event)
{	
	mouseVec.setX(event->x() - PosVieja.x()); mouseVec.setY((-event->y() + PosVieja.y()));
	
	if (event->buttons() & Qt::LeftButton) {

		PerpendMouseVec.setX(cos(gradosARadianes(90)) * mouseVec.x() + sin(gradosARadianes(90)) * mouseVec.y());
		PerpendMouseVec.setY(-sin(gradosARadianes(90)) * mouseVec.x() + cos(gradosARadianes(90)) * mouseVec.y());
		PerpendMouseVec = PerpendMouseVec.normalized();

	} else if (event->buttons() & Qt::RightButton) { //con la bolita el z y con el segundo click x y
		
		
		//ojo tomar en cuenta el muestreo, decir el h porque si es muy pequeno puede no funcionar bien
		if(moverIlumina && opcionIlumina != 0.0f){
			pos0[0] = pos0[0] + (mouseVec.x()/abs(200.0f*(1.0f/pos0[2])));
			pos0[1] = pos0[1] + (mouseVec.y()/abs(200.0f*(1.0f/pos0[2])));
		}else{
			T3d.transladaTextura.setX(T3d.transladaTextura.x() + (mouseVec.x()/abs(540.0f*(1.0f/T3d.transladaTextura.z()))));
			T3d.transladaTextura.setY(T3d.transladaTextura.y() + (mouseVec.y()/abs(540.0f*(1.0f/T3d.transladaTextura.z()))));
		}

		//printf("Valor del z:%f, x:%f y:%f\n",T3d.transladaTextura.z(),T3d.transladaTextura.x(),T3d.transladaTextura.y());
	}else if(event->buttons() & Qt::MidButton){

		if(moverIlumina && opcionIlumina != 0.0f){
			pos0[2] = pos0[2] + (mouseVec.y()/100.0f);
		}else{
			T3d.transladaTextura.setZ(T3d.transladaTextura.z() + (mouseVec.y()/100.0f));
		}
	}
		
	PosVieja.setX(event->x());PosVieja.setY(event->y());
}

void pantalla::drawCubo(Box *box2)
{
	Box *box = new Box(box2);
	box->Tam+= QVector3D(0.002f,0.002f,0.002f);
	glColor3f(box->Color.x(), box->Color.y(), box->Color.z());

    glBegin(GL_LINES);

	//cara frontal
    glVertex3f(-box->Tam.x(), -box->Tam.y(),  box->Tam.z());
    glVertex3f( box->Tam.x(), -box->Tam.y(),  box->Tam.z());
    glVertex3f( box->Tam.x(),  box->Tam.y(),  box->Tam.z());
    glVertex3f(-box->Tam.x(),  box->Tam.y(),  box->Tam.z());
    //cara trasera
    glVertex3f( box->Tam.x(), -box->Tam.y(), -box->Tam.z());
    glVertex3f(-box->Tam.x(), -box->Tam.y(), -box->Tam.z());
    glVertex3f(-box->Tam.x(),  box->Tam.y(), -box->Tam.z());
    glVertex3f( box->Tam.x(),  box->Tam.y(), -box->Tam.z());
	//cara lateral izq
    glVertex3f(-box->Tam.x(),-box->Tam.y(), -box->Tam.z());
    glVertex3f(-box->Tam.x(),-box->Tam.y(),  box->Tam.z());
    glVertex3f(-box->Tam.x(), box->Tam.y(),  box->Tam.z());
    glVertex3f(-box->Tam.x(), box->Tam.y(), -box->Tam.z());
    //cara lateral dcha
    glVertex3f(box->Tam.x(), -box->Tam.y(),  box->Tam.z());
    glVertex3f(box->Tam.x(), -box->Tam.y(), -box->Tam.z());
    glVertex3f(box->Tam.x(),  box->Tam.y(), -box->Tam.z());
    glVertex3f(box->Tam.x(),  box->Tam.y(),  box->Tam.z());
    //cara arriba
    glVertex3f(-box->Tam.x(), box->Tam.y(),  box->Tam.z());
    glVertex3f( box->Tam.x(), box->Tam.y(),  box->Tam.z());
    glVertex3f( box->Tam.x(), box->Tam.y(), -box->Tam.z());
    glVertex3f(-box->Tam.x(), box->Tam.y(), -box->Tam.z());
    //cara abajo
    glVertex3f( box->Tam.x(),-box->Tam.y(), -box->Tam.z());
    glVertex3f( box->Tam.x(),-box->Tam.y(),  box->Tam.z());
    glVertex3f(-box->Tam.x(),-box->Tam.y(),  box->Tam.z());
    glVertex3f(-box->Tam.x(),-box->Tam.y(), -box->Tam.z());
	//uniones de las lineas
	glVertex3f( box->Tam.x(),  box->Tam.y(),  box->Tam.z());
    glVertex3f( box->Tam.x(), -box->Tam.y(),  box->Tam.z());
    glVertex3f(-box->Tam.x(), -box->Tam.y(),  box->Tam.z());
    glVertex3f(-box->Tam.x(),  box->Tam.y(),  box->Tam.z());

	glVertex3f( box->Tam.x(),box->Tam.y(), -box->Tam.z());
    glVertex3f( box->Tam.x(),-box->Tam.y(),  -box->Tam.z());
    glVertex3f(-box->Tam.x(),-box->Tam.y(),  -box->Tam.z());
    glVertex3f(-box->Tam.x(),box->Tam.y(), -box->Tam.z());
    
	glEnd();
}
void pantalla::drawGuiaFTM()
{
	glColor3f(ejeX->redF(),ejeX->greenF(),ejeX->blueF());
	glBegin(GL_LINES);
	glVertex3f(-0.003f,-0.003f,1.003f);
	glVertex3f(1.003f,-0.003f,1.003f);
	glEnd();
	glColor3f(ejeY->redF(),ejeY->greenF(),ejeY->blueF());
	glBegin(GL_LINES);
	glVertex3f(-0.003f,-0.003f,1.003f);
	glVertex3f(-0.003f,1.003f,1.003f);
	glEnd();
	glColor3f(ejeZ->redF(),ejeZ->greenF(),ejeZ->blueF());
	glBegin(GL_LINES);
	glVertex3f(-0.003f,-0.003f,1.003f);
	glVertex3f(-0.003f,-0.003f,-0.003f);
	glEnd();
}
void pantalla::printOpenglError()
{
	GLuint errnum;
	const char *errstr;
	while (errnum = glGetError())
	{
		errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
		printf("ERROR GL: %s\n",errstr );
	}
}

void pantalla::configIlumina()
{
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Amb_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dif_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Esp_color);

	glEnable(GL_COLOR_MATERIAL);
	/*glMaterialfv(GL_FRONT, GL_AMBIENT, Amb_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Dif_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, Esp_color);*/
    glMaterialf(GL_FRONT, GL_SHININESS, shininessIlumina);
}
void pantalla::reInicializoTodo()
{
	//El timer
	sec=0;
	fps=0;
	m_timer->start();
	//volumen
	glDeleteTextures(1, &T3d.TexNombre);//elimino la textura de la TG
	T3d.Tex3d = 0; T3d.TexNombre = 0; T3d.ancho = 0; T3d.alto = 0; T3d.profundidad = 0; T3d.componentes = 0; T3d.DSX = 0.0f; T3d.DSY = 0.0f; T3d.DSZ = 0.0f; T3d.escalaTextura = QVector3D(1.0f,1.0f,1.0f); T3d.transladaTextura = QVector3D(0.0f,0.0f,-2.0f); T3d.verFTM = 0.0f; T3d.terminarRay = 0.95f; T3d.medVoxel = 0.01f;
	//box o caja
	Caja.Color = QVector3D(0.0f,0.0f,1.0f);Caja.Tam = QVector3D(0.5f,0.5f,0.5f); Caja.activo = false; Caja.mayor = 0.0f;

	//rotación
	quatMatrix.m11 = 1; quatMatrix.m21 = 0; quatMatrix.m31 = 0; quatMatrix.m41 = 0;
	quatMatrix.m12 = 0; quatMatrix.m22 = 1; quatMatrix.m32 = 0; quatMatrix.m42 = 0;
	quatMatrix.m13 = 0; quatMatrix.m23 = 0; quatMatrix.m33 = 1; quatMatrix.m43 = 0;
	quatMatrix.m14 = 0; quatMatrix.m24 = 0; quatMatrix.m34 = 0; quatMatrix.m44 = 1;

	//shader
	//Shader *s; el shader es el mismo por eso solo se inicializa una vez
	
	//función de transferencia
	//FunctionTrans * MyFT;
	drawFT->volumenPinta = 0;
	drawFTSeg->volumenPinta = 0;
	MyFT->liberoMemoria();
	delete MyFT;

	//pintaFT * drawFT;
	//pintaFTSeg * drawFTSeg;
	//pintaFTUni * drawFTUni;
	truncaFtmWidget = 255;
	
	
	//float ray_step;
	glDeleteTextures(1, &FTexture1d);//elimino la textura de la TG
	FTexture1d = 0;
	glDeleteTextures(1, &FTexture3d);//elimino la textura de la TG
	FTexture3d = 0;
	glDeleteTextures(1, &MagSegTexture3d);//elimino la textura de la TG
	MagSegTexture3d = 0;
	glDeleteTextures(1, &SegTexture3d);//elimino la textura de la TG
	SegTexture3d = 0;

	//iluminaci'on
	pos0[0] = 0.6f;
    pos0[1] = 0.6f;
    pos0[2] = 0.6f;
    pos0[3] = 0.0f;

	Amb_color[0] = 0.0f;
    Amb_color[1] = 0.0f;
    Amb_color[2] = 0.0f;
    Amb_color[3] = 1.0f;
	
    Dif_color[0] = 1.0f;
    Dif_color[1] = 1.0f;
    Dif_color[2] = 1.0f;
    Dif_color[3] = 1.0f;
	
    Esp_color[0] = 1.0f;
    Esp_color[1] = 1.0f;
    Esp_color[2] = 1.0f;
    Esp_color[3] = 1.0f;

	moverIlumina =  false;
	opcionIlumina = 0;
	shininessIlumina = 20.0f;
}