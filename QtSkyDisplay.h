#ifndef QTSKYDISPLAY_H
#define QTSKYDISPLAY_H

#include <QPainter>
#include <QImage>
#include <QTime>
#include <QLabel>
#include "declarations.h"
#include "completeskymodel.h"
#include "sky_thread.h"


#define THREADS 8


class QtSkyDisplay : public QObject
{
Q_OBJECT
public:
    QtSkyDisplay(int pixX, int pixY, QLabel *time);
    ~QtSkyDisplay();

    void sky_display();
    void sky_display_multithreads();
	void save_image(QString& file_name);
    void paint_event(QPainter* painter);
	void dithering();
    void random_noise();

    void set_params(int pixX, int pixY, float view_angle,
                    double albiedoR, double albiedoG, double albiedoB,
                    float horizontal_angle, float vertical_angle,
                    int turbid);
	void set_sky_intensity(float);
	void set_solar_intensity(float);
	void set_solar_elevation(float elevation);
	void set_dithering(int level);

    //zmienne
    SkyData             sky_data;
    CompleteSkyModel*   sky_model;
    double              albiedo[3];
    int                 turbidity;
	vec3				sun;
    int*                color_buffer;
    QLabel*             time_of_execution;
	int					dithering_level;

    int                 threads_count;
    int                 ideal_threads;
    QTime               start_of_exec;
    sky_thread*         threads[THREADS];

private slots:
    void end_thread();

signals:
    void sky_completed();
};

#endif // QTSKYDISPLAY_H
