
#include "FunctionTrans.h" 

#define MAKE_PIXEL(a,r,g,b) (a << 24) + (r << 16) + (g << 8) + b

FunctionTrans::FunctionTrans(Textura3d * Volumen)
{
	this->Volumen = Volumen;
	bitsTFvalores = 8;
	bitsTFmagnitud = 8;
	bitsTFsegDerivada = 8;
	bitsTFdistZona = 8;
	valTFvalores = 1 << bitsTFvalores;
	valTFmagnitud = 1 << bitsTFmagnitud;
	valTFsegDerivada = 1 << bitsTFsegDerivada;
	valTFdistZona = 1 << bitsTFdistZona;
	
	MagxValor = NULL;SegDerxValor = NULL;
	MagSegVolumenTextura = NULL;
	TexturaSegDerxValor = TexturaMagxValor = NULL;
	SegVolumenTextura = NULL;

	MagVolumen = SegDerivada = NULL;
	VectoresGrad = NULL;

	//con vector dinamico
	qkerInterpola.push_back(0);qkerInterpola.push_back(111);qkerInterpola.push_back(388);qkerInterpola.push_back(500);qkerInterpola.push_back(388);qkerInterpola.push_back(111);qkerInterpola.push_back(0);
	qkerPrimera.push_back(0);qkerPrimera.push_back(222);qkerPrimera.push_back(666);qkerPrimera.push_back(0);qkerPrimera.push_back(-666);qkerPrimera.push_back(-222);qkerPrimera.push_back(0);
	//ojo falta kernel de segunda

	//tamaño del kernel
	tamKer = 7;
	divKer = 1000;

	//selecciono el metodo para calcular los datos
	selectMetodo = false;
	//dimension seleccionada
	nroDimensiones = 3;

	//el tipo de segunda derivada seleccionada para obtener la medida
	tipoSegDerivada = 1;

	//se obtiene la cantidad de procesadores
	nroProcesadoresOpenMP = omp_get_num_procs();
	//se activa la cantidad de hilos a usar
	omp_set_num_threads(nroProcesadoresOpenMP);
}

FunctionTrans::~FunctionTrans(void)
{
}

int FunctionTrans::loadGradiente() //listo
{
		if(Volumen == NULL) return NULL;
		//
		double mayor=0,menor=0;
		if(selectMetodo){
			//metodo del kernels
			qcalculaKerPrimera();

		}else{
			if(MagVolumen==NULL && (MagVolumen=(double *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(double)))==NULL) return NULL;
			if(VectoresGrad==NULL && (VectoresGrad=(QVector3D *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(QVector3D)))==NULL) return NULL;
			MagMax = 0.0f; MagMin = 65536.0f;
			int indPos;
			//variables auxiliares para la coherencia al utilizar OpenMP
			QVector<double> menores(nroProcesadoresOpenMP,65536.0f);
			QVector<double> mayores(nroProcesadoresOpenMP,0.0f);
			int id;
			#pragma omp parallel private(id,indPos)
			{
				id = omp_get_thread_num();
			#pragma omp for nowait
				for(int k=0; k<Volumen->profundidad ; ++k){
					for(int j=0; j<Volumen->alto ; ++j){
						for(int i=0; i<Volumen->ancho ; ++i){
							indPos = i+(j+k*Volumen->alto)*Volumen->ancho;
							MagVolumen[indPos] = getDistGrad(Volumen->Tex3d,Volumen->ancho,Volumen->alto,Volumen->profundidad,i,j,k);
							if( mayores[id]<MagVolumen[indPos] ) mayores[id] = MagVolumen[indPos];
							if( menores[id]>MagVolumen[indPos] )  menores[id] = MagVolumen[indPos];

						}
					}
				}
			}
				//Hay que obtener el menor de menores y el mayor de mayores
				for(int i=0;i<nroProcesadoresOpenMP;++i){
					MagMax = max(mayores[i],MagMax);
					MagMin = min(menores[i],MagMin);
				}

		}
		
		return 1;
}
int FunctionTrans::loadFTValorxMagGradiente(float * matrix, int width, int height)//listo
{
	if(matrix==NULL) return NULL;

	int indpixelMag;
	float indpixelMagf;

	if(MagSegVolumenTextura==NULL && (MagSegVolumenTextura=(unsigned int *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(unsigned int)))==NULL) return NULL;
	if(SegVolumenTextura==NULL && (SegVolumenTextura=(unsigned char *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(unsigned char)))==NULL) return NULL;
	
	int indPos;
#pragma omp parallel private(indPos,indpixelMagf,indpixelMag)
	{
	#pragma omp for nowait
	for(int k=0; k<Volumen->profundidad ; ++k){
		//printf("nro de proc: %d\n",omp_get_thread_num());
			for(int j=0; j<Volumen->alto ; ++j){
				for(int i=0; i<Volumen->ancho ; ++i){
					indPos = i+(j+k*Volumen->alto)*Volumen->ancho;
					indpixelMagf = ((MagVolumen[indPos] + 0.5f)/cortePixelMag);
					// por si sobrepasa el intervalo
					if(cortePixelMag<=1.0f)
						indpixelMag = (int)(MagVolumen[indPos] + 0.5f);
					else
						indpixelMag = indpixelMagf;
					//almaceno en la textura 3D la Mag del Grad
					MagSegVolumenTextura[indPos] = MAKE_PIXEL((int)indpixelMagf,0,0,0);
					//Acumulo los valores en la matriz
					matrix[ Volumen->Tex3d[indPos] + indpixelMag*width ]++;
				}
			}
		}
	}
		return 1;
}
int FunctionTrans::loadSegDerivada()
{
	if(Volumen == NULL || VectoresGrad == NULL) return NULL;

	if(selectMetodo){
			//metodo de los kernels
			//con los diversos tipos de forma de calcular
			switch(tipoSegDerivada){
					case 0:
						calculaKerSegundaLaplaciano();
						break;
					case 1:
						calculaKerSegundaGradDelGrad();
						break;
					case 2:
						calculaKerSegundaHessian();
						break;
					}
	}else{
		if(SegDerivada==NULL && (SegDerivada=(double *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(double)))==NULL) return NULL;
		SegMax = 0.0f; SegMin = 65536.0f;
		int indPos;
		QVector<double> menores(nroProcesadoresOpenMP,65536.0f);
		QVector<double> mayores(nroProcesadoresOpenMP,0.0f);
		int id;
		#pragma omp parallel private(indPos)
		{
			id = omp_get_thread_num();
			#pragma omp for nowait
			for(int k=0; k<Volumen->profundidad ; ++k){
				//printf("nro de proc: %d\n",omp_get_thread_num());
				for(int j=0; j<Volumen->alto ; ++j){
					for(int i=0; i<Volumen->ancho ; ++i){
						indPos = i+(j+k*Volumen->alto)*Volumen->ancho;

						switch(tipoSegDerivada){
						case 0:
							SegDerivada[indPos] = getValSegDerivada(VectoresGrad,Volumen->ancho,Volumen->alto,Volumen->profundidad,i,j,k);
							break;
						case 1:
							SegDerivada[indPos] = getValSegDerivadaDobleGrad(VectoresGrad,MagVolumen,Volumen->ancho,Volumen->alto,Volumen->profundidad,i,j,k);
							break;
						case 2:
							SegDerivada[indPos] = getValSegDerivadaHessian(VectoresGrad,Volumen->ancho,Volumen->alto,Volumen->profundidad,i,j,k);
							break;
						}

						if( mayores[id]<SegDerivada[indPos] ) mayores[id] = SegDerivada[indPos];
						if( menores[id]>SegDerivada[indPos] ) menores[id] = SegDerivada[indPos];
					}
				}
			}
		}
		//busco el mayor y menor
		for(int i=0;i<nroProcesadoresOpenMP;++i){
			SegMax = max(mayores[i],SegMax);
			SegMin = min(menores[i],SegMin);
		}
	}
		return 1;
}

