#include "ftm.h"

FTM::FTM(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//ajusto mi pantallaWidget en la parte central del window
	setCentralWidget(ui.pantallaWidget);

	//desactivo la ftm Widget
	ui.groupBox_2->setEnabled(false);
	ui.groupBox->setEnabled(false);
	ui.ConFigClasi->setEnabled(false);

	//inicializo la funcion de transferencia widget
	ui.pantallaWidget->drawFT = ui.AreaDraw;
	//mi tabla de figuras de clasificación
	ui.pantallaWidget->drawFT->tablaFig = ui.tablaFigClasi;

	ui.pantallaWidget->drawFTSeg = ui.AreaDrawSeg;
	//mi tabla de figuras de clasificación
	ui.pantallaWidget->drawFTSeg->tablaFig = ui.tablaFigClasi;

	//pata la FT unidimensional
	ui.pantallaWidget->drawFTUni = ui.AreaDrawFT1;

	ui.widgetSetFigClasi->setBackgroundRole(QPalette::Dark);
	ui.scrollArea->setBackgroundRole(QPalette::Dark);
	ui.scrollArea_2->setBackgroundRole(QPalette::Dark);
	//ui.dockWidgetFT->setMyImagen(ui.pantallaWidget->MyFT->TexturaMagxValor);
	//ui.setFTM->setVisible(false);
	
	//comparto el color del label
	//ui.pantallaWidget->MyFT->valorColor = new QColor();
	//ui.colorValor->setColor((ui.pantallaWidget->MyFT->valorColor));

	//mapaSelect = 1;
	//ui.pushValxMag->setEnabled(false);
	//ui.scrollArea->setPalette(QPalette(Qt::black))

	//arreglo las lineasGuias
	ui.EjeY->linea.setLine(3,0,3,258);
	ui.EjeY->colorLinea = Qt::red;
	ui.EjeZ->linea.setLine(3,0,3,258);
	ui.EjeZ->colorLinea = Qt::blue;
	ui.EjeX2 = ui.EjeX1;

	//se los paso al pantalla
	ui.pantallaWidget->ejeX = &(ui.EjeX1->colorLinea);
	ui.pantallaWidget->ejeY = &(ui.EjeY->colorLinea);
	ui.pantallaWidget->ejeZ = &(ui.EjeZ->colorLinea);

	//conecto las FTM widget con el widget de config
	connect(ui.pantallaWidget->drawFT, SIGNAL(cambioFig(clasificaFig *)), ui.widgetSetFigClasi, SLOT(setFigActual(clasificaFig *)));
	connect(ui.pantallaWidget->drawFTSeg, SIGNAL(cambioFigSeg(clasificaFig *)), ui.widgetSetFigClasi, SLOT(setFigActual(clasificaFig *)));
	//volver al estado inicial al crear fig
	connect(ui.pantallaWidget->drawFT, SIGNAL(creaFig()), this, SLOT(actulizaEstadoConfClasi()));
	connect(ui.pantallaWidget->drawFTSeg, SIGNAL(creaFig()), this, SLOT(actulizaEstadoConfClasi()));
	
	ui.widgetSetFigClasi->setAutoFillBackground(true);

	//colores iniciales
	ui.pColorConf1->setPalette(QPalette(Qt::black)); ui.pColorConf1->tipo=1;
	ui.pColorConf2->setPalette(QPalette(Qt::white));ui.pColorConf2->tipo=2;

	//connect para poder configurar
	connect(ui.pColorConf1, SIGNAL(clicked(int)), this, SLOT(label_Clicked(int)));
	connect(ui.pColorConf2, SIGNAL(clicked(int)), this, SLOT(label_Clicked(int)));
	connect(ui.tablaFigClasi,SIGNAL(cellClicked(int,int)), this, SLOT(actualFig(int,int)));

	//para acomodar losheader horizontales
	ui.tablaFigClasi->resizeColumnToContents(0);
    ui.tablaFigClasi->horizontalHeader()->setStretchLastSection(true);

	//propiedades de los kernels
	ui.dockWidgetKernels->setVisible(false);
	ui.dockWidgetFT1->setVisible(false);
	ui.dockWidgetFT1->setFixedWidth(284);
	connect(ui.AreaDrawFT1, SIGNAL(loadFT1()), ui.pantallaWidget, SLOT(loadFTUni()));

	ui.dockWidgetVol->setVisible(false);
	ui.dockWidgetVol->setFixedWidth(284);

	//eldockWidget de iluminaci'on
	ui.dockWidgetIlumina->setVisible(false);
	ui.dockWidgetIlumina->setFixedWidth(180);

	//colores default del Blinn-P
	ui.Lambiental->setPalette(QPalette(Qt::black));
	ui.Ldifusa->setPalette(QPalette(Qt::white));
	ui.Lespecular->setPalette(QPalette(Qt::white));

	//conecto las FTM entre si
	connect(ui.pantallaWidget->drawFT, SIGNAL(cambioFig(clasificaFig *)), ui.pantallaWidget->drawFTSeg, SLOT(actualizarFTMSeg(clasificaFig *)));
	connect(ui.pantallaWidget->drawFTSeg, SIGNAL(cambioFigSeg(clasificaFig *)), ui.pantallaWidget->drawFT, SLOT(actualizarFTM(clasificaFig *)));
	
	//para el refrescar automatico
	connect(ui.pantallaWidget->drawFT,SIGNAL(updateVol()),ui.pantallaWidget,SLOT(updateVolumenFTM()));
	connect(ui.pantallaWidget->drawFTSeg,SIGNAL(updateVol()),ui.pantallaWidget,SLOT(updateVolumenFTM()));


	//conecto la tabla de fig con las FTMs Widget
	//para DrawFTM Seg
	connect(ui.tablaFigClasi, SIGNAL(itemSelectionChanged()), ui.pantallaWidget->drawFTSeg, SLOT(SeleccionoFigOFTable()));
	connect(ui.tablaFigClasi,SIGNAL(cellDoubleClicked(int,int)), ui.pantallaWidget->drawFTSeg, SLOT(creaComboBox(int,int)));
	connect(ui.tablaFigClasi,SIGNAL(currentCellChanged(int,int,int,int)), ui.pantallaWidget->drawFTSeg, SLOT(cambioCellSelect(int,int,int,int)));
	
	//para DrawFTM
	connect(ui.tablaFigClasi, SIGNAL(itemSelectionChanged()), ui.pantallaWidget->drawFT, SLOT(Selecciono()));
	connect(ui.tablaFigClasi,SIGNAL(cellDoubleClicked(int,int)), ui.pantallaWidget->drawFT, SLOT(creaComboBox(int,int)));
	connect(ui.tablaFigClasi,SIGNAL(currentCellChanged(int,int,int,int)), ui.pantallaWidget->drawFT, SLOT(cambioCellSelect(int,int,int,int)));

}

FTM::~FTM()
{

}
void FTM::resizeEvent(QResizeEvent * e)
{
	
}

