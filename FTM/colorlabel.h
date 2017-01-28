#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QColor>
#include <QColorDialog>

class colorLabel : public QLabel
{
	Q_OBJECT

public:
	int tipo;
	colorLabel(QWidget *parent);
	~colorLabel();
signals:
	void clicked(int tip);
private:
	void mousePressEvent(QMouseEvent * ev);
	
};

#endif // COLORLABEL_H
