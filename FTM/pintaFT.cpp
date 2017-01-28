#include "pintaFT.h"
#define MAKE_PIXEL(a,r,g,b) (a << 24) + (r << 16) + (g << 8) + b

pintaFT::pintaFT()
{
	MyImagenAct = new QImage();
	MyImagenBase = new QImage();
	MyImagenBaseAct = new QImage();

	//figura seleccionada
	FigSelect = NULL;
	
	soloUnaVez=true;
	moverFigClasi = false;
	escalaFigClasi = 0;

	autoRefresh = false;
}

pintaFT::~pintaFT()
{
}
void pintaFT::setMyImagen(QImage * Image)
{
	MyImagenAct = Image;
}
void pintaFT::paintInImage(QImage * Image)
{
	//pinto todas las figuras
	for(int i=0;i<vectorOfFig.size(); ++i)
	{
		//ojo si la caracteristica es activa la pinto
		if(vectorOfFig.at(i)->FigSelect)
			vectorOfFig.at(i)->pinta(Image,volumenPinta->valTFmagnitud);
	}
}
void pintaFT::paintInFTM(QImage * Image)
{
	//pinto todas las figuras
	for(int i=0;i<vectorOfFig.size(); ++i)
	{
		if(vectorOfFig.at(i)->FigSelect)	
		vectorOfFig.at(i)->pintaInFTM(Image);
	}
	//si la FigSelect no es nula, pinto su area de selección
	if(FigSelect!=NULL && FigSelect->FigSelect)
		FigSelect->pintaInFTMSelect(Image);			
}
void pintaFT::paintEvent (QPaintEvent *)
{
	QPainter pintor(this);
	pintor.drawImage(0,0,*MyImagenAct);
} 
void pintaFT::mousePressEvent(QMouseEvent * mouseEvent)
{
	////para conectar la señal de presionar en la tabla con  la figClasi que select
	//if(soloUnaVez){
	//		connect(tablaFig, SIGNAL(itemSelectionChanged()), this, SLOT(Selecciono()));
	//		connect(tablaFig,SIGNAL(cellDoubleClicked(int,int)), this, SLOT(creaComboBox(int,int)));
	//		connect(tablaFig,SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(cambioCellSelect(int,int,int,int)));
	//		soloUnaVez = false;
	//}

	//si no hay figura seleccionada tengo varias opciones, seleccionar una o crear 
	if(FigSelect==NULL){
		//dependiendo del tipo de figura la creo en este caso solo esta el rectangulo
		FigSelect = new rectanguloClasi();
		FigSelect->coorIni = QPoint(mouseEvent->x(),mouseEvent->y());
		FigSelect->tipoFig = 0;
		FigSelect->FigSelect = true;
		vectorOfFig.push_back(FigSelect);
		//inserto en la tabla
		tablaFig->insertRow(tablaFig->rowCount());
		
		QTableWidgetItem *nombreFig = new QTableWidgetItem(FigSelect->nombre);
		QTableWidgetItem *tipoFig = new QTableWidgetItem(tr("R"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/rectangulo.gif")));
		QTableWidgetItem *estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/activo.gif")),tr("Activo"));
	
        tablaFig->setItem(tablaFig->rowCount()-1, 1, nombreFig);
		tablaFig->setItem(tablaFig->rowCount()-1, 2, estadoFig);
		tablaFig->setItem(tablaFig->rowCount()-1, 0, tipoFig);

	}else{

		//si le di click a la figClasi la puedo mover
		if(mouseEvent->x()>=FigSelect->coorIni.x() && mouseEvent->x()<=(FigSelect->coorIni.x()+FigSelect->coorFin.x()) && mouseEvent->y()>=FigSelect->coorIni.y() && mouseEvent->y()<=(FigSelect->coorIni.y()+FigSelect->coorFin.y()))
		{
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			moverFigClasi=true;
		}else{
			int x1 = FigSelect->coorIni.x()-3, y1 = FigSelect->coorIni.y()-3,x2 = FigSelect->coorIni.x()+FigSelect->coorFin.x(),y2 = FigSelect->coorIni.y()+FigSelect->coorFin.y();
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			if(mouseEvent->x()>=x1 && mouseEvent->x()<=(x1+3) && mouseEvent->y()>=y1 && mouseEvent->y()<=(y1+3))
				escalaFigClasi=1;
			else
				if(mouseEvent->x()>=x2 && mouseEvent->x()<=(x2+3) && mouseEvent->y()>=y1 && mouseEvent->y()<=(y1+3))
					escalaFigClasi=2;
				else
					if(mouseEvent->x()>=x2 && mouseEvent->x()<=(x2+3) && mouseEvent->y()>=y2 && mouseEvent->y()<=(y2+3))
						escalaFigClasi=3;
					else
						if(mouseEvent->x()>=x1 && mouseEvent->x()<=(x1+3) && mouseEvent->y()>=y2 && mouseEvent->y()<=(y2+3))
							escalaFigClasi=4;
		} 
	}
}
void pintaFT::mouseReleaseEvent(QMouseEvent * mouseEvent)
{
	QPoint puntoFinal(mouseEvent->x(),mouseEvent->y());
	//si no esta creada la FigClasi
	if(FigSelect->coorFin.x()==0 && FigSelect->coorFin.y()==0){
		//tengo que hacer eso para la figSelect
		if(puntoFinal.x()<FigSelect->coorIni.x()){puntoFinal.setX(FigSelect->coorIni.x());FigSelect->coorIni.setX(mouseEvent->x());} 
		if(puntoFinal.y()<FigSelect->coorIni.y()){puntoFinal.setY(FigSelect->coorIni.y());FigSelect->coorIni.setY(mouseEvent->y());}
		FigSelect->coorFin = QPoint(abs(puntoFinal.x()-FigSelect->coorIni.x()),abs(puntoFinal.y()-FigSelect->coorIni.y()));

		emit creaFig();
	}
	//si estuve moviendo una figura actulizo las coord
	int dx,dy;
	if(moverFigClasi)
	{
		dx = FigSelect->coorIni.x() + mouseEvent->x() - postAct.x();dy = FigSelect->coorIni.y() + mouseEvent->y() - postAct.y();
		FigSelect->coorIni.setX(dx);
		FigSelect->coorIni.setY(dy);
	}else{
		//para escalar la imagen por ejes
		switch (escalaFigClasi){
		case 1:
			dx = - mouseEvent->x() + FigSelect->coorIni.x();dy =  - mouseEvent->y() + FigSelect->coorIni.y();
			FigSelect->coorFin.setX(FigSelect->coorFin.x() + dx);
			FigSelect->coorFin.setY(FigSelect->coorFin.y() + dy);

			if( FigSelect->coorFin.x()<0) FigSelect->coorIni.setX(mouseEvent->x()+FigSelect->coorFin.x()),FigSelect->coorFin.setX(abs(FigSelect->coorFin.x()));
			if( FigSelect->coorFin.y()<0) FigSelect->coorIni.setY(mouseEvent->y()+FigSelect->coorFin.y()),FigSelect->coorFin.setY(abs(FigSelect->coorFin.y()));
			break;
		case 2:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin+= QPoint(dx,-dy);
			FigSelect->coorIni.setY( FigSelect->coorIni.y()+dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 3:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin+= QPoint(dx,dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 4:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin+= QPoint(-dx,dy);
			FigSelect->coorIni.setX( FigSelect->coorIni.x()+dx);
			break;
		}
	
	}

	//actualizo la imagen
	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();


	//tengo que actualizar el volumen
	//auto refresco
	if(autoRefresh) 
		emit updateVol();

	//re inicializo
	moverFigClasi=false;
	escalaFigClasi = 0;

	//para comunicar los dos widgets de FTM
	emit cambioFig(FigSelect);
}
void pintaFT::mouseMoveEvent(QMouseEvent * mouseEvent)
{
	//si me puedo mover
	if(moverFigClasi)
	{
		int dx = FigSelect->coorIni.x() + mouseEvent->x() - postAct.x(),dy = FigSelect->coorIni.y() + mouseEvent->y() - postAct.y();
		FigSelect->coorIni.setX(dx);
		FigSelect->coorIni.setY(dy);

		postAct = QPoint(mouseEvent->x(),mouseEvent->y());

		//actualizo la imagen del FTM
		*MyImagenAct = *MyImagenBaseAct;
		paintInFTM(MyImagenAct);
		update();

		//para actulizar la otra tabla tambien
		emit cambioFig(FigSelect);

	}else if (FigSelect->coorFin.x()!=0 && FigSelect->coorFin.y()!=0){
		int dx,dy;
		switch (escalaFigClasi){
		case 1:
			dx = - mouseEvent->x() + FigSelect->coorIni.x();dy =  - mouseEvent->y() + FigSelect->coorIni.y();
			FigSelect->coorIni.setX(mouseEvent->x());
			FigSelect->coorIni.setY(mouseEvent->y());
			FigSelect->coorFin.setX(FigSelect->coorFin.x() + dx);
			FigSelect->coorFin.setY(FigSelect->coorFin.y() + dy);
			break;
		case 2:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin+= QPoint(dx,-dy);
			FigSelect->coorIni.setY( FigSelect->coorIni.y()+dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 3:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin+= QPoint(dx,dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 4:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin+= QPoint(-dx,dy);
			FigSelect->coorIni.setX( FigSelect->coorIni.x()+dx);
			break;
		}
		
		postAct = QPoint(mouseEvent->x(),mouseEvent->y());

		//actualizo la imagen del FTM
		*MyImagenAct = *MyImagenBaseAct;
		paintInFTM(MyImagenAct);
		update();
		//para comunicar los dos widgets de FTM
		emit cambioFig(FigSelect);
	}
}
void pintaFT::crearVolumenofClasificacion()
{	
	//tengo que iterar por el eje de la segDerivada
	bool primeraVez = (volumeFTM.size()==0);
	for(int i=0; i<volumenPinta->valTFsegDerivada; ++i){
		QImage * Aux;
		if(primeraVez){
			Aux = new QImage(MyImagenAct->width(),MyImagenAct->height(),QImage::Format_ARGB32);
		}else{
			Aux = (volumeFTM.at(i));
		}
		Aux->fill(0);//seteo a cero todo 

		//pinto la figura en el lapso del z
		for(int j=0;j<vectorOfFig.size();++j)
		{
			if(volumenPinta->nroDimensiones==3 && vectorOfFig.at(j)->FigSelect && i<=(vectorOfFig.at(j)->Z.x()+vectorOfFig.at(j)->Z.y()) && i>=(vectorOfFig.at(j)->Z.x()))
				vectorOfFig.at(j)->pinta(Aux,volumenPinta->valTFmagnitud);
			else if(volumenPinta->nroDimensiones==2)
					vectorOfFig.at(j)->pinta(Aux,volumenPinta->valTFmagnitud);
		}

		//agrego la imagen en la pos z
		if(primeraVez)
			volumeFTM.push_back(Aux);
	}
}
unsigned char* pintaFT::bitsVolumenClasi()
{
	if(volumeFTM.size()==0) return NULL;
	
	unsigned int* datosClasi = (unsigned int*)malloc(volumenPinta->valTFvalores*volumenPinta->valTFmagnitud*volumenPinta->valTFsegDerivada*4);
	//int k, j,i;
	QRgb ColorAux;
	#pragma omp parallel private(ColorAux)
	{
		#pragma omp for nowait
		for(int k=0; k<volumenPinta->valTFsegDerivada; k++){
			for(int j=0; j<volumenPinta->valTFmagnitud; j++){
				for(int i=0; i<volumenPinta->valTFvalores; i++){
				 ColorAux = volumeFTM.at(k)->pixel(i,j);  
				 datosClasi[i+(j+k*volumenPinta->valTFmagnitud)*volumenPinta->valTFvalores] = MAKE_PIXEL(qRed(ColorAux),qGreen(ColorAux),qBlue(ColorAux),qAlpha(ColorAux));
			 }
			}
		}
	}
	return ((unsigned char*) datosClasi);
}
void pintaFT::Selecciono()
{
	if(vectorOfFig.size()== 0) return;
	//escojo al nuevo y redibujo
	FigSelect = vectorOfFig.at(tablaFig->currentRow());

	//actualizo la imagen del FTM
	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();

	//envio a la otra ftm
	emit cambioFig(FigSelect);
}
void pintaFT::actualizarFTM(clasificaFig *Fig)
{
	//selecciono mi Fig actual
	FigSelect = Fig,

	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();
}

void pintaFT::creaComboBox(int fil, int col)
{
	if(col==1) return;

	if(col==0){
		//para seleccionar las figClasi
		QIcon icon1(QString::fromUtf8("iconos/rectangulo.gif"));
		QIcon icon2(QString::fromUtf8("iconos/triangulo.gif"));
		QIcon icon3(QString::fromUtf8("iconos/circulo.gif"));
		QComboBox *tipoFig = new QComboBox(); tipoFig->addItem(icon1,NULL);tipoFig->addItem(icon2,NULL);tipoFig->addItem(icon3,NULL);
		tipoFig->setCurrentIndex(FigSelect->tipoFig);
		//conecto para cambiar figuraClasi
		//connect(tipoFig, SIGNAL(currentIndexChanged(int)), this, SLOT(actulizarFigClasi(int)));
		connect(tipoFig, SIGNAL(activated(int)), this, SLOT(actulizarFigClasi(int)));
		tablaFig->setCellWidget(fil, col,tipoFig);

	}else{
		//para verificar si esta activo o no
		QIcon icona1(QString::fromUtf8("iconos/activo.gif"));
		QIcon icona2(QString::fromUtf8("iconos/desactivo.gif"));
		QComboBox *estadoFig = new QComboBox();estadoFig->addItem(icona1,tr("Activo"));estadoFig->addItem(icona2,tr("Desactivo"));
		//activa figClasi
		connect(estadoFig, SIGNAL(activated(int)), this, SLOT(activaFigClasi(int)));
		estadoFig->setCurrentIndex(!FigSelect->FigSelect);
		tablaFig->setCellWidget(fil, col,estadoFig);
	}
	tablaFig->update();
}
void pintaFT::actulizarFigClasi(int fig)
{
	//si se selecciona otro tipo de fig se crea
	if(FigSelect->tipoFig != fig)
	{
		clasificaFig * aux;

		switch(fig){
		case 0: 
			aux = new rectanguloClasi();
			aux->tipoFig = 0;
			break;
		case 1: 
			aux = new trianguloClasi();
			aux->tipoFig = 1;
			break;
		case 2: 
			aux = new circuloClasi();
			aux->tipoFig = 2;
			break;
		}
		//intercambio valores
		aux->bordeFig = FigSelect->bordeFig;
		aux->Z = FigSelect->Z;
		aux->coorIni = FigSelect->coorIni;
		aux->coorFin = FigSelect->coorFin;
		aux->colorIni = FigSelect->colorIni;
		aux->colorFin = FigSelect->colorFin;
		aux->DegradadoIni = FigSelect->DegradadoIni;
		aux->DegradadoFin = FigSelect->DegradadoFin;
		aux->FigSelect = FigSelect->FigSelect;
		aux->penFig = FigSelect->penFig;
		aux->nombre = FigSelect->nombre;
		
		//actulizo la fig
		delete FigSelect;
		FigSelect = aux;

		vectorOfFig[tablaFig->currentRow()] = aux;


		*MyImagenAct = *MyImagenBaseAct;
		paintInFTM(MyImagenAct);
		update();

		//aviso la otra ftm
		emit cambioFig(FigSelect);
	}

	int filFigClasi;
	for(int i=0; i<vectorOfFig.size(); ++i) if(vectorOfFig.at(i)==FigSelect) { filFigClasi=i; break; }

	tablaFig->removeCellWidget(filFigClasi,0);
	//acomodo el item
	QTableWidgetItem *tipoFig;

	switch(vectorOfFig.at(filFigClasi)->tipoFig){
		case 0:
			tipoFig = new QTableWidgetItem(tr("R"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/rectangulo.gif")));
			break;
		case 1:
			tipoFig = new QTableWidgetItem(tr("T"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/triangulo.gif")));
			break;
		case 2:
			tipoFig = new QTableWidgetItem(tr("C"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/circulo.gif")));
			break;
	}
	tablaFig->setItem(filFigClasi, 0, tipoFig);

	//si esta el refrescar automáticamente activo refresco
	if(autoRefresh && vectorOfFig.at(filFigClasi)->FigSelect) 
		emit updateVol();
}
void pintaFT::activaFigClasi(int fig)
{
	if( (!fig) != FigSelect->FigSelect ){
		FigSelect->FigSelect = !fig;

		*MyImagenAct = *MyImagenBaseAct;
		paintInFTM(MyImagenAct);
		update();

		//aviso a la otra ftm
		emit cambioFig(FigSelect);
	}

	int filFigClasi;
	for(int i=0; i<vectorOfFig.size(); ++i) if(vectorOfFig.at(i)==FigSelect) { filFigClasi=i; break; }

	tablaFig->removeCellWidget(filFigClasi,2);

	//acomodo el item
	QTableWidgetItem *estadoFig;

	switch(!vectorOfFig.at(filFigClasi)->FigSelect){
			case 0:
				estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/activo.gif")),tr("Activo"));
				break;
			case 1:
				estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/desactivo.gif")),tr("Desactivo"));
				break;
		}

	tablaFig->setItem(filFigClasi, 2, estadoFig);
	//si esta el refrescar automáticamente activo refresco
	if(autoRefresh) 
		emit updateVol();

}
void pintaFT::cambioCellSelect(int fil,int col,int filAnt,int colAnt)
{
	//si no habia presionado antes nada no hago nada
	if(filAnt==-1 || colAnt==-1 || fil==-1 || col==-1) return;


	if(colAnt==0 || colAnt==2 && filAnt<=tablaFig->rowCount() && fil<=tablaFig->rowCount())
	{
		if(filAnt>=vectorOfFig.size()) return;
		//remuevo el Widget
		tablaFig->removeCellWidget(filAnt,colAnt);

		//acomodo el item
		QTableWidgetItem *tipoFig;
		QTableWidgetItem *estadoFig;

		switch(vectorOfFig.at(filAnt)->tipoFig){
			case 0:
				tipoFig = new QTableWidgetItem(tr("R"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/rectangulo.gif")));
				break;
			case 1:
				tipoFig = new QTableWidgetItem(tr("T"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/triangulo.gif")));
				break;
			case 2:
				tipoFig = new QTableWidgetItem(tr("C"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/circulo.gif")));
				break;
		}

		switch(!vectorOfFig.at(filAnt)->FigSelect){
			case 0:
				estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/activo.gif")),tr("Activo"));
				break;
			case 1:
				estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/desactivo.gif")),tr("Desactivo"));
				break;
		}

		tablaFig->setItem(filAnt, 2, estadoFig);
		tablaFig->setItem(filAnt, 0, tipoFig);
	}
}