int FunctionTrans::loadFTValorxSegDer(unsigned int * matrix, int width, int height)//listo
{
	if(matrix==NULL) return NULL;                                         
	
	int indpixelSeg, indPos;
	float indpixelSegf;
	QVector3D auxVector;

#pragma omp parallel private(indpixelSeg,indPos,indpixelSegf,auxVector)
	{
	#pragma omp for nowait
	for(int k=0; k<Volumen->profundidad ; ++k){
			//printf("nro de proc: %d\n",omp_get_thread_num());
			for(int j=0; j<Volumen->alto ; ++j){
				for(int i=0; i<Volumen->ancho ; ++i){
					indPos = i+(j+k*Volumen->alto)*Volumen->ancho;
					indpixelSegf = ( (SegDerivada[indPos] - SegMin + 0.5f) / cortePixelSeg );
					//por si sobrepasa el rango
					if( cortePixelSeg <= 1.0f )
						indpixelSeg = (SegDerivada[indPos] - SegMin + 0.5f);
					else 
						indpixelSeg = indpixelSegf;


					SegVolumenTextura[indPos] = 255 - (int)indpixelSegf;
					matrix[ Volumen->Tex3d[indPos] + (indpixelSeg*width) ]++;

					//voy a normalizar y transformo a coordenadas 0 1
					auxVector = VectoresGrad[indPos];
					auxVector.normalize();
					auxVector = (auxVector + QVector3D(1.0f,1.0f,1.0f)) / 2.0f;

					int x =(int)255*auxVector.x(), y=(int)255*auxVector.y(), z=(int)255*auxVector.z();
					//le paso los datos a la textura
					MagSegVolumenTextura[indPos] = (MagSegVolumenTextura[indPos]) + (x << 16) + (y << 8) + z;

				}
			}
		}
	}
		return 1;
}
inline int FunctionTrans::getSegDer(double *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k)
   {return(volume[i+(j+k*height)*width]);}
