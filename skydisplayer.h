#ifndef SKYDISPLAYER_H
#define SKYDISPLAYER_H

#include <QWidget>
#include <QTime>
#include "declarations.h"
#include "completeskymodel.h"
#include "sky_thread.h"


/**@fixme Liczba wątków jest taka jaka wartość zmiennej ideal_threads, więc może się stać
coś złego jeżeli tam będzie za dużo.*/
#define THREADS 8

/**
   @brief Klasa SkyDisplayer jest widgetem, na którym zostanie narysowane niebo
   oraz przechowuje wszystko co tylko jest potrzebne do jego wyświetlenia i wygenerowania.


 */
class SkyDisplayer : public QWidget
{
	Q_OBJECT
private:
	SkyData				sky_data;
	CompleteSkyModel*   sky_model;
	vec3				sun_vector;
	int*                color_buffer;
	int					dithering_level;

	int                 threads_count;
	int                 ideal_threads;
	sky_thread*         threads[THREADS];

	QTime               start_of_exec;
	int					last_generation_time;
public:
	explicit SkyDisplayer( int sizeX, int sizeY, QWidget *parent = 0 );

	void generate_sky(int pixX, int pixY, float view_radius,
					  double albiedoR, double albiedoG, double albiedoB,
					  float horizontal_angle, float vertical_angle,
					  int turbid);

	void save_image(QString& file_name);


	void set_sky_intensity(float);
	void set_solar_intensity(float);
	void set_solar_elevation(float elevation);
	void set_dithering(int level);
	void set_params(int pixX, int pixY, float view_angle,
					double albedoR, double albedoG, double albedoB,
					float horizontal_angle, float vertical_angle,
					int turbid);
protected:
	void paintEvent(QPaintEvent *event);


	void sky_display();
	void sky_display_multithreads();
	void dithering();
	void dithering(unsigned short* R, unsigned short* G, unsigned short* B);
	void copy_from_16bit(unsigned short* R, unsigned short* G, unsigned short* B);
	void random_noise();
signals:
	void sky_completed();
private slots:
	void end_thread();
};

#endif // SKYDISPLAYER_H

