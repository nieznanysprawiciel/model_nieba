#include "DrawingArea.h"
#include <QPainter>


DrawingArea::DrawingArea(QtSkyDisplay* displayer, int sizeX, int sizeY, QWidget *parent) :
    QWidget(parent)
{
    sky_display = displayer;
    setFixedSize(sizeX, sizeY);
}

void DrawingArea::generate_sky(int pixX, int pixY, float view_radius,
							   double albiedoR, double albiedoG,
							   double albiedoB, float horizontal_angle,
							   float vertical_angle, int turbid)
{
	//zmieniamy rozmiar widgetu, jeżeli zmienił się żądany rozmiar obrazka
	if( QSize(pixX, pixY) != size () )
		setFixedSize(pixX,pixY);
	//ustawiamy parametry nieba
	sky_display->set_params(pixX, pixY, view_radius,
							albiedoR, albiedoG,
							albiedoB, horizontal_angle,
							vertical_angle, turbid);

    sky_display->sky_display_multithreads();    //wersja wielowątkowa
    //sky_display->sky_display();       //wersja jednowątkowa
	repaint();			//musimy odświerzyć zawartość widgetu
}


void DrawingArea::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

    QPainter painter(this);
    /*painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(rect(), Qt::AlignCenter, "Qt");*/
    sky_display->paint_event(&painter);
}