inline QVector3D FunctionTrans::getGradiente(QVector3D *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k)
{return(volume[i+(j+k*height)*width]);}
inline double FunctionTrans::getValSegDerivada(QVector3D *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k)
{
	double gx,gy,gz;

   if (i>0)
      if (i<width-1) gx=(getGradiente(volume,width,height,depth,i+1,j,k).x()-getGradiente(volume,width,height,depth,i-1,j,k).x())/2.0;
      else gx=getGradiente(volume,width,height,depth,i,j,k).x()-getGradiente(volume,width,height,depth,i-1,j,k).x();
   else gx=getGradiente(volume,width,height,depth,i+1,j,k).x()-getGradiente(volume,width,height,depth,i,j,k).x();

   if (j>0)
      if (j<height-1) gy=(getGradiente(volume,width,height,depth,i,j+1,k).y()-getGradiente(volume,width,height,depth,i,j-1,k).y())/2.0;
      else gy=getGradiente(volume,width,height,depth,i,j,k).y()-getGradiente(volume,width,height,depth,i,j-1,k).y();
   else gy=getGradiente(volume,width,height,depth,i,j+1,k).y()-getGradiente(volume,width,height,depth,i,j,k).y();

   if (k>0)
      if (k<depth-1) gz=(getGradiente(volume,width,height,depth,i,j,k+1).z()-getGradiente(volume,width,height,depth,i,j,k-1).z())/2.0;
      else gz=getGradiente(volume,width,height,depth,i,j,k).z()-getGradiente(volume,width,height,depth,i,j,k-1).z();
   else gz=getGradiente(volume,width,height,depth,i,j,k+1).z()-getGradiente(volume,width,height,depth,i,j,k).z();

   return(gx+gy+gz);
}
inline double FunctionTrans::getValSegDerivadaHessian(QVector3D *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k)
{
	double gx,gy,gz,gxy,gxz,gyz;

   if (i>0)
      if (i<width-1) gx=(getGradiente(volume,width,height,depth,i+1,j,k).x()-getGradiente(volume,width,height,depth,i-1,j,k).x())/2.0;
      else gx=getGradiente(volume,width,height,depth,i,j,k).x()-getGradiente(volume,width,height,depth,i-1,j,k).x();
   else gx=getGradiente(volume,width,height,depth,i+1,j,k).x()-getGradiente(volume,width,height,depth,i,j,k).x();

   if (j>0)
      if (j<height-1) gy=(getGradiente(volume,width,height,depth,i,j+1,k).y()-getGradiente(volume,width,height,depth,i,j-1,k).y())/2.0;
      else gy=getGradiente(volume,width,height,depth,i,j,k).y()-getGradiente(volume,width,height,depth,i,j-1,k).y();
   else gy=getGradiente(volume,width,height,depth,i,j+1,k).y()-getGradiente(volume,width,height,depth,i,j,k).y();

   if (k>0)
      if (k<depth-1) gz=(getGradiente(volume,width,height,depth,i,j,k+1).z()-getGradiente(volume,width,height,depth,i,j,k-1).z())/2.0;
      else gz=getGradiente(volume,width,height,depth,i,j,k).z()-getGradiente(volume,width,height,depth,i,j,k-1).z();
   else gz=getGradiente(volume,width,height,depth,i,j,k+1).z()-getGradiente(volume,width,height,depth,i,j,k).z();
	//d xy
   if (i>0)
      if (i<width-1) gxy=(getGradiente(volume,width,height,depth,i+1,j,k).y()-getGradiente(volume,width,height,depth,i-1,j,k).y())/2.0;
      else gxy=getGradiente(volume,width,height,depth,i,j,k).y()-getGradiente(volume,width,height,depth,i-1,j,k).y();
   else gxy=getGradiente(volume,width,height,depth,i+1,j,k).y()-getGradiente(volume,width,height,depth,i,j,k).y();
   //d xz
   if (i>0)
      if (i<width-1) gxz=(getGradiente(volume,width,height,depth,i+1,j,k).z()-getGradiente(volume,width,height,depth,i-1,j,k).z())/2.0;
      else gxz=getGradiente(volume,width,height,depth,i,j,k).z()-getGradiente(volume,width,height,depth,i-1,j,k).z();
   else gxz=getGradiente(volume,width,height,depth,i+1,j,k).z()-getGradiente(volume,width,height,depth,i,j,k).z();
	//d yz
   if (j>0)
      if (j<height-1) gyz=(getGradiente(volume,width,height,depth,i,j+1,k).z()-getGradiente(volume,width,height,depth,i,j-1,k).z())/2.0;
      else gyz=getGradiente(volume,width,height,depth,i,j,k).z()-getGradiente(volume,width,height,depth,i,j-1,k).z();
   else gyz=getGradiente(volume,width,height,depth,i,j+1,k).z()-getGradiente(volume,width,height,depth,i,j,k).z();

	if(MagVolumen[i+(j+k*height)*width] == 0.0f) return 0.0f;

   double auxX = (getGradiente(volume,width,height,depth,i,j,k).x()*gx + getGradiente(volume,width,height,depth,i,j,k).y()*gxy + getGradiente(volume,width,height,depth,i,j,k).z()*gxz);
   double auxY = (getGradiente(volume,width,height,depth,i,j,k).x()*gxy + getGradiente(volume,width,height,depth,i,j,k).y()*gy + getGradiente(volume,width,height,depth,i,j,k).z()*gyz);
   double auxZ = (getGradiente(volume,width,height,depth,i,j,k).x()*gxz + getGradiente(volume,width,height,depth,i,j,k).y()*gyz + getGradiente(volume,width,height,depth,i,j,k).z()*gz);
   
   return ((1.0f/(MagVolumen[i+(j+k*height)*width]*MagVolumen[i+(j+k*height)*width])) * (auxX*getGradiente(volume,width,height,depth,i,j,k).x()+auxY*getGradiente(volume,width,height,depth,i,j,k).y()+auxZ*getGradiente(volume,width,height,depth,i,j,k).z()));
}
inline double FunctionTrans::getValSegDerivadaDobleGrad(QVector3D *volume1,double *volume2,
                   unsigned int width,unsigned int height,unsigned int depth,
				   unsigned int i,unsigned int j,unsigned int k)
{
	double gx,gy,gz;

   if (i>0)
      if (i<width-1) gx=(getMagGradiente(volume2,width,height,depth,i+1,j,k)-getMagGradiente(volume2,width,height,depth,i-1,j,k))/2.0;
      else gx=getMagGradiente(volume2,width,height,depth,i,j,k)-getMagGradiente(volume2,width,height,depth,i-1,j,k);
   else gx=getMagGradiente(volume2,width,height,depth,i+1,j,k)-getMagGradiente(volume2,width,height,depth,i,j,k);

   if (j>0)
      if (j<height-1) gy=(getMagGradiente(volume2,width,height,depth,i,j+1,k)-getMagGradiente(volume2,width,height,depth,i,j-1,k))/2.0;
      else gy=getMagGradiente(volume2,width,height,depth,i,j,k)-getMagGradiente(volume2,width,height,depth,i,j-1,k);
   else gy=getMagGradiente(volume2,width,height,depth,i,j+1,k)-getMagGradiente(volume2,width,height,depth,i,j,k);

   if (k>0)
      if (k<depth-1) gz=(getMagGradiente(volume2,width,height,depth,i,j,k+1)-getMagGradiente(volume2,width,height,depth,i,j,k-1))/2.0;
      else gz=getMagGradiente(volume2,width,height,depth,i,j,k)-getMagGradiente(volume2,width,height,depth,i,j,k-1);
   else gz=getMagGradiente(volume2,width,height,depth,i,j,k+1)-getMagGradiente(volume2,width,height,depth,i,j,k);

   double aux = gx*volume1[i+(j+k*height)*width].x() + gy*volume1[i+(j+k*height)*width].y() + gz*volume1[i+(j+k*height)*width].z(); 

	if(aux == 0.0f || MagVolumen[i+(j+k*height)*width] == 0) return 0.0f;
   return (1.0f/MagVolumen[i+(j+k*height)*width]) * aux;
}

