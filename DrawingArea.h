#ifndef DRAWING_AREA_H
#define DRAWING_AREA_H

#include <QWidget>
#include "QtSkyDisplay.h"

class DrawingArea : public QWidget
{
    Q_OBJECT
private:
    QtSkyDisplay* sky_display;

public:
    explicit DrawingArea( QtSkyDisplay* displayer,
                         int sizeX, int sizeY, QWidget *parent = 0 );

	void generate_sky(int pixX, int pixY, float view_radius,
					  double albiedoR, double albiedoG, double albiedoB,
					  float horizontal_angle, float vertical_angle,
					  int turbid);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

};

#endif // DRAWING_AREA_H
