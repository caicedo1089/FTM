#include "pintaftseg.h"

pintaFTSeg::pintaFTSeg()
{
	MyImagenAct = new QImage();
	MyImagenBase = new QImage();
	MyImagenBaseAct = new QImage();

	//figura seleccionada
	FigSelect = NULL;
	
	soloUnaVez=true;
	moverFigClasi = false;
	escalaFigClasi = 0;
	escalaFigClasiLado = false;

	//apuntador de vectores de fig
	vectorOfFig = NULL;

	autoRefresh = false;
}

pintaFTSeg::~pintaFTSeg()
{

}

void pintaFTSeg::setMyImagen(QImage * Image)
{
	MyImagenAct = Image;
}
void pintaFTSeg::paintInImage(QImage * Image)
{
	//pinto todas las figuras
	for(int i=0;i<vectorOfFig->size(); ++i)
	{
		//ojo si la caracteristica es activa la pinto
		if(vectorOfFig->at(i)->FigSelect)
			vectorOfFig->at(i)->pintaSeg(Image,volumenPinta->valTFmagnitud);
	}
}
void pintaFTSeg::paintInFTM(QImage * Image){
	//pinto todas las figuras
	for(int i=0;i<vectorOfFig->size(); ++i)
	{
		if(vectorOfFig->at(i)->FigSelect)	
		vectorOfFig->at(i)->pintaInFTMSeg(Image,volumenPinta->valTFmagnitud);
	}
	//si la FigSelect no es nula, pinto su area de selección
	if(FigSelect!=NULL && FigSelect->FigSelect)
		FigSelect->pintaInFTMSelectSeg(Image,volumenPinta->valTFmagnitud);
			
}
void pintaFTSeg::paintEvent (QPaintEvent *)
{
	QPainter pintor(this);
	pintor.drawImage(0,0,*MyImagenAct);
} 
void pintaFTSeg::mousePressEvent(QMouseEvent * mouseEvent)
{
	//si no hay figura seleccionada tengo varias opciones, seleccionar una o crear 
	if(FigSelect==NULL){
		/*if(soloUnaVez){
			connect(tablaFig, SIGNAL(itemSelectionChanged()), this, SLOT(SeleccionoFigOFTable()));
			connect(tablaFig,SIGNAL(cellDoubleClicked(int,int)), this, SLOT(creaComboBox(int,int)));
			connect(tablaFig,SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(cambioCellSelect(int,int,int,int)));
			soloUnaVez = false;
		}*/

		//dependiendo del tipo de figura la creo en este caso solo esta el rectangulo
		FigSelect = new rectanguloClasi();
		FigSelect->coorIni = QPoint(mouseEvent->x(),128);
		FigSelect->Z = QPoint(mouseEvent->y(),0);
		FigSelect->tipoFig = 0;
		FigSelect->FigSelect = true;
		vectorOfFig->push_back(FigSelect);
		//inserto en la tabla
		tablaFig->insertRow(tablaFig->rowCount());
		
		QTableWidgetItem *nombreFig = new QTableWidgetItem(FigSelect->nombre);
		QTableWidgetItem *tipoFig = new QTableWidgetItem(tr("R"));tipoFig->setIcon(QIcon(QString::fromUtf8("iconos/rectangulo.gif")));
		QTableWidgetItem *estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/activo.gif")),tr("Activo"));
	
        tablaFig->setItem(tablaFig->rowCount()-1, 1, nombreFig);
		tablaFig->setItem(tablaFig->rowCount()-1, 2, estadoFig);
		tablaFig->setItem(tablaFig->rowCount()-1, 0, tipoFig);

		tablaFig->update();
		

	}else{

		//si le di click a la figClasi la puedo mover
		if(mouseEvent->x()>=FigSelect->coorIni.x() && mouseEvent->x()<=(FigSelect->coorIni.x()+FigSelect->coorFin.x()) && mouseEvent->y()>=FigSelect->Z.x() && mouseEvent->y()<=(FigSelect->Z.x()+FigSelect->Z.y()))
		{
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			moverFigClasi=true;
		}else{
			int x1 = FigSelect->coorIni.x()-3, y1 = FigSelect->Z.x()-3,x2 = FigSelect->coorIni.x()+FigSelect->coorFin.x(),y2 = FigSelect->Z.x()+FigSelect->Z.y();
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
void pintaFTSeg::mouseReleaseEvent(QMouseEvent * mouseEvent)
{
	QPoint puntoFinal(mouseEvent->x(),mouseEvent->y());
	//si no esta creada la FigClasi
	if(FigSelect->coorFin.x()==0 && FigSelect->coorFin.y()==0){
		//tengo que hacer eso para la figSelect
		if(puntoFinal.x()<FigSelect->coorIni.x()){puntoFinal.setX(FigSelect->coorIni.x());FigSelect->coorIni.setX(mouseEvent->x());} 
		if(puntoFinal.y()<FigSelect->Z.x()){puntoFinal.setY(FigSelect->Z.x());FigSelect->Z.setX(mouseEvent->y());}
		FigSelect->coorFin = QPoint(abs(puntoFinal.x()-FigSelect->coorIni.x()),64);
		FigSelect->Z.setY(abs(puntoFinal.y()-FigSelect->Z.x()));
		FigSelect->Z.setX(FigSelect->Z.x());

		emit creaFig();
	}
	//si estuve moviendo una figura actualizo las coord
	int dx,dy;
	if(moverFigClasi)
	{
		int dx = FigSelect->coorIni.x() + mouseEvent->x() - postAct.x(),dy = FigSelect->Z.x() + mouseEvent->y() - postAct.y();
		FigSelect->coorIni.setX(dx);
		FigSelect->Z.setX(dy);
	}else{
		//para escalar la imagen por ejes
		switch (escalaFigClasi){
		case 1:
			dx = - mouseEvent->x() + FigSelect->coorIni.x();dy =  - mouseEvent->y() + FigSelect->Z.x();
			FigSelect->coorIni.setX(mouseEvent->x());
			FigSelect->Z.setX(mouseEvent->y());
			FigSelect->coorFin.setX(FigSelect->coorFin.x() + dx);
			FigSelect->Z.setY(FigSelect->Z.y() + dy);

			break;
		case 2:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin.setX(FigSelect->coorFin.x()+dx);
			FigSelect->Z.setY(FigSelect->Z.y()-dy);
			FigSelect->Z.setX(FigSelect->Z.x()+dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 3:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin.setX(FigSelect->coorFin.x()+dx);
			FigSelect->Z.setY(FigSelect->Z.y()+dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 4:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin.setX(FigSelect->coorFin.x()-dx);
			FigSelect->Z.setY(FigSelect->Z.y()+dy);
			FigSelect->coorIni.setX( FigSelect->coorIni.x()+dx);
			break;
		}
	
	}

	//actualizo la imagen
	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();

	//tengo que actualizar el volumen
	//ojo con este refresh porque tiene mucho computo
	if(autoRefresh) 
		emit updateVol();

	//re inicializo
	moverFigClasi=false;
	escalaFigClasi = 0;

	emit cambioFigSeg(FigSelect);
}
void pintaFTSeg::mouseMoveEvent(QMouseEvent * mouseEvent)
{
	//si me puedo mover
	if(moverFigClasi)
	{
		int dx = FigSelect->coorIni.x() + mouseEvent->x() - postAct.x(),dy = FigSelect->Z.x() + mouseEvent->y() - postAct.y();
		FigSelect->coorIni.setX(dx);
		FigSelect->Z.setX(dy);

		postAct = QPoint(mouseEvent->x(),mouseEvent->y());

		//actualizo la imagen del FTM
		*MyImagenAct = *MyImagenBaseAct;
		paintInFTM(MyImagenAct);
		update();

		emit cambioFigSeg(FigSelect);

	}else if (FigSelect->coorFin.x()!=0 && FigSelect->coorFin.y()!=0){
		int dx,dy;
		switch (escalaFigClasi){
		case 1:
			dx = - mouseEvent->x() + FigSelect->coorIni.x();dy =  - mouseEvent->y() + FigSelect->Z.x();
			FigSelect->coorIni.setX(mouseEvent->x());
			FigSelect->Z.setX(mouseEvent->y());
			FigSelect->coorFin.setX(FigSelect->coorFin.x() + dx);
			FigSelect->Z.setY(FigSelect->Z.y() + dy);

			break;
		case 2:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin.setX(FigSelect->coorFin.x()+dx);
			FigSelect->Z.setY(FigSelect->Z.y()-dy);
			FigSelect->Z.setX(FigSelect->Z.x()+dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 3:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin.setX(FigSelect->coorFin.x()+dx);
			FigSelect->Z.setY(FigSelect->Z.y()+dy);
			postAct = QPoint(mouseEvent->x(),mouseEvent->y());
			break;
		case 4:
			dx = mouseEvent->x() - postAct.x();dy = mouseEvent->y() - postAct.y();
			FigSelect->coorFin.setX(FigSelect->coorFin.x()-dx);
			FigSelect->Z.setY(FigSelect->Z.y()+dy);
			FigSelect->coorIni.setX( FigSelect->coorIni.x()+dx);
			break;
		}
		
		postAct = QPoint(mouseEvent->x(),mouseEvent->y());

		//actualizo la imagen del FTM
		*MyImagenAct = *MyImagenBaseAct;
		paintInFTM(MyImagenAct);
		update();

		emit cambioFigSeg(FigSelect);
	}
}

void pintaFTSeg::SeleccionoFigOFTable()
{
	if(vectorOfFig->size()== 0) return;

	if( (tablaFig->currentRow()+1) > vectorOfFig->size() ) return;

	//escojo al nuevo y redibujo
	FigSelect = vectorOfFig->at(tablaFig->currentRow());

	//actualizo la imagen del FTM
	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();

	emit cambioFigSeg(FigSelect);
}

void pintaFTSeg::actualizarFTMSeg(clasificaFig *Fig)
{
	//selecciono mi Fig actual
	FigSelect = Fig,

	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();
}

void pintaFTSeg::actulizarFigClasi(int fig)
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

		(*vectorOfFig)[tablaFig->currentRow()] = aux;

	}

	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();

	//aviso la otra ftm
	emit cambioFigSeg(FigSelect);
}
void pintaFTSeg::activaFigClasi(int fig)
{
	FigSelect->FigSelect = !fig;

	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();

	//aviso a la otra ftm
	emit cambioFigSeg(FigSelect);
}

void pintaFTSeg::creaComboBox(int fil, int col)
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
		connect(tipoFig, SIGNAL(currentIndexChanged(int)), this, SLOT(actulizarFigClasi(int)));
		tablaFig->setCellWidget(fil, col,tipoFig);

	}else{
		//para verificar si esta activo o no
		//QTableWidgetItem *estadoFig = new QTableWidgetItem(tr("Activo"));
		QIcon icona1(QString::fromUtf8(":iconos/activo.gif"));
		QIcon icona2(QString::fromUtf8(":iconos/desactivo.gif"));
		QComboBox *estadoFig = new QComboBox();estadoFig->addItem(icona1,tr("Activo"));estadoFig->addItem(icona2,tr("Desactivo"));
		//activa figClasi
		connect(estadoFig, SIGNAL(currentIndexChanged(int)), this, SLOT(activaFigClasi(int)));
		estadoFig->setCurrentIndex(!FigSelect->FigSelect);
		tablaFig->setCellWidget(fil, col,estadoFig);
	}
	tablaFig->update();
}

void pintaFTSeg::cambioCellSelect(int fil,int col,int filAnt,int colAnt)
{

	//si no habia presionado antes nada no hago nada
	if(filAnt==-1 || colAnt==-1 || fil==-1 || col==-1) return;

	if(colAnt==0 || colAnt==2 && filAnt<=tablaFig->rowCount() && fil<=tablaFig->rowCount())
	{
		if(filAnt>=vectorOfFig->size()) return;
		//remuevo el Widget
		tablaFig->removeCellWidget(filAnt,colAnt);
		//acomodo el item
		QTableWidgetItem *tipoFig;
		QTableWidgetItem *estadoFig;

		switch(vectorOfFig->at(filAnt)->tipoFig){
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

		switch(!vectorOfFig->at(filAnt)->FigSelect){
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

void pintaFTSeg::insertArriba(int fil,QTableWidget *tablaFig,QVector<clasificaFig*> *vectorOfFig)
{
		//acomodo el item
		QTableWidgetItem *tipoFig;
		QTableWidgetItem *estadoFig;
		QTableWidgetItem *nombreFig = new QTableWidgetItem(vectorOfFig->at(fil)->nombre);

		switch(vectorOfFig->at(fil)->tipoFig){
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

		switch(!vectorOfFig->at(fil)->FigSelect){
			case 0:
				estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/activo.gif")),tr("Activo"));
				break;
			case 1:
				estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/desactivo.gif")),tr("Desactivo"));
				break;
		}

		tablaFig->setItem(fil, 2, estadoFig);
		tablaFig->setItem(fil, 1, nombreFig);
		tablaFig->setItem(fil, 0, tipoFig);
}
void pintaFTSeg::actualizaFtmElimina(clasificaFig *FigSelectNew)
{
	FigSelect = FigSelectNew;
	emit cambioFigSeg(FigSelectNew);

	*MyImagenAct = *MyImagenBaseAct;
	paintInFTM(MyImagenAct);
	update();
}