void FTM::keyPressEvent (QKeyEvent * event )
{

}
void FTM::keyReleaseEvent(QKeyEvent * event )
{
	//printf("pinto 2");
}
void FTM::on_actionAbrir_PVM_triggered()
{
	NameImg = QFileDialog::getOpenFileName(this,"Abrir un Volumen PVM",".pvm","Volumen (*.pvm )");
	if(NameImg!=QString::null){
		//elimino y re inicializo los volúmenes
		if(ui.pantallaWidget->T3d.Tex3d != 0){ 
			ui.pantallaWidget->reInicializoTodo();
		
		}

		//actionStatusBar->setText(QString("Cargando Volumen..."));
		ui.pantallaWidget->loadTexture3D(NameImg.toLatin1().data());
		ui.pantallaWidget->loadFTUni();
		ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
		ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);
		//actulizo las FTMs Widget
		ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
		ui.pantallaWidget->drawFT->update();
		ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
		ui.pantallaWidget->drawFTSeg->update();

		//activo la ftm Widget
		ui.groupBox_2->setEnabled(true);
		ui.groupBox->setEnabled(true);
		ui.ConFigClasi->setEnabled(true);

	}
}
void FTM::on_actionAbrir_FTs_triggered()
{
	// si han cargado algún volumen
	if(ui.pantallaWidget->T3d.Tex3d != NULL){
		QString NomFun = QFileDialog::getOpenFileName(this,"Abrir las FTs",".fts","Funciones de Transferencia(*.fts )");
		if(NomFun == QString::null ) return;
		FILE * pFile;
		pFile = fopen (NomFun.toStdString().c_str(), "r");
		if(pFile == NULL) QMessageBox::information(this, tr("Error"), tr("<p>Ocurrio un ERROR Al abrir el archivo."));
		else
		{
			//guardo los puntos de control de la funcion de transferencia 1D
			int tipFT, Tam, val, r, g, b, a;
			fscanf(pFile, "%d %d\n",&tipFT,&Tam);
			if(tipFT == 1 && Tam > 0){
				ui.pantallaWidget->drawFTUni->puntosControlFT.clear();
				for(int i=0; i<Tam; ++i)
				{
					fscanf(pFile, "%d %d %d %d %d\n",&val, &r, &g, &b, &a);
					ui.pantallaWidget->drawFTUni->puntosControlFT.push_back(QPair<int,QColor>(val,QColor(r,g,b,a)));
				}
				ui.pantallaWidget->drawFTUni->drawFTUni();
				ui.pantallaWidget->drawFTUni->update();
				ui.pantallaWidget->loadFTUni();
			}else{
				//dejo la Identidad
				ui.pantallaWidget->drawFTUni->puntosControlFT.clear();
				ui.pantallaWidget->drawFTUni->puntosControlFT.push_back(QPair<int,QColor>(0,QColor(0,0,0,0)));
				ui.pantallaWidget->drawFTUni->puntosControlFT.push_back(QPair<int,QColor>(255,QColor(255,255,255,255)));
			}
			//ojo creo que tengo que hacer update

			fscanf (pFile, "%d %d\n",&tipFT,&Tam);
			if(tipFT == 3){
			//limpio el vector y tambien la tabla de fig 
				for(int i=ui.pantallaWidget->drawFTSeg->tablaFig->rowCount()-1; i>-1; --i){ 
					ui.pantallaWidget->drawFTSeg->tablaFig->removeRow(i);
					delete ui.pantallaWidget->drawFT->vectorOfFig[i];
				}
			ui.pantallaWidget->drawFT->vectorOfFig.clear();
			ui.pantallaWidget->drawFTSeg->tablaFig->update();
			//cargamos las nueva figuras
			int figTip,activa,Zx,Zy,CoorIx,CoorIy,CoorFx,CoorFy, colorIr,colorIg,colorIb,colorIa,colorFr,colorFg,colorFb,colorFa;
			float degraI,degraF;
				for(int i=0; i<Tam; ++i)
				{
					char nombre[200];
					//El tipo de figura y si esta activa o no
					fscanf(pFile,"%d %d\n",&figTip,&activa);
					// el nombre lo guardo de tabla Ojo
					//ui.pantallaWidget->drawFT->vectorOfFig[i]->nombre = ui.pantallaWidget->drawFTSeg->tablaFig->item(i,1)->text();
					fgets (nombre, 200, pFile);
					//fscanf(pFile,"%s\n",nombre);
					//degradado de las figuras
					fscanf(pFile,"%f %f\n",&degraI,&degraF);
					//los puntos Z CoordIni CoordFin
					fscanf(pFile,"%d %d %d %d %d %d\n",&Zx,&Zy,&CoorIx,&CoorIy,&CoorFx,&CoorFy);
					//los respectivos colores
					//Ini
					fscanf(pFile,"%d %d %d %d\n",&colorIr,&colorIg,&colorIb,&colorIa);
					//Fin
					fscanf(pFile,"%d %d %d %d\n",&colorFr,&colorFg,&colorFb,&colorFa);
					//creo la fig seg'un tipo
					clasificaFig * aux;
					switch(figTip){
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
					//seteo los valores
					aux->bordeFig = false;
					aux->Z = QPoint(Zx,Zy);
					aux->coorIni = QPoint(CoorIx,CoorIy);
					aux->coorFin = QPoint(CoorFx,CoorFy);
					aux->colorIni = QColor(colorIr,colorIg,colorIb,colorIa);
					aux->colorFin = QColor(colorFr,colorFg,colorFb,colorFa);
					aux->DegradadoIni = degraI;
					aux->DegradadoFin = degraF;
					aux->FigSelect = activa;
					aux->penFig.setStyle(Qt::NoPen);
					aux->nombre = QString(nombre);
					//Ingreso a mi vector de fig
					ui.pantallaWidget->drawFT->vectorOfFig.push_back(aux);

					//ahora en la tabla de figClasi
					ui.pantallaWidget->drawFTSeg->tablaFig->insertRow(i);

					///////////
					QTableWidgetItem *nombreFig = new QTableWidgetItem(aux->nombre);
					QTableWidgetItem *tipoFig;
					QTableWidgetItem *estadoFig;

					switch(aux->tipoFig){
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

					switch(!aux->FigSelect){
						case 0:
							estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/activo.gif")),tr("Activo"));
							break;
						case 1:
							estadoFig = new QTableWidgetItem(QIcon(QString::fromUtf8("iconos/desactivo.gif")),tr("Desactivo"));
							break;
					}

				
					ui.pantallaWidget->drawFTSeg->tablaFig->setItem(i, 1, nombreFig);
					ui.pantallaWidget->drawFTSeg->tablaFig->setItem(i, 2, estadoFig);
					ui.pantallaWidget->drawFTSeg->tablaFig->setItem(i, 0, tipoFig);

					//ui.pantallaWidget->drawFTSeg->tablaFig->update();
				}

				//actualizo
				ui.pantallaWidget->drawFT->tablaFig->update();
				//actualizo tambien las FTM
				ui.pantallaWidget->drawFTSeg->actualizaFtmElimina(NULL);
				//actulizo la FTM
				ui.pantallaWidget->updateVolumenFTM();
			}
		
		}
		fclose (pFile);
	}else
	{
		QMessageBox::information(this, tr("Operación no permitida"), tr("<p>Debe cargar un volúmen."));
	}

}
void FTM::on_actionAbrir_TexturasVol_triggered()
{
	if(ui.pantallaWidget->T3d.Tex3d != 0)//si hay algún volumen cargado
	{
		QString NomFun = QFileDialog::getOpenFileName(this,"Abrir las Texturas del Vol.",".txv","Texturas del Vol.(*.txv )");
		if(NomFun == QString::null ) return;
		FILE *pFile;
		pFile = fopen (NomFun.toStdString().c_str(),"rb");
		int ancho, alto, prof, result;
		fscanf (pFile, "%d %d %d\n",&ancho, &alto, &prof);
		//verifico que coincidan los tamaños
		if(ui.pantallaWidget->T3d.ancho== ancho && ui.pantallaWidget->T3d.alto== alto && ui.pantallaWidget->T3d.profundidad==prof){
			int tam = ui.pantallaWidget->T3d.ancho*ui.pantallaWidget->T3d.alto*ui.pantallaWidget->T3d.profundidad;
			//recupero la textura de la mag y ls normales
			result = fread (ui.pantallaWidget->MyFT->MagSegVolumenTextura , 1 , sizeof(int)*tam , pFile);
			//recupero la textura de la seg deri
			result += fread (ui.pantallaWidget->MyFT->SegVolumenTextura , 1 , sizeof(char)*tam , pFile );
			//ahora las imagenes guias de las FTM widgets
			result += fread (&ui.pantallaWidget->MyFT->MagMax , 1 , sizeof(double) , pFile );
			result += fread (&ui.pantallaWidget->MyFT->MagMin , 1 , sizeof(double) , pFile );
			result += fread (ui.pantallaWidget->MyFT->MagxValor, 1 , (sizeof(float)*ui.pantallaWidget->MyFT->valTFvalores*ui.pantallaWidget->MyFT->valTFmagnitud), pFile );
			result += fread (&ui.pantallaWidget->MyFT->SegMax , 1 , sizeof(double) , pFile );
			result += fread (&ui.pantallaWidget->MyFT->SegMin , 1 , sizeof(double) , pFile );
			result += fread (ui.pantallaWidget->MyFT->SegDerxValor, 1 , (sizeof(float)*ui.pantallaWidget->MyFT->valTFvalores*ui.pantallaWidget->MyFT->valTFsegDerivada) , pFile );

			fclose (pFile);
			if(result != (sizeof(double)*4 + sizeof(int)*tam + sizeof(char)*tam + sizeof(float)*ui.pantallaWidget->MyFT->valTFvalores*ui.pantallaWidget->MyFT->valTFmagnitud + sizeof(float)*ui.pantallaWidget->MyFT->valTFvalores*ui.pantallaWidget->MyFT->valTFsegDerivada))
			{
				QMessageBox::information(this, tr("Error"), tr("<p>Existe algún problema con la carga."));
			}else{
				//se hizo la carga de manera correcta se puede actulizar las texturas en la TG
				//cargo a la TG la textura de la mag y las normales
				ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
				//la seg deri
				ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);
				//los histogramas
				ui.pantallaWidget->updateHistoFTMWidget();

				//actulizo FTMs
				ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
				ui.pantallaWidget->drawFT->update();
				ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
				ui.pantallaWidget->drawFTSeg->update();
			}
		}else{
			QMessageBox::information(this, tr("Archivo Incompatible"), tr("<p>Las dimesiones del archivo no coinciden con las del Volúmen."));
		}

	}
}
void FTM::on_actionGuardar_FTs_triggered()
{
	// si han cargado algún volumen
	if(ui.pantallaWidget->T3d.Tex3d != 0){
		QString NomFun = QFileDialog::getSaveFileName(this,"Guardar las FTs",".fts","Funciones de Transferencia(*.fts )");
		if(NomFun == QString::null ) return;
		FILE * pFile;
		pFile = fopen (NomFun.toLatin1().data(), "w");
		if(pFile == NULL) QMessageBox::information(this, tr("Error"), tr("<p>Ocurrio un ERROR Al crear el archivo"));
		else
		{
			//guardo los puntos de control de la funcion de transferencia 1D
			fprintf (pFile, "1 %d\n",ui.pantallaWidget->drawFTUni->puntosControlFT.size());
			for(int i=0; i<ui.pantallaWidget->drawFTUni->puntosControlFT.size(); ++i)
			{
				fprintf(pFile, "%d %d %d %d %d\n",ui.pantallaWidget->drawFTUni->puntosControlFT[i].first,ui.pantallaWidget->drawFTUni->puntosControlFT[i].second.red(),ui.pantallaWidget->drawFTUni->puntosControlFT[i].second.green(),ui.pantallaWidget->drawFTUni->puntosControlFT[i].second.blue(),ui.pantallaWidget->drawFTUni->puntosControlFT[i].second.alpha());
			}
			char nombre[1000];
			fprintf (pFile, "3 %d\n",ui.pantallaWidget->drawFT->vectorOfFig.size());
			for(int i=0; i<ui.pantallaWidget->drawFT->vectorOfFig.size(); ++i)
			{
				//El tipo de figura y si esta activa o no
				fprintf(pFile,"%d %d\n",ui.pantallaWidget->drawFT->vectorOfFig[i]->tipoFig,(int)ui.pantallaWidget->drawFT->vectorOfFig[i]->FigSelect);
				// el nombre lo guardo de tabla Ojo
				ui.pantallaWidget->drawFT->vectorOfFig[i]->nombre = ui.pantallaWidget->drawFTSeg->tablaFig->item(i,1)->text();
				fprintf(pFile,"%s\n",ui.pantallaWidget->drawFT->vectorOfFig[i]->nombre.toStdString().c_str());
				//degradado de las figuras
				fprintf(pFile,"%f %f\n",ui.pantallaWidget->drawFT->vectorOfFig[i]->DegradadoIni,ui.pantallaWidget->drawFT->vectorOfFig[i]->DegradadoFin);
				//los puntos Z CoordIni CoordFin
				fprintf(pFile,"%d %d %d %d %d %d\n",ui.pantallaWidget->drawFT->vectorOfFig[i]->Z.x(),ui.pantallaWidget->drawFT->vectorOfFig[i]->Z.y(),ui.pantallaWidget->drawFT->vectorOfFig[i]->coorIni.x(),ui.pantallaWidget->drawFT->vectorOfFig[i]->coorIni.y(),ui.pantallaWidget->drawFT->vectorOfFig[i]->coorFin.x(),ui.pantallaWidget->drawFT->vectorOfFig[i]->coorFin.y());
				//los respectivos colores
				//Ini
				fprintf(pFile,"%d %d %d %d\n",ui.pantallaWidget->drawFT->vectorOfFig[i]->colorIni.red(),ui.pantallaWidget->drawFT->vectorOfFig[i]->colorIni.green(),ui.pantallaWidget->drawFT->vectorOfFig[i]->colorIni.blue(),ui.pantallaWidget->drawFT->vectorOfFig[i]->colorIni.alpha());
				//Fin
				fprintf(pFile,"%d %d %d %d\n",ui.pantallaWidget->drawFT->vectorOfFig[i]->colorFin.red(),ui.pantallaWidget->drawFT->vectorOfFig[i]->colorFin.green(),ui.pantallaWidget->drawFT->vectorOfFig[i]->colorFin.blue(),ui.pantallaWidget->drawFT->vectorOfFig[i]->colorFin.alpha());
			}
		
		}
		fclose (pFile);
	}
}
void FTM::on_actionGuardar_TexturasVol_triggered()
{
	if(ui.pantallaWidget->T3d.Tex3d != 0)//si hay algún volumen cargado
	{
		QString NomFun = QFileDialog::getSaveFileName(this,"Guardar las Texturas Calculadas",".txv","Texturas del Vol.(*.txv )");
		if(NomFun == QString::null ) return;
		FILE *pFile;
		pFile = fopen (NomFun.toLatin1().data(),"wb");
		fprintf (pFile, "%d %d %d\n",ui.pantallaWidget->T3d.ancho,ui.pantallaWidget->T3d.alto,ui.pantallaWidget->T3d.profundidad);
		int tam = ui.pantallaWidget->T3d.ancho*ui.pantallaWidget->T3d.alto*ui.pantallaWidget->T3d.profundidad;
		//guardo la textura de la mag y las normales
		fwrite (ui.pantallaWidget->MyFT->MagSegVolumenTextura , 1 , sizeof(int)*tam , pFile );
		//la textura de la seg deri
		fwrite(ui.pantallaWidget->MyFT->SegVolumenTextura , 1 , sizeof(char)*tam , pFile );
		//las imagenes guias en la FTM widget
		fwrite(&ui.pantallaWidget->MyFT->MagMax , 1 , sizeof(double) , pFile );
		fwrite(&ui.pantallaWidget->MyFT->MagMin , 1 , sizeof(double) , pFile );
		fwrite(ui.pantallaWidget->MyFT->MagxValor , 1 , (sizeof(float)*ui.pantallaWidget->MyFT->valTFvalores*ui.pantallaWidget->MyFT->valTFmagnitud) , pFile );
		fwrite(&ui.pantallaWidget->MyFT->SegMax , 1 , sizeof(double) , pFile );
		fwrite(&ui.pantallaWidget->MyFT->SegMin , 1 , sizeof(double) , pFile );
		fwrite(ui.pantallaWidget->MyFT->SegDerxValor , 1 , (sizeof(float)*ui.pantallaWidget->MyFT->valTFvalores*ui.pantallaWidget->MyFT->valTFsegDerivada) , pFile );

		//cierro archivo
		fclose (pFile);
		//printf("sizeof de segderBuffer:%d\n",ui.pantallaWidget->MyFT->TexturaSegDerxValor->byteCount());
	}else{
		QMessageBox::information(this, tr("Operación no permitida"), tr("<p>Necesitas tener un volúmen cargado."));
	}
}

void FTM::on_actionMostrarBox_triggered()
{
	ui.pantallaWidget->Caja.activo = !ui.pantallaWidget->Caja.activo;
}
void FTM::on_actionColorBox_triggered()
{
	QColor anterior(ui.pantallaWidget->Caja.Color.x(),ui.pantallaWidget->Caja.Color.y(),ui.pantallaWidget->Caja.Color.z());
	QColor aux(QColorDialog::getColor(anterior,this,tr("Elija el color del Box")).rgb());
	ui.pantallaWidget->Caja.Color.setX(aux.redF());ui.pantallaWidget->Caja.Color.setY(aux.greenF());ui.pantallaWidget->Caja.Color.setZ(aux.blueF());
}

void FTM::on_actionMostrarFTM_triggered()
{
	if(ui.pantallaWidget->T3d.verFTM > 0.0f) ui.pantallaWidget->T3d.verFTM = 0.0f;
	else ui.pantallaWidget->T3d.verFTM = 1.0f;
}

void FTM::on_crearFigClasi_clicked()
{
	ui.pantallaWidget->drawFT->FigSelect = NULL;
	ui.pantallaWidget->drawFTSeg->FigSelect = NULL;
	ui.widgetSetFigClasi->FigSelect = NULL;
}

void FTM::on_refrescarVol_clicked()
{
	/*if(ui.pantallaWidget->drawFT->volumeFTM.size()!=0){
		ui.pantallaWidget->drawFT->crearVolumenofClasificacion();
		unsigned char * dat = ui.pantallaWidget->drawFT->bitsVolumenClasi();
		ui.pantallaWidget->updateVolumenFTM(dat);
	}*/
	ui.pantallaWidget->updateVolumenFTM();
}
void FTM::on_tablaFigClasi_cellActivated(int fil, int col)
{
	printf("hieron enter en la col:%d fil:%d\n",col,fil);
	//if( fil==1 ){}
}
void FTM::on_subirFigClasi_clicked()
{
	if(ui.pantallaWidget->drawFT->FigSelect==NULL || ui.pantallaWidget->drawFTSeg->FigSelect==NULL)
	{
		QMessageBox::information(this, tr("Figura no seleccionada"), tr("<p>Por favor seleccione alguna figura en la lista de figuras creadas."));
		return;
	}

	if(ui.pantallaWidget->drawFT->tablaFig->rowCount()>1 && ui.pantallaWidget->drawFT->vectorOfFig.at(0)!= ui.pantallaWidget->drawFT->FigSelect)
	{
		int seleccion = ui.pantallaWidget->drawFT->tablaFig->currentRow();
		if(seleccion!=-1){
			//guardo los nombres en sus respectivas figuras
			ui.pantallaWidget->drawFT->vectorOfFig[seleccion]->nombre = ui.pantallaWidget->drawFTSeg->tablaFig->item(seleccion,1)->text();
			ui.pantallaWidget->drawFT->vectorOfFig[seleccion-1]->nombre = ui.pantallaWidget->drawFTSeg->tablaFig->item(seleccion-1,1)->text();

			//cambio el arreglo de figuras
			swap(ui.pantallaWidget->drawFT->vectorOfFig[seleccion-1],ui.pantallaWidget->drawFT->vectorOfFig[seleccion]);
			//ahora en la tabla
			ui.pantallaWidget->drawFTSeg->insertArriba(seleccion,ui.pantallaWidget->drawFTSeg->tablaFig,ui.pantallaWidget->drawFTSeg->vectorOfFig);
			ui.pantallaWidget->drawFTSeg->insertArriba(seleccion-1,ui.pantallaWidget->drawFTSeg->tablaFig,ui.pantallaWidget->drawFTSeg->vectorOfFig);
			
			//actulizo los actuales
			ui.pantallaWidget->drawFTSeg->tablaFig->setCurrentCell(seleccion-1,0);
			ui.pantallaWidget->drawFTSeg->FigSelect = ui.pantallaWidget->drawFT->vectorOfFig[seleccion-1];
			//actulizo
			ui.pantallaWidget->drawFT->tablaFig->update();
		}else{
			QMessageBox::information(this, tr("Figura no seleccionada"), tr("<p>Por favor seleccione alguna figura en la lista de figuras creadas."));
			return;
		}
		
	}
}

void FTM::on_bajarFigClasi_clicked()
{
	if(ui.pantallaWidget->drawFT->FigSelect==NULL || ui.pantallaWidget->drawFTSeg->FigSelect==NULL)
	{
		QMessageBox::information(this, tr("Figura no seleccionada"), tr("<p>Por favor seleccione alguna figura en la lista de figuras creadas."));
		return;
	}

	if(ui.pantallaWidget->drawFT->tablaFig->rowCount()>1 && ui.pantallaWidget->drawFT->vectorOfFig.at(ui.pantallaWidget->drawFT->tablaFig->rowCount()-1)!= ui.pantallaWidget->drawFT->FigSelect)
	{
		int seleccion = ui.pantallaWidget->drawFT->tablaFig->currentRow();
		if(seleccion!=-1){
			//guardo los nombres en sus respectivas figuras
			ui.pantallaWidget->drawFT->vectorOfFig[seleccion]->nombre = ui.pantallaWidget->drawFTSeg->tablaFig->item(seleccion,1)->text();
			ui.pantallaWidget->drawFT->vectorOfFig[seleccion+1]->nombre = ui.pantallaWidget->drawFTSeg->tablaFig->item(seleccion+1,1)->text();

			//cambio el arreglo de figuras
			swap(ui.pantallaWidget->drawFT->vectorOfFig[seleccion+1],ui.pantallaWidget->drawFT->vectorOfFig[seleccion]);
			//ahora en la tabla
			ui.pantallaWidget->drawFTSeg->insertArriba(seleccion,ui.pantallaWidget->drawFTSeg->tablaFig,ui.pantallaWidget->drawFTSeg->vectorOfFig);
			ui.pantallaWidget->drawFTSeg->insertArriba(seleccion+1,ui.pantallaWidget->drawFTSeg->tablaFig,ui.pantallaWidget->drawFTSeg->vectorOfFig);
			
			//actualizo las figClasi actuales
			ui.pantallaWidget->drawFTSeg->tablaFig->setCurrentCell(seleccion+1,0);
			ui.pantallaWidget->drawFTSeg->FigSelect = ui.pantallaWidget->drawFT->vectorOfFig[seleccion+1];
			//actulizo
			ui.pantallaWidget->drawFT->tablaFig->update();
		}else{
			QMessageBox::information(this, tr("Figura no seleccionada"), tr("<p>Por favor seleccione alguna figura en la lista de figuras creadas."));
			return;
		}
		
	}
}

void FTM::on_eliminarFigClasi_clicked()
{
	int seleccion = ui.pantallaWidget->drawFT->tablaFig->currentRow();

	if(seleccion!=-1){
		
		if(ui.pantallaWidget->drawFTSeg->FigSelect==ui.pantallaWidget->drawFT->vectorOfFig.at(seleccion)) ui.pantallaWidget->drawFTSeg->FigSelect = NULL;
		ui.pantallaWidget->drawFT->vectorOfFig.remove(seleccion);

		ui.pantallaWidget->drawFTSeg->tablaFig->removeRow(seleccion);
		//actualizo
		ui.pantallaWidget->drawFT->tablaFig->update();
		//actualizo tambien las FTM
		ui.pantallaWidget->drawFTSeg->actualizaFtmElimina(NULL);

	}else{
			QMessageBox::information(this, tr("Figura no seleccionada"), tr("<p>Por favor seleccione alguna figura en la lista de figuras creadas."));
			return;
		}
}
void FTM::on_AutoRefresh_stateChanged (int estado)
{
	ui.refrescarVol->setEnabled(!estado);
	ui.pantallaWidget->drawFT->autoRefresh = estado;
	ui.pantallaWidget->drawFTSeg->autoRefresh = estado;

	if(ui.pantallaWidget->drawFT->autoRefresh)
		ui.pantallaWidget->updateVolumenFTM();
}

void FTM::on_sliderConFigIni_valueChanged(int valor)
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;

	//printf("valor ini:%f fin:%d\n",ui.widgetSetFigClasi->FigSelect->DegradadoIni,ui.widgetSetFigClasi->FigSelect->DegradadoFin);
	ui.widgetSetFigClasi->FigSelect->DegradadoIni = (float)valor/(float)100;
	if(ui.widgetSetFigClasi->FigSelect->DegradadoIni <= ui.widgetSetFigClasi->FigSelect->DegradadoFin){
		ui.widgetSetFigClasi->FigSelect->DegradadoIni = ui.widgetSetFigClasi->FigSelect->DegradadoFin + (float)1/(float)100;
		ui.sliderConFigIni->setSliderPosition((int)(ui.widgetSetFigClasi->FigSelect->DegradadoIni*100.0));
	}
	
	ui.widgetSetFigClasi->update();

	//actualizo solo las FTM o el Volumen segun auto refresh
	ui.AreaDraw->actualizarFTM(ui.widgetSetFigClasi->FigSelect);
	ui.AreaDrawSeg->actualizarFTMSeg(ui.widgetSetFigClasi->FigSelect);
	//if(ui.pantallaWidget->drawFT->autoRefresh)
	//	ui.pantallaWidget->updateVolumenFTM();
}

