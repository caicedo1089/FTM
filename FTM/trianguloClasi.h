#ifndef TRIANGULOCLASI_H
#define TRIANGULOCLASI_H

#include "clasificaFig.h"

class trianguloClasi : 
	public clasificaFig
{
public:
	trianguloClasi();
	~trianguloClasi();
	//pintar en el widget de valor de los datos y la mag del gradiente
	void pinta(QImage* tabla, int Tam)const;
	void pintaInFTM(QImage* tabla)const;
	void pintaInFTMSelect(QImage* tabla)const;

	//pintar en el widget de valor de los datos y la segunda derivada
	void pintaSeg(QImage* tabla, int Tam)const;
	void pintaInFTMSeg(QImage* tabla, int Tam)const;
	void pintaInFTMSelectSeg(QImage* tabla, int Tam)const;

private:
	
};

#endif // TRIANGULOCLASI_H