inline int FunctionTrans::getMagGradiente(double *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k)
   {return(volume[i+(j+k*height)*width]);}

inline int FunctionTrans::getMuestra(unsigned short int  *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k)
   {return(volume[i+(j+k*height)*width]);}

inline double FunctionTrans::getDistGrad(unsigned short int *volume,
                          unsigned int width,unsigned int height,unsigned int depth,
                          unsigned int i,unsigned int j,unsigned int k)
{
   double gx,gy,gz;

   if (i>0)
      if (i<width-1) gx=(getMuestra(volume,width,height,depth,i+1,j,k)-getMuestra(volume,width,height,depth,i-1,j,k))/2.0;
      else gx=getMuestra(volume,width,height,depth,i,j,k)-getMuestra(volume,width,height,depth,i-1,j,k);
   else gx=getMuestra(volume,width,height,depth,i+1,j,k)-getMuestra(volume,width,height,depth,i,j,k);

   if (j>0)
      if (j<height-1) gy=(getMuestra(volume,width,height,depth,i,j+1,k)-getMuestra(volume,width,height,depth,i,j-1,k))/2.0;
      else gy=getMuestra(volume,width,height,depth,i,j,k)-getMuestra(volume,width,height,depth,i,j-1,k);
   else gy=getMuestra(volume,width,height,depth,i,j+1,k)-getMuestra(volume,width,height,depth,i,j,k);

   if (k>0)
      if (k<depth-1) gz=(getMuestra(volume,width,height,depth,i,j,k+1)-getMuestra(volume,width,height,depth,i,j,k-1))/2.0;
      else gz=getMuestra(volume,width,height,depth,i,j,k)-getMuestra(volume,width,height,depth,i,j,k-1);
   else gz=getMuestra(volume,width,height,depth,i,j,k+1)-getMuestra(volume,width,height,depth,i,j,k);

   return(sqrt(gx*gx+gy*gy+gz*gz));
}

inline int FunctionTrans::getMuestra(unsigned char *volume,
                   unsigned int width,unsigned int height,unsigned int depth,
                   unsigned int i,unsigned int j,unsigned int k)
   {return(volume[i+(j+k*height)*width]);}