void FTM::on_sliderConFigFin_valueChanged(int valor)
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;

	//printf("valor ini:%f fin:%d\n",ui.widgetSetFigClasi->FigSelect->DegradadoIni,ui.widgetSetFigClasi->FigSelect->DegradadoFin);
	ui.widgetSetFigClasi->FigSelect->DegradadoFin = (float)valor/(float)100;
	if(ui.widgetSetFigClasi->FigSelect->DegradadoFin >= ui.widgetSetFigClasi->FigSelect->DegradadoIni){
		ui.widgetSetFigClasi->FigSelect->DegradadoFin = ui.widgetSetFigClasi->FigSelect->DegradadoIni - (float)1/(float)100;
		ui.sliderConFigFin->setSliderPosition((int)(ui.widgetSetFigClasi->FigSelect->DegradadoFin*100.0));
	}
	
	ui.widgetSetFigClasi->update();
	
	//actualizo solo las FTM o el Volumen segun auto refresh
	ui.AreaDraw->actualizarFTM(ui.widgetSetFigClasi->FigSelect);
	ui.AreaDrawSeg->actualizarFTMSeg(ui.widgetSetFigClasi->FigSelect);

	//if(ui.pantallaWidget->drawFT->autoRefresh)
	//	ui.pantallaWidget->updateVolumenFTM();
}
void FTM::on_sliderAlphaFin_valueChanged(int valor)
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;
	ui.widgetSetFigClasi->FigSelect->colorFin.setAlpha(valor);
	ui.widgetSetFigClasi->update();
	//actualizo solo las FTM o el Volumen segun auto refresh
	ui.AreaDraw->actualizarFTM(ui.widgetSetFigClasi->FigSelect);
	ui.AreaDrawSeg->actualizarFTMSeg(ui.widgetSetFigClasi->FigSelect);
}
void FTM::on_sliderAlphaIni_valueChanged(int valor)
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;
	ui.widgetSetFigClasi->FigSelect->colorIni.setAlpha(valor);
	ui.widgetSetFigClasi->update();
	//actualizo solo las FTM o el Volumen segun auto refresh
	ui.AreaDraw->actualizarFTM(ui.widgetSetFigClasi->FigSelect);
	ui.AreaDrawSeg->actualizarFTMSeg(ui.widgetSetFigClasi->FigSelect);
}
void FTM::on_sliderConFigIni_sliderReleased()
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;

	if(ui.pantallaWidget->drawFT->autoRefresh)
		ui.pantallaWidget->updateVolumenFTM();

}
void FTM::on_sliderConFigFin_sliderReleased()
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;

	if(ui.pantallaWidget->drawFT->autoRefresh)
		ui.pantallaWidget->updateVolumenFTM();
}
void FTM::on_sliderAlphaFin_sliderReleased()
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;

	if(ui.pantallaWidget->drawFT->autoRefresh)
		ui.pantallaWidget->updateVolumenFTM();
}
void FTM::on_sliderAlphaIni_sliderReleased()
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;

	if(ui.pantallaWidget->drawFT->autoRefresh)
		ui.pantallaWidget->updateVolumenFTM();
}
void FTM::label_Clicked(int tip)
{
	if(ui.widgetSetFigClasi->FigSelect==NULL) return;

	if(tip==1)
	{
		ui.widgetSetFigClasi->FigSelect->colorFin.setRgba( QColorDialog::getColor((ui.widgetSetFigClasi->FigSelect->colorFin),this,tr("Elija el color P1"),QColorDialog::ShowAlphaChannel).rgba());
		ui.pColorConf1->setPalette(QPalette(ui.widgetSetFigClasi->FigSelect->colorFin.rgb()));
	}else{
		ui.widgetSetFigClasi->FigSelect->colorIni.setRgba( QColorDialog::getColor((ui.widgetSetFigClasi->FigSelect->colorIni),this,tr("Elija el color P2"),QColorDialog::ShowAlphaChannel).rgba());
		ui.pColorConf2->setPalette(QPalette(ui.widgetSetFigClasi->FigSelect->colorIni.rgb()));
	}
	//actualizo solo las FTM o el Volumen segun auto refresh
	ui.AreaDraw->actualizarFTM(ui.widgetSetFigClasi->FigSelect);
	ui.AreaDrawSeg->actualizarFTMSeg(ui.widgetSetFigClasi->FigSelect);

	if(ui.pantallaWidget->drawFT->autoRefresh)
		ui.pantallaWidget->updateVolumenFTM();

}

