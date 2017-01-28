#include "colorlabel.h"

colorLabel::colorLabel(QWidget *parent)
	: QLabel(parent)
{
}

colorLabel::~colorLabel()
{

}

void colorLabel::mousePressEvent(QMouseEvent * ev)
{
	emit clicked(tipo);
}