inline double FunctionTrans::getDistGrad(unsigned char *volume,
                          unsigned int width,unsigned int height,unsigned int depth,
                          unsigned int i,unsigned int j,unsigned int k)
   {
   double gx,gy,gz;

   if (i>0)
      if (i<width-1) gx=(getMuestra(volume,width,height,depth,i+1,j,k)-getMuestra(volume,width,height,depth,i-1,j,k))/2.0;
      else gx=getMuestra(volume,width,height,depth,i,j,k)-getMuestra(volume,width,height,depth,i-1,j,k);
   else gx=getMuestra(volume,width,height,depth,i+1,j,k)-getMuestra(volume,width,height,depth,i,j,k);

   if (j>0)
      if (j<height-1) gy=(getMuestra(volume,width,height,depth,i,j+1,k)-getMuestra(volume,width,height,depth,i,j-1,k))/2.0;
      else gy=getMuestra(volume,width,height,depth,i,j,k)-getMuestra(volume,width,height,depth,i,j-1,k);
   else gy=getMuestra(volume,width,height,depth,i,j+1,k)-getMuestra(volume,width,height,depth,i,j,k);

   if (k>0)
      if (k<depth-1) gz=(getMuestra(volume,width,height,depth,i,j,k+1)-getMuestra(volume,width,height,depth,i,j,k-1))/2.0;
      else gz=getMuestra(volume,width,height,depth,i,j,k)-getMuestra(volume,width,height,depth,i,j,k-1);
   else gz=getMuestra(volume,width,height,depth,i,j,k+1)-getMuestra(volume,width,height,depth,i,j,k);

   VectoresGrad[i+(j+k*height)*width].setX(gx);VectoresGrad[i+(j+k*height)*width].setY(gy);VectoresGrad[i+(j+k*height)*width].setZ(gz);

   return (sqrt(gx*gx+gy*gy+gz*gz));
}

long long FunctionTrans::qcalculaSubKer(QVector<int> *KerX,QVector<int> *KerY,QVector<int> *KerZ, QVector<QVector<QVector<int>>> *KerSal)
{
	long long acumulaSuma = 0;
	//limpio la matrix
	KerSal->clear();
	//inicilaliza el kernel volumen
	for(int o=0;o<KerZ->size();++o){
		QVector<QVector<int>> aux1;
		KerSal->push_back( aux1 );
		for(int p=0;p<KerY->size();++p){
			QVector<int> aux2;
			(*KerSal)[o].push_back( aux2 );
			for(int q=0;q<KerX->size();++q){
				(*KerSal)[o][p].push_back(0);
			}
		}
	}
	#pragma omp parallel 
	{
		#pragma omp for reduction(+:acumulaSuma)
		for(int o=0;o<KerZ->size();++o){
			
			for(int p=0;p<KerY->size();++p){
				
				for(int q=0;q<KerX->size();++q){
					(*KerSal)[q][p][o] = (*KerX)[q]*(*KerY)[p]*(*KerZ)[o];
					acumulaSuma += abs((long) ((*KerSal)[q][p][o]));
				}
			}
		}
	}
			return acumulaSuma;
}
void FunctionTrans::qcalculaKerPrimera()//listo
{
	//obtengo los tres kernel para la primera derivada
	//es decir las derivadas parciales con respecto a x, y e z

	//primera con respecto a x
	sumakerDerX = qcalculaSubKer(&qkerPrimera,&qkerInterpola,&qkerInterpola,&qkerDerX);

	//primera con respecto a y
	sumakerDerY = qcalculaSubKer(&qkerInterpola,&qkerPrimera,&qkerInterpola,&qkerDerY);

	//primera con respecto a z
	sumakerDerZ = qcalculaSubKer(&qkerInterpola,&qkerInterpola,&qkerPrimera,&qkerDerZ);

	//creo mis vectores gradientes con el kernel
	if(VectoresGrad==NULL && (VectoresGrad=(QVector3D *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(QVector3D)))==NULL) return;
	if(MagVolumen==NULL && (MagVolumen=(double *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(double)))==NULL) return;
	
	MagMax = 0.0f; MagMin = 65536.0f;
	double gx,gy,gz;
	int tamDif1 = qkerInterpola.size()/2, tamDif2 = tamDif1+1;
	int indPos,qMi,pMj,oMk,muestra;

	QVector<double> menores(nroProcesadoresOpenMP,65536.0f);
	QVector<double> mayores(nroProcesadoresOpenMP,0.0f);
	int id;
	#pragma omp parallel private(gx,gy,gz,indPos,qMi,pMj,oMk,muestra,id)
	{
		id = omp_get_thread_num();
		#pragma omp for nowait
		for(int k=0; k<Volumen->profundidad ; ++k){
				for(int j=0; j<Volumen->alto ; ++j){
					for(int i=0; i<Volumen->ancho ; ++i){
						gx=gy=gz=0;
						
						//computo kernel
						for(int o=(k-tamDif1);o<(k+tamDif2);++o)
						{
							for(int p=(j-tamDif1);p<(j+tamDif2);++p)
							{
								for(int q=(i-tamDif1);q<i+tamDif2;(++q))
								{
									//si tiene valores validos computo el gradiente
									if(o>=0 && p>=0 && q>=0 && o<Volumen->profundidad && p<Volumen->alto && q<Volumen->ancho)
									{
										muestra = getMuestra(Volumen->Tex3d,Volumen->ancho,Volumen->alto,Volumen->profundidad,q,p,o);
										qMi = q-i+tamDif1; pMj = p-j+tamDif1; oMk = o-k+tamDif1;
										//para la componente X del vector gradiente
										gx+=((double)muestra*(double)qkerDerX[qMi][pMj][oMk]);
										gy+=((double)muestra*(double)qkerDerY[qMi][pMj][oMk]);
										gz+=((double)muestra*(double)qkerDerZ[qMi][pMj][oMk]);

									}
						
								}
							
							}
						
						}

						if(sumakerDerX!=0)
							gx/= sumakerDerX;
						if(sumakerDerY!=0)
							gy/= sumakerDerY;
						if(sumakerDerZ!=0)
							gz/= sumakerDerZ;

						indPos = i+(j+k*Volumen->alto)*Volumen->ancho;

						VectoresGrad[indPos].setX(gx);VectoresGrad[indPos].setY(gy);VectoresGrad[indPos].setZ(gz);
						MagVolumen[indPos] = (sqrt(gx*gx+gy*gy+gz*gz));

						//escojo los m'inimos y max
						if( mayores[id]<MagVolumen[indPos] ) mayores[id] = MagVolumen[indPos];
						if( menores[id]>MagVolumen[indPos] ) menores[id] = MagVolumen[indPos];
					}
				}
		}
	}

	for(int i=0;i<nroProcesadoresOpenMP;++i){
		MagMax = max(mayores[i],MagMax);
		MagMin = min(menores[i],MagMin);
	}
}