void FTM::actualFig(int fil, int col)
{
	if(ui.widgetSetFigClasi->FigSelect==NULL && fil < ui.pantallaWidget->drawFT->vectorOfFig.size()) return;

	ui.pColorConf1->setPalette(QPalette(ui.pantallaWidget->drawFT->vectorOfFig[fil]->colorFin.rgb()));
	ui.pColorConf2->setPalette(QPalette(ui.pantallaWidget->drawFT->vectorOfFig[fil]->colorIni.rgb()));
	ui.sliderConFigFin->setSliderPosition((int)(ui.pantallaWidget->drawFT->vectorOfFig[fil]->DegradadoFin*100.0));
	ui.sliderConFigIni->setSliderPosition((int)(ui.pantallaWidget->drawFT->vectorOfFig[fil]->DegradadoIni*100.0));
	ui.sliderAlphaIni->setSliderPosition((ui.pantallaWidget->drawFT->vectorOfFig[fil]->colorIni.alpha()));
	ui.sliderAlphaFin->setSliderPosition((ui.pantallaWidget->drawFT->vectorOfFig[fil]->colorFin.alpha()));
}
void FTM::on_tamKernels_valueChanged(int valor)
{
	if(ui.listInterpola->count()<valor)
	{
		for(int i=ui.listInterpola->count(); i<valor; ++i)
		{
			ui.listInterpola->addItem(tr("0"));  ui.listInterpola->item(i)->setFlags (ui.listInterpola->item(i)->flags () | Qt::ItemIsEditable);
			ui.listPrimera->addItem(tr("0")); ui.listPrimera->item(i)->setFlags (ui.listPrimera->item(i)->flags () | Qt::ItemIsEditable);
			ui.listSegunda->addItem(tr("0")); ui.listSegunda->item(i)->setFlags (ui.listSegunda->item(i)->flags () | Qt::ItemIsEditable);
		}
	}else{
		if(ui.listInterpola->count()>valor)
		{
			for(int i=valor-1; i<ui.listInterpola->count(); ++i)
			{
				 delete ui.listInterpola->item(i);
				 delete ui.listPrimera->item(i);
				 delete ui.listSegunda->item(i);
			}
		}
	
	}	
}
void FTM::on_calculaKernels_clicked()
{
	//debo de validar los nros antes e reemplazarlos y calcular
	bool valida;
	for(int i=0; i<ui.listInterpola->count(); ++i)
	{
		//valido interpola
		ui.listInterpola->item(i)->text().toFloat(&valida);
		if(!valida){
			QMessageBox::information(this, tr("Número no válido!"), tr("<p>Existen números no válidos. El formato es el siguiente 000.000 ó 0"));
			ui.listInterpola->setCurrentRow(i);
			return;
		}
		//valido primera
		ui.listPrimera->item(i)->text().toFloat(&valida);
		if(!valida){
			QMessageBox::information(this, tr("Número no válido!"), tr("<p>Existen números no válidos. El formato es el siguiente 000.000 ó 0"));
			ui.listPrimera->setCurrentRow(i);
			return;
		}
		//valido segunda
		ui.listSegunda->item(i)->text().toFloat(&valida);
		if(!valida){
			QMessageBox::information(this, tr("Número no válido!"), tr("<p>Existen números no válidos. El formato es el siguiente 000.000 ó 0"));
			ui.listSegunda->setCurrentRow(i);
			return;
		}	 	
	}

	//ahora reseteo los kernels para proceder a re-calcular
	ui.pantallaWidget->MyFT->tamKer = ui.tamKernels->value();

	ui.pantallaWidget->MyFT->qkerInterpola.clear();
	ui.pantallaWidget->MyFT->qkerPrimera.clear();
	ui.pantallaWidget->MyFT->qkerSegunda.clear();

	for(int i=0; i<ui.listInterpola->count(); ++i)
	{
		ui.pantallaWidget->MyFT->qkerInterpola.push_back(ui.listInterpola->item(i)->text().toFloat()*ui.pantallaWidget->MyFT->divKer);
		ui.pantallaWidget->MyFT->qkerPrimera.push_back(ui.listPrimera->item(i)->text().toFloat()*ui.pantallaWidget->MyFT->divKer);
		ui.pantallaWidget->MyFT->qkerSegunda.push_back(ui.listSegunda->item(i)->text().toFloat()*ui.pantallaWidget->MyFT->divKer);
	}
	
	//ahora hay que recalcular todo los volúmenes y los Widgets FTM
	ui.pantallaWidget->MyFT->loadGradiente();
	ui.pantallaWidget->MyFT->loadSegDerivada();
	ui.pantallaWidget->reloadFT();

	//unsigned int * dat = MyFT->MagSegVolumenTextura;
	ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
	ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);

	//actulizo FTMs
	ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
	ui.pantallaWidget->drawFT->update();
	ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
	ui.pantallaWidget->drawFTSeg->update();
}

