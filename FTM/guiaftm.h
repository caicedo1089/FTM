#ifndef GUIAFTM_H
#define GUIAFTM_H

#include <QGLWidget>

class guiaFTM : public QGLWidget
{
	Q_OBJECT

public:
	guiaFTM(QWidget *parent);
	~guiaFTM();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);

private:
	
};

#endif // GUIAFTM_H