void FunctionTrans::calculaKerSegundaLaplaciano()//listo
{
	
	//obtengo los tres kernel para la segunda derivada
	//es decir las derivadas parciales con respecto a x, y e z

	//primera con respecto a x
	sumakerDerX = qcalculaSubKer(&qkerSegunda,&qkerInterpola,&qkerInterpola,&qkerDerX);
	//primera con respecto a y
	sumakerDerY = qcalculaSubKer(&qkerInterpola,&qkerSegunda,&qkerInterpola,&qkerDerY);
	//primera con respecto a z
	sumakerDerZ = qcalculaSubKer(&qkerInterpola,&qkerInterpola,&qkerSegunda,&qkerDerZ);

	if(SegDerivada==NULL && (SegDerivada=(double *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(double)))==NULL) return;
	SegMax = 0.0f; SegMin = 65536.0f;
	double gx,gy,gz;
	int tamDif1 = qkerInterpola.size()/2, tamDif2 = tamDif1+1;
	int indPos,qMi,pMj,oMk,muestra;

	QVector<double> menores(nroProcesadoresOpenMP,65536.0f);
	QVector<double> mayores(nroProcesadoresOpenMP,0.0f);
	int id;
	#pragma omp parallel private(gx,gy,gz,indPos,qMi,pMj,oMk,muestra,id)
	{
		id = omp_get_thread_num();
		#pragma omp for nowait
		for(int k=0; k<Volumen->profundidad ; ++k){
				for(int j=0; j<Volumen->alto ; ++j){
					for(int i=0; i<Volumen->ancho ; ++i){
						gx=gy=gz=0;
						
						//computo kernel
						for(int o=(k-tamDif1);o<(k+tamDif2);++o)
						{
							for(int p=(j-tamDif1);p<(j+tamDif2);++p)
							{
								for(int q=(i-tamDif1);q<i+tamDif2;(++q))
								{
									//si tiene valores validos computo el gradiente
									if(o>=0 && p>=0 && q>=0 && o<Volumen->profundidad && p<Volumen->alto && q<Volumen->ancho)
									{
										muestra = getMuestra(Volumen->Tex3d,Volumen->ancho,Volumen->alto,Volumen->profundidad,q,p,o);
										qMi = q-i+tamDif1; pMj = p-j+tamDif1; oMk = o-k+tamDif1;
										//para la componente X del vector gradiente
										gx+=((double)muestra*(double)qkerDerX[qMi][pMj][oMk]);
										gy+=((double)muestra*(double)qkerDerY[qMi][pMj][oMk]);
										gz+=((double)muestra*(double)qkerDerZ[qMi][pMj][oMk]);

									}
						
								}
							
							}
						
						}

						if(sumakerDerX!=0)
							gx/= sumakerDerX;
						if(sumakerDerY!=0)
							gy/= sumakerDerY;
						if(sumakerDerZ!=0)
							gz/= sumakerDerZ;
						
						indPos = i+(j+k*Volumen->alto)*Volumen->ancho;
						SegDerivada[indPos] = (gx+gy+gz);

						if( mayores[id]<SegDerivada[indPos] ) mayores[id] = SegDerivada[indPos];
						if( menores[id]>SegDerivada[indPos] ) menores[id] = SegDerivada[indPos];

						
					}
				}
		}
	}
	//buco mayores y menores
	for(int i=0;i<nroProcesadoresOpenMP;++i){
		SegMax = max(mayores[i],SegMax);
		SegMin = min(menores[i],SegMin);
	}
}
void FunctionTrans::calculaKerSegundaGradDelGrad()//listo
{

if(SegDerivada==NULL && (SegDerivada=(double *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(double)))==NULL) return;
	
	SegMax = 0.0f; SegMin = 65536.0f;
	double gx,gy,gz;
	double aux;
	
	int tamDif1 = qkerInterpola.size()/2, tamDif2 = tamDif1+1;
	int indPos,qMi,pMj,oMk,muestra;

	QVector<double> menores(nroProcesadoresOpenMP,65536.0f);
	QVector<double> mayores(nroProcesadoresOpenMP,0.0f);
	int id;
	#pragma omp parallel private(gx,gy,gz,indPos,qMi,pMj,oMk,muestra,id,aux)
	{
		id = omp_get_thread_num();
		#pragma omp for nowait
		for(int k=0; k<Volumen->profundidad ; ++k){
				for(int j=0; j<Volumen->alto ; ++j){
					for(int i=0; i<Volumen->ancho ; ++i){
						gx=gy=gz=0;
						
						//computo kernel
						for(int o=(k-tamDif1);o<(k+tamDif2);++o)
						{
							for(int p=(j-tamDif1);p<(j+tamDif2);++p)
							{
								for(int q=(i-tamDif1);q<i+tamDif2;(++q))
								{
									//si tiene valores validos computo el gradiente
									if(o>=0 && p>=0 && q>=0 && o<Volumen->profundidad && p<Volumen->alto && q<Volumen->ancho)
									{
										muestra = getMagGradiente(MagVolumen,Volumen->ancho,Volumen->alto,Volumen->profundidad,q,p,o);
										qMi = q-i+tamDif1; pMj = p-j+tamDif1; oMk = o-k+tamDif1;
										//para la componente X del vector gradiente
										gx+=((double)muestra*(double)qkerDerX[qMi][pMj][oMk]);
										gy+=((double)muestra*(double)qkerDerY[qMi][pMj][oMk]);
										gz+=((double)muestra*(double)qkerDerZ[qMi][pMj][oMk]);

									}
						
								}
							
							}
						
						}

						if(sumakerDerX!=0)
							gx/= sumakerDerX;
						if(sumakerDerY!=0)
							gy/= sumakerDerY;
						if(sumakerDerZ!=0)
							gz/= sumakerDerZ;

						indPos = i+(j+k*Volumen->alto)*Volumen->ancho;

						 aux = gx*VectoresGrad[indPos].x() + gy*VectoresGrad[indPos].y() + gz*VectoresGrad[indPos].z(); 

						if(aux == 0.0f || MagVolumen[indPos] == 0.0f) SegDerivada[indPos] = 0.0f;
						else SegDerivada[indPos] = (1.0f/MagVolumen[indPos]) * aux;

						
						if( mayores[id]<SegDerivada[indPos] ) mayores[id] = SegDerivada[indPos];
						if( menores[id]>SegDerivada[indPos] ) menores[id] = SegDerivada[indPos];

				}
			}
		}
	}
	//buco mayores y menores
	for(int i=0;i<nroProcesadoresOpenMP;++i){
		SegMax = max(mayores[i],SegMax);
		SegMin = min(menores[i],SegMin);
	}
}
void FunctionTrans::calculaKerSegundaHessian()//listo
{
	//obtengo los kernels para la segunda derivada
	//es decir las derivadas parciales con respecto a x,y,z,xy,xz,yz

	//las derivadas parciales extras
	long long sumakerDerXY,sumakerDerXZ,sumakerDerYZ;
	QVector<QVector<QVector<int>>> qkerDerXY,qkerDerXZ,qkerDerYZ;

	//primera con respecto a x
	sumakerDerX = qcalculaSubKer(&qkerSegunda,&qkerInterpola,&qkerInterpola,&qkerDerX);
	//primera con respecto a y
	sumakerDerY = qcalculaSubKer(&qkerInterpola,&qkerSegunda,&qkerInterpola,&qkerDerY);
	//primera con respecto a z
	sumakerDerZ = qcalculaSubKer(&qkerInterpola,&qkerInterpola,&qkerSegunda,&qkerDerZ);
	//XY
	sumakerDerXY = qcalculaSubKer(&qkerPrimera,&qkerPrimera,&qkerInterpola,&qkerDerXY);
	//XZ
	sumakerDerXZ = qcalculaSubKer(&qkerPrimera,&qkerInterpola,&qkerPrimera,&qkerDerXZ);
	//YZ
	sumakerDerYZ = qcalculaSubKer(&qkerInterpola,&qkerPrimera,&qkerPrimera,&qkerDerYZ);


	if(SegDerivada==NULL && (SegDerivada=(double *)malloc(Volumen->ancho*Volumen->alto*Volumen->profundidad*sizeof(double)))==NULL) return;
	
	SegMax = 0.0f; SegMin = 65536.0f;
	double gx,gy,gz,gxy,gxz,gyz,auxX,auxY,auxZ;
	
	int tamDif1 = qkerInterpola.size()/2, tamDif2 = tamDif1+1;
	int indPos,qMi,pMj,oMk,muestra;

	QVector<double> menores(nroProcesadoresOpenMP,65536.0f);
	QVector<double> mayores(nroProcesadoresOpenMP,0.0f);
	int id;
	#pragma omp parallel private(gx,gy,gz,gxy,gxz,gyz,indPos,qMi,pMj,oMk,muestra,id,auxX,auxY,auxZ)
	{
		id = omp_get_thread_num();
		#pragma omp for nowait
		for(int k=0; k<Volumen->profundidad ; ++k){
				for(int j=0; j<Volumen->alto ; ++j){
					for(int i=0; i<Volumen->ancho ; ++i){
						gx=gy=gz=gxy=gxz=gyz=0;
						
						//computo kernel
						for(int o=(k-tamDif1);o<(k+tamDif2);++o)
						{
							for(int p=(j-tamDif1);p<(j+tamDif2);++p)
							{
								for(int q=(i-tamDif1);q<i+tamDif2;(++q))
								{
									//si tiene valores validos computo el gradiente
									if(o>=0 && p>=0 && q>=0 && o<Volumen->profundidad && p<Volumen->alto && q<Volumen->ancho)
									{
										muestra = getMuestra(Volumen->Tex3d,Volumen->ancho,Volumen->alto,Volumen->profundidad,q,p,o);
										qMi = q-i+tamDif1; pMj = p-j+tamDif1; oMk = o-k+tamDif1;

										//para la componente X del vector gradiente
										gx+=((double)muestra*(double)qkerDerX[qMi][pMj][oMk]);
										gy+=((double)muestra*(double)qkerDerY[qMi][pMj][oMk]);
										gz+=((double)muestra*(double)qkerDerZ[qMi][pMj][oMk]);
										gxy+=((double)muestra*(double)qkerDerXY[qMi][pMj][oMk]);
										gxz+=((double)muestra*(double)qkerDerXZ[qMi][pMj][oMk]);
										gyz+=((double)muestra*(double)qkerDerYZ[qMi][pMj][oMk]);

									}
						
								}
							
							}
						
						}

						if(sumakerDerX!=0)
							gx/= sumakerDerX;
						if(sumakerDerY!=0)
							gy/= sumakerDerY;
						if(sumakerDerZ!=0)
							gz/= sumakerDerZ;
						if(sumakerDerXY!=0)
							gxy/= sumakerDerXY;
						if(sumakerDerXZ!=0)
							gxz/= sumakerDerXZ;
						if(sumakerDerYZ!=0)
							gyz/= sumakerDerYZ;

						indPos = i+(j+k*Volumen->alto)*Volumen->ancho;
				
					   auxX = (VectoresGrad[indPos].x()*gx + VectoresGrad[indPos].y()*gxy + VectoresGrad[indPos].z()*gxz);
					   auxY = (VectoresGrad[indPos].x()*gxy + VectoresGrad[indPos].y()*gy + VectoresGrad[indPos].z()*gyz);
					   auxZ = (VectoresGrad[indPos].x()*gxz + VectoresGrad[indPos].y()*gyz + VectoresGrad[indPos].z()*gz);

					   if(MagVolumen[indPos] == 0.0f) SegDerivada[indPos] = 0.0f;
					   else SegDerivada[indPos] = ((1.0f/(MagVolumen[indPos]*MagVolumen[indPos])) * (auxX*VectoresGrad[indPos].x()+auxY*VectoresGrad[indPos].y()+auxZ*VectoresGrad[indPos].z()));


						if( mayores[id]<SegDerivada[indPos] ) mayores[id] = SegDerivada[indPos];
						if( menores[id]>SegDerivada[indPos] ) menores[id] = SegDerivada[indPos];
		
					}
				}
		}
	}
	//buco mayores y menores
	for(int i=0;i<nroProcesadoresOpenMP;++i){
		SegMax = max(mayores[i],SegMax);
		SegMin = min(menores[i],SegMin);
	}
}
void FunctionTrans::liberoMemoria()
{
	if(MagxValor != NULL) free(MagxValor), MagxValor=NULL;
	if(SegDerxValor!= NULL ) free(SegDerxValor), SegDerxValor=NULL; 
	if(MagSegVolumenTextura != NULL) free(MagSegVolumenTextura), MagSegVolumenTextura=NULL;
	if(TexturaSegDerxValor != NULL) free(TexturaSegDerxValor), TexturaSegDerxValor=NULL;
	if(TexturaMagxValor != NULL) free(TexturaMagxValor), TexturaMagxValor=NULL;
	if(SegVolumenTextura != NULL) free(SegVolumenTextura), SegVolumenTextura=NULL;
	if(MagVolumen != NULL) free(MagVolumen), MagVolumen=NULL;
	if(SegDerivada != NULL) free(SegDerivada), SegDerivada=NULL;
	if(VectoresGrad != NULL) free(VectoresGrad), VectoresGrad=NULL;

}