void FTM::on_actionDif_Centrales_triggered()
{
	if(ui.actionDif_Centrales->isChecked()){
		ui.actionKernels->setChecked(false);
		ui.actionPropiedades->setEnabled(false);
		ui.pantallaWidget->MyFT->selectMetodo = false;
		ui.dockWidgetKernels->setVisible(false);
		
		//ahora hay que recalcular todo los volúmenes y los Widgets FTM
		ui.pantallaWidget->MyFT->loadGradiente();
		ui.pantallaWidget->MyFT->loadSegDerivada();
		ui.pantallaWidget->reloadFT();

		//unsigned int * dat = MyFT->MagSegVolumenTextura;
		ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
		ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);

		//actulizo FTMs
		ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
		ui.pantallaWidget->drawFT->update();
		ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
		ui.pantallaWidget->drawFTSeg->update();

	}else{
		ui.actionKernels->setChecked(true);
		ui.actionPropiedades->setEnabled(true);
		ui.pantallaWidget->MyFT->selectMetodo = true;
	}
}
void FTM::on_actionKernels_triggered()
{
	if(ui.actionKernels->isChecked()){
		ui.actionDif_Centrales->setChecked(false);
		ui.actionPropiedades->setEnabled(true);
		ui.pantallaWidget->MyFT->selectMetodo = true;
	}else{
		ui.actionDif_Centrales->setChecked(true);
		ui.actionPropiedades->setEnabled(false);
		ui.pantallaWidget->MyFT->selectMetodo = false;
		ui.dockWidgetKernels->setVisible(false);

		//ahora hay que recalcular todo los volúmenes y los Widgets FTM
		ui.pantallaWidget->MyFT->loadGradiente();
		ui.pantallaWidget->MyFT->loadSegDerivada();
		ui.pantallaWidget->reloadFT();

		//unsigned int * dat = MyFT->MagSegVolumenTextura;
		ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
		ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);
		
		//actulizo FTMs
		ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
		ui.pantallaWidget->drawFT->update();
		ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
		ui.pantallaWidget->drawFTSeg->update();
	}
}
void FTM::on_actionPropiedades_triggered()
{
	ui.dockWidgetKernels->setVisible(true);
	ui.dockWidgetKernels->setFixedWidth(180);
}
void FTM::on_actionLaplaciano_triggered()
{
		ui.actionLaplaciano->setChecked(true);
		ui.actionGrad_del_Grad->setChecked(false);
		ui.actionM_Hessian->setChecked(false);
		ui.pantallaWidget->MyFT->tipoSegDerivada = 0;

		if(ui.actionDif_Centrales->isChecked()){

			//ahora hay que recalcular todo los volúmenes y los Widgets FTM
			ui.pantallaWidget->MyFT->loadGradiente();
			ui.pantallaWidget->MyFT->loadSegDerivada();
			ui.pantallaWidget->reloadFT();

			//unsigned int * dat = MyFT->MagSegVolumenTextura;
			ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
			ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);

			//actulizo FTMs
			ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
			ui.pantallaWidget->drawFT->update();
			ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
			ui.pantallaWidget->drawFTSeg->update();
	}
}
void FTM::on_actionGrad_del_Grad_triggered()
{
	ui.actionLaplaciano->setChecked(false);
	ui.actionGrad_del_Grad->setChecked(true);
	ui.actionM_Hessian->setChecked(false);
	ui.pantallaWidget->MyFT->tipoSegDerivada = 1;

	if(ui.actionDif_Centrales->isChecked()){

			//ahora hay que recalcular todo los volúmenes y los Widgets FTM
			ui.pantallaWidget->MyFT->loadGradiente();
			ui.pantallaWidget->MyFT->loadSegDerivada();
			ui.pantallaWidget->reloadFT();

			//unsigned int * dat = MyFT->MagSegVolumenTextura;
			ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
			ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);

			//actulizo FTMs
			ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
			ui.pantallaWidget->drawFT->update();
			ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
			ui.pantallaWidget->drawFTSeg->update();
	}
}
void FTM::on_actionM_Hessian_triggered()
{
	ui.actionLaplaciano->setChecked(false);
	ui.actionGrad_del_Grad->setChecked(false);
	ui.actionM_Hessian->setChecked(true);
	ui.pantallaWidget->MyFT->tipoSegDerivada = 2;

	if(ui.actionDif_Centrales->isChecked()){

			//ahora hay que recalcular todo los volúmenes y los Widgets FTM
			ui.pantallaWidget->MyFT->loadGradiente();
			ui.pantallaWidget->MyFT->loadSegDerivada();
			ui.pantallaWidget->reloadFT();

			//unsigned int * dat = MyFT->MagSegVolumenTextura;
			ui.pantallaWidget->loadVolumenMagSeg(ui.pantallaWidget->MyFT->MagSegVolumenTextura);
			ui.pantallaWidget->loadVolumenSeg(ui.pantallaWidget->MyFT->SegVolumenTextura);

			//actulizo FTMs
			ui.pantallaWidget->drawFT->paintInFTM(ui.pantallaWidget->drawFT->MyImagenAct);
			ui.pantallaWidget->drawFT->update();
			ui.pantallaWidget->drawFTSeg->paintInFTM(ui.pantallaWidget->drawFTSeg->MyImagenAct);
			ui.pantallaWidget->drawFTSeg->update();
	}
}
void FTM::on_actionMostrar_WidgetFTM_triggered()
{
	if(ui.pantallaWidget->T3d.TexNombre == NULL){
		ui.actionMostrar_WidgetFTM->setChecked(false);
		return;
	} 
	//para mostrar todo el dockwidget
	ui.dockWidget_3->setVisible(true);
} 
void FTM::on_actionMostrar_WidgetFTuni_triggered()
{
	if(ui.pantallaWidget->T3d.TexNombre == NULL){
		ui.actionMostrar_WidgetFTuni->setChecked(false);
		return;
	} 
	//muestro el dockwidget de la FTUni
	ui.dockWidgetFT1->setVisible(true);
}
void FTM::on_Push1D_clicked()
{
	ui.pantallaWidget->MyFT->nroDimensiones=1;
	//presiono los botones
	ui.Push1D->setEnabled(false);
	ui.Push2D->setEnabled(true);
	ui.Push3D->setEnabled(true);

	//ui.dockWidgetFT1->setEnabled(true);
	ui.groupBoxUni->setEnabled(true);

	//des habilito las otras ftm
	//ui.dockWidget_3->setEnabled(false);
	ui.groupBox_2->setEnabled(false);
	ui.groupBox->setEnabled(false);
	ui.ConFigClasi->setEnabled(false);

	ui.Push1DFTUni->setEnabled(false);
	ui.dockWidgetFT1->setVisible(true);

}
void FTM::on_Push2D_clicked()
{
	ui.pantallaWidget->MyFT->nroDimensiones=2;
	//presiono los botones
	ui.Push1D->setEnabled(true);
	ui.Push2D->setEnabled(false);
	ui.Push3D->setEnabled(true);
	//des habilito las otras ftm
	// deshabilito la 3er dimensi'on
	ui.dockWidget_3->setEnabled(true);
	ui.pantallaWidget->drawFTSeg->setEnabled(false);
	ui.scrollArea_2->setVisible(false);
	ui.EjeZ->setVisible(false);
	ui.EjeX2->setVisible(false);
	ui.cero_2->setVisible(false);
	//actualizo las figuras
	ui.pantallaWidget->updateVolumenFTM();
}
void FTM::on_Push3D_clicked()
{
	ui.pantallaWidget->MyFT->nroDimensiones=3;
	//presiono los botones
	ui.Push1D->setEnabled(true);
	ui.Push2D->setEnabled(true);
	ui.Push3D->setEnabled(false);

	//des habilito las otras ftm
	ui.dockWidget_3->setEnabled(true);
	ui.pantallaWidget->drawFTSeg->setEnabled(true);
	ui.scrollArea_2->setVisible(true);
	ui.EjeZ->setVisible(true);
	ui.EjeX2->setVisible(true);
	ui.cero_2->setVisible(true);
	//ojo hacer update de las figuras
	ui.pantallaWidget->updateVolumenFTM();
}
void FTM::on_Push1DFTUni_clicked()
{

}
void FTM::on_Push2Do3DFTUni_clicked()
{
	ui.pantallaWidget->MyFT->nroDimensiones=3;
	//presiono los botones
	ui.Push1D->setEnabled(true);
	ui.Push2D->setEnabled(true);
	ui.Push3D->setEnabled(false);

	//ui.dockWidgetFT1->setEnabled(false);
	ui.groupBoxUni->setEnabled(false);

	//des habilito las otras ftm
	//ui.dockWidget_3->setEnabled(true);
	ui.groupBox_2->setEnabled(true);
	ui.groupBox->setEnabled(true);
	ui.ConFigClasi->setEnabled(true);

	ui.pantallaWidget->drawFTSeg->setEnabled(true);
	ui.scrollArea_2->setVisible(true);
	ui.EjeZ->setVisible(true);
	ui.EjeX2->setVisible(true);
	ui.cero_2->setVisible(true);
	//ojo hacer update de las figuras
	ui.pantallaWidget->updateVolumenFTM();

}
void FTM::on_sliderTruncaFTMWidget_valueChanged(int valor)
{
	ui.pantallaWidget->truncaFtmWidget = valor;
	ui.spinActualFTMhisto2D->setValue(valor);

	ui.pantallaWidget->updateHistoFTMWidget();
	//actualizo solo las FTM o el Volumen segun auto refresh
	ui.AreaDraw->actualizarFTM(ui.widgetSetFigClasi->FigSelect);
	ui.AreaDrawSeg->actualizarFTMSeg(ui.widgetSetFigClasi->FigSelect);
}

