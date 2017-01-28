#ifndef CIRCULOCLASI_H
#define CIRCULOCLASI_H

#pragma once
#include "clasificafig.h"


class circuloClasi:
	public clasificaFig
{
public:
	circuloClasi();
	~circuloClasi();

	////pintar en el widget de valor de los datos y la mag del gradiente
	void pinta(QImage* tabla, int Tam)const;
	void pintaInFTM(QImage* tabla)const;
	void pintaInFTMSelect(QImage* tabla)const;

	////pintar en el widget de valor de los datos y la segunda derivada
	void pintaSeg(QImage* tabla, int Tam)const;
	void pintaInFTMSeg(QImage* tabla, int Tam)const;
	void pintaInFTMSelectSeg(QImage* tabla, int Tam)const;

private:
	
};

#endif // CIRCULOCLASI_H