void FTM::on_spinMinFTMhisto2D_valueChanged(int valor)
{
	ui.sliderTruncaFTMWidget->setMinimum(valor);
	ui.spinActualFTMhisto2D->setMinimum(valor);
}
void FTM::on_spinMaxFTMhisto2D_valueChanged(int valor)
{
	ui.sliderTruncaFTMWidget->setMaximum(valor);
	ui.spinActualFTMhisto2D->setMaximum(valor);
}
void FTM::on_spinActualFTMhisto2D_valueChanged(int valor)
{
	ui.sliderTruncaFTMWidget->setValue(valor);
	on_sliderTruncaFTMWidget_valueChanged(valor);
}
void FTM::on_lineEditValor_returnPressed()
{
	QString aux = ui.lineEditValor->text();
	bool ok;
	aux.toFloat(&ok);
	if(!ok)
	{
		QMessageBox::information(this, tr("Número no válido!"), tr("<p>Existen números no válidos. El formato es el siguiente 000.000 ó 0"));
		return;
	}
	ui.pantallaWidget->T3d.medVoxel = aux.toFloat(&ok);
}
void FTM::on_spinMinX_valueChanged(int valor)
{
	printf("fino:%f\n",ui.pantallaWidget->minTex[0]);
	ui.pantallaWidget->minTex[0] = ui.pantallaWidget->T3d.medVoxel*(float)valor;
	printf("fino2:%f\n",ui.pantallaWidget->minTex[0]);
}
void FTM::on_spinMinY_valueChanged(int valor)
{
	ui.pantallaWidget->minTex[1] = ui.pantallaWidget->T3d.medVoxel*(float)valor;
}
void FTM::on_spinMinZ_valueChanged(int valor)
{
	ui.pantallaWidget->minTex[2] = ui.pantallaWidget->T3d.medVoxel*(float)valor;
}
void FTM::on_spinMaxX_valueChanged(int valor)
{
	ui.pantallaWidget->maxTex[0] = 1.0f - ui.pantallaWidget->T3d.medVoxel*(float)valor;
}
void FTM::on_spinMaxY_valueChanged(int valor)
{
	ui.pantallaWidget->maxTex[1] = 1.0f - ui.pantallaWidget->T3d.medVoxel*(float)valor;
}
void FTM::on_spinMaxZ_valueChanged(int valor)
{
	ui.pantallaWidget->maxTex[2] = 1.0f - ui.pantallaWidget->T3d.medVoxel*(float)valor;
}
void FTM::on_doubleSpinMuestreo_valueChanged(double valor)
{
	ui.pantallaWidget->ray_step = (float)valor;
}
void FTM::on_sliderEscalaX_valueChanged(int valor)
{
	ui.pantallaWidget->T3d.DSX = ((float)valor/50.0f);
}
void FTM::on_sliderEscalaY_valueChanged(int valor)
{
	ui.pantallaWidget->T3d.DSY = ((float)valor/50.0f);
}
void FTM::on_sliderEscalaZ_valueChanged(int valor)
{
	ui.pantallaWidget->T3d.DSZ = ((float)valor/50.0f);
}
void FTM::on_actionPropiedadesVol_triggered()
{
	ui.dockWidgetVol->setVisible(true);
}
void FTM::on_actionMoverIlu_triggered()
{
	ui.pantallaWidget->moverIlumina = ui.actionMoverIlu->isChecked();
}
void FTM::on_radioGradiente_clicked()
{
	if(ui.pantallaWidget->opcionIlumina == 0.0f) return;

	ui.Lambiental->setEnabled(false);
	ui.Ldifusa->setEnabled(false);
	ui.Lespecular->setEnabled(false);
	ui.pushAmbiental->setEnabled(false);
	ui.pushDifusa->setEnabled(false);
	ui.pushEspecular->setEnabled(false);
	ui.pantallaWidget->opcionIlumina = 1.0f;
}
void FTM::on_radioSegDerivada_clicked()
{
	if(ui.pantallaWidget->opcionIlumina == 0.0f) return;

	ui.Lambiental->setEnabled(false);
	ui.Ldifusa->setEnabled(false);
	ui.Lespecular->setEnabled(false);
	ui.pushAmbiental->setEnabled(false);
	ui.pushDifusa->setEnabled(false);
	ui.pushEspecular->setEnabled(false);
	ui.pantallaWidget->opcionIlumina = 2.0f;

}
void FTM::on_radioMagYSeg_clicked()
{
	if(ui.pantallaWidget->opcionIlumina == 0.0f) return;

	ui.Lambiental->setEnabled(false);
	ui.Ldifusa->setEnabled(false);
	ui.Lespecular->setEnabled(false);
	ui.pushAmbiental->setEnabled(false);
	ui.pushDifusa->setEnabled(false);
	ui.pushEspecular->setEnabled(false);

	ui.pantallaWidget->opcionIlumina = 3.0f;

}
void FTM::on_radioBlinnPhong_clicked()
{
	if(ui.pantallaWidget->opcionIlumina == 0.0f) return;

	ui.Lambiental->setEnabled(true);
	ui.Ldifusa->setEnabled(true);
	ui.Lespecular->setEnabled(true);
	ui.pushAmbiental->setEnabled(true);
	ui.pushDifusa->setEnabled(true);
	ui.pushEspecular->setEnabled(true);

	//opci'on de phong
	ui.pantallaWidget->opcionIlumina = 4.0f;

}
void FTM::on_pushAmbiental_clicked()
{
	QColor aux(ui.pantallaWidget->Amb_color[0]*255,ui.pantallaWidget->Amb_color[1]*255,ui.pantallaWidget->Amb_color[2]*255,ui.pantallaWidget->Amb_color[3]*255); 
	aux = QColorDialog::getColor(aux,this,tr("Elija el color Ambiental"),QColorDialog::ShowAlphaChannel).rgba();
	ui.Lambiental->setPalette(QPalette(aux));
	ui.pantallaWidget->Amb_color[0] = aux.redF();ui.pantallaWidget->Amb_color[1] = aux.greenF();ui.pantallaWidget->Amb_color[2] = aux.blueF();ui.pantallaWidget->Amb_color[3] = aux.alphaF();
}
void FTM::on_pushDifusa_clicked()
{
	QColor aux(ui.pantallaWidget->Dif_color[0]*255,ui.pantallaWidget->Dif_color[1]*255,ui.pantallaWidget->Dif_color[2]*255,ui.pantallaWidget->Dif_color[3]*255); 
	aux = QColorDialog::getColor(aux,this,tr("Elija el color Difuso"),QColorDialog::ShowAlphaChannel).rgba();
	ui.Ldifusa->setPalette(QPalette(aux));
	ui.pantallaWidget->Dif_color[0] = aux.redF();ui.pantallaWidget->Dif_color[1] = aux.greenF();ui.pantallaWidget->Dif_color[2] = aux.blueF();ui.pantallaWidget->Dif_color[3] = aux.alphaF();

}
void FTM::on_pushEspecular_clicked()
{
	QColor aux(ui.pantallaWidget->Esp_color[0]*255,ui.pantallaWidget->Esp_color[1]*255,ui.pantallaWidget->Esp_color[2]*255,ui.pantallaWidget->Esp_color[3]*255); 
	aux = QColorDialog::getColor(aux,this,tr("Elija el color Especular"),QColorDialog::ShowAlphaChannel).rgba();
	ui.Lespecular->setPalette(QPalette(aux));
	ui.pantallaWidget->Esp_color[0] = aux.redF();ui.pantallaWidget->Esp_color[1] = aux.greenF();ui.pantallaWidget->Esp_color[2] = aux.blueF();ui.pantallaWidget->Esp_color[3] = aux.alphaF();
}
void FTM::on_actionActivarIlu_triggered()
{
	ui.pantallaWidget->opcionIlumina = (float)ui.actionActivarIlu->isChecked()*4;
	ui.radioBlinnPhong->setChecked(true);
}
void FTM::on_actionPropiedadesIlu_triggered()
{
	if(ui.pantallaWidget->opcionIlumina == 0.0f) return;

	ui.dockWidgetIlumina->setVisible(true);
}
void FTM::on_doubleSpinShinIlumina_valueChanged(double valor)
{
	ui.pantallaWidget->shininessIlumina = (float)valor;
}
void FTM::on_tablaFigClasi_itemSelectionChanged()
{
	//printf("cambio la vaina");
}
void FTM::actulizaEstadoConfClasi()
{
	//re seteo la fig de config
	ui.sliderConFigIni->setSliderPosition(255);
	ui.sliderConFigFin->setSliderPosition(0);
	ui.sliderAlphaIni->setSliderPosition(255);
	ui.sliderAlphaFin->setSliderPosition(0);
	ui.pColorConf1->setPalette(QPalette(Qt::black));
	ui.pColorConf2->setPalette(QPalette(Qt::white));
}