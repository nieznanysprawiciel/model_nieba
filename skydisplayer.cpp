#include "skydisplayer.h"
#include <QPainter>
#include <ctime>

SkyDisplayer::SkyDisplayer( int sizeX, int sizeY, QWidget *parent ) :
	QWidget(parent)
{
	setFixedSize(sizeX, sizeY);

	sky_data.horizontal_angle   =   0;
	sky_data.vertical_angle     =   0;
	sky_data.horizontal_pixels  =   sizeX;
	sky_data.vertical_pixels    =   sizeY;
	sky_data.near_plane         =   sizeX;

	for( int i = 0; i < 10; ++i )
		sky_data.albedo[i] = 0.8;


	sky_data.turbidity = 1;
	dithering_level = 0;
	ideal_threads = QThread::idealThreadCount();
	//ideal_threads = 1;			// For debug
	if( ideal_threads > THREADS )
		ideal_threads = THREADS;

	color_buffer = new int[sizeX * sizeY];
	vec3 zenith = vec3(0,1,0);
	sun_vector = vec3(0,1,-1);

	for(int i= 0 ; i < sizeX*sizeY; ++i)
		color_buffer[i] = 0xFFFFFFFF;

	sky_model = new CompleteSkyModel();
	sky_model->init(color_buffer, sizeX, sizeY, sizeX,
					sky_data.albedo, sky_data.turbidity,
					zenith, sun_vector);

	repaint_timer.setInterval(1000);
	repaint_timer.stop();       //pewnie to zbędne
	//timer odświeżający widok w trakcie generowania
	connect(&repaint_timer, SIGNAL(timeout()),this, SLOT(repaint()));
}


void SkyDisplayer::generate_sky(int pixX, int pixY, float view_angle,
							   double *albiedo, float horizontal_angle,
							   float vertical_angle, float turbid)
{
	//zmieniamy rozmiar widgetu, jeżeli zmienił się żądany rozmiar obrazka
	if( QSize(pixX, pixY) != size () )
		setFixedSize(pixX,pixY);
	//ustawiamy parametry nieba
	set_params( pixX, pixY, view_angle,
				albiedo, horizontal_angle,
				vertical_angle, turbid);

	repaint_timer.start();
	sky_display_multithreads();    //wersja wielowątkowa
	//sky_display();               //wersja jednowątkowa
	repaint();			//musimy odświerzyć zawartość widgetu
}


void SkyDisplayer::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);

	QRect copy_rect(0,0,sky_data.horizontal_pixels,sky_data.vertical_pixels);
	QImage sky_image( (uchar*)color_buffer, sky_data.horizontal_pixels,
					 sky_data.vertical_pixels, QImage::Format_RGB32 );

	painter.drawImage( copy_rect, sky_image );
}



void SkyDisplayer::set_params(int pixX, int pixY, float view_angle,
				double* albedo,	float horizontal_angle,
				float vertical_angle, float turbid)
{
	sky_data.horizontal_angle   =   radians(-horizontal_angle);
	sky_data.vertical_angle     =   radians(vertical_angle);
	sky_data.view_angle			=	view_angle;

	if(	sky_data.horizontal_pixels != pixX
		|| sky_data.vertical_pixels	!= pixY )
	{//musimy zmienić rozmiar tablicy
		delete[] color_buffer;
		color_buffer = new int[pixX*pixY];

		sky_data.horizontal_pixels  =   pixX;
		sky_data.vertical_pixels    =   pixY;
	}


	int near_plane = (int)(glm::sqrt((float)(pixX*pixX)+(float)(pixY*pixY))
						   /(2*glm::tan(radians(view_angle/2))));
	sky_data.near_plane = near_plane;
	sky_data.turbidity = turbid;

	for( unsigned int i = 0; i < 9; ++i )
		sky_data.albedo[i] = albedo[i];
}




void SkyDisplayer::set_dithering(int level)
{
	//dithering_level = level;
	if( level != 0 )
		dithering_level = 255;		// Przekształcamy obrazek z 16 bit na kanał do RGB
	else
		dithering_level = 0;
}


void SkyDisplayer::set_sky_intensity(float intensity)
{
	sky_model->set_sky_intensity( intensity );
}

void SkyDisplayer::set_solar_intensity(float intensity)
{
	sky_model->set_solar_intensity( intensity );
}

void SkyDisplayer::set_solar_elevation(float elevation)
{
	float rad_elevation = glm::radians(elevation);
	sun_vector.x = 0.0;
	sun_vector.y = glm::sin(rad_elevation);
	sun_vector.z = -glm::cos(rad_elevation);
}


void SkyDisplayer::sky_display_multithreads()
{
	vec3    euler_angles(sky_data.vertical_angle,sky_data.horizontal_angle,0.0);
	quat    rotation(euler_angles);

	start_of_exec.restart();

	sky_model->init(color_buffer, sky_data.horizontal_pixels,
					sky_data.vertical_pixels, sky_data.near_plane,
					sky_data.albedo, sky_data.turbidity, sun_vector);

	sky_model->set_rotation(rotation);
	threads_count = ideal_threads;

	//przydzielamy wątkom zakres działania
	unsigned int offset = 0;
	unsigned int counter = sky_data.horizontal_pixels*sky_data.vertical_pixels / threads_count;
	for( int i = 0; i < threads_count - 1; ++i )
	{
		threads[i] = new sky_thread(sky_model, offset , offset + counter );
		offset += counter;
	}
	//liczba pikseli mogła być niepodzielna przez liczbę wątków, dlatego ostatni wątek idzie do końca tablicy
	threads[threads_count - 1] =  new sky_thread(sky_model, offset,
								sky_data.horizontal_pixels*sky_data.vertical_pixels);


	for( int i = 0; i < threads_count; ++i )
		connect(threads[i],SIGNAL(finished()),this,SLOT(end_thread()));


	for( int i = 0; i < threads_count; ++i )
		threads[i]->start();
///////////////

}

void SkyDisplayer::end_thread()
{
	--threads_count;
	if( threads_count > 0 )
		return;

	last_generation_time = start_of_exec.elapsed();

	//kasujemy wątki
	for(int i = 0; i < ideal_threads; ++i)
		delete threads[i];

	unsigned short* R = sky_model->get_R16_buffer();
	unsigned short* G = sky_model->get_G16_buffer();
	unsigned short* B = sky_model->get_B16_buffer();
	dithering(R, G, B);
	copy_from_16bit(R, G, B);
	//random_noise();

	repaint_timer.stop();
	emit sky_completed();
}


void SkyDisplayer::save_image(QString& file_name)
{
	QImage sky_image((uchar*)color_buffer, sky_data.horizontal_pixels,
					 sky_data.vertical_pixels, QImage::Format_RGB32);
	sky_image.save(file_name,0,100);
}


/**Używa algorytmu Floyda-Steinberga do obrazka.
Aby włączyć dithering trzeba ustawić zmienną dithering_level
na wartość większą niż 1.*/
void SkyDisplayer::dithering(unsigned short* R, unsigned short* G, unsigned short* B)
{
	if( dithering_level < 2 )	//nie mamy co robić
		return;

	int vert_pix = sky_data.vertical_pixels;
	int hor_pix = sky_data.horizontal_pixels;

	//Będziemy przetwarzać piksele w poziomie. Błędy propagują się
	//najdalej o jeden piksel w związku z czym potrzebujemy tablicy,
	//która pomieści dwa wiersze obrazka
	float* error_table = new float[hor_pix<<1];
	unsigned short* color_buff = NULL;

	for( int j = 0; j < 3; ++j )
	{   // Iterujemy po kanałach
		if( j == 0 )
			color_buff = R;
		else if( j == 1 )
			color_buff = G;
		else
			color_buff = B;

		for( int i=0; i < hor_pix*2; ++i )
			error_table[i] = 0;		//zerujemy pamięć

		for( int vert = 0; vert < vert_pix; ++vert )
		{//przechodzimy obrazek w pionie

			for( int hor = 0; hor < hor_pix; ++hor )
			{//przechodzimy obrazek w poziomie
				unsigned short pixel = color_buff[vert*hor_pix + hor];		//pobieramy piksel
				float fpix = (float)pixel + error_table[hor];	//konwertujemy na float i dodajemy błąd

				int pix;
				float pix_error;
				pix_error = fmod(fpix, (float)dithering_level);	//liczymy błąd
				if( pix_error < (float)dithering_level/(float)2)
					pix = (int)(fpix - pix_error);
				else
					pix = (int)(fpix - pix_error + dithering_level),
					pix_error -= dithering_level;

				if( pix > 0xFFFF )
				{//jeżeli wartość wyszła poza zakres, to obcinamy
				//a to co zostanie propagujemy razem z błędem
					pix_error += pix - 0xFFFF;
					pix = 0xFFFF;
				}

								//dodajemy błąd do tabeli z odpowiednimi wagami
				error_table[hor_pix + hor] += (float)5*pix_error/(float)16;
				//na brzegach obrazka błędy po prostu olewamy
				if( hor != 0 )
					error_table[hor_pix + hor - 1] += (float)3*pix_error/(float)16;
				if( hor != hor_pix - 1)
				{
					error_table[hor + 1] += (float)7*pix_error/(float)16;
					error_table[hor_pix + hor + 1] += pix_error/(float)16;
				}

				color_buff[vert*hor_pix + hor] = pix;	//wstawiamy do pamięci

			}
			//przenosimy roboczy wiersz tabeli do góry, a dolny wiersz zerujemy
			for( int i = 0; i < hor_pix; ++i )
				error_table[i] = error_table[hor_pix + i], error_table[hor_pix + i] = 0;
		}
	}

	delete [] error_table;
}

/**
   @brief QtSkyDisplay::copy_from_16bit przekształca tablicę ze składowymi 16 bitowymi
   na format RGB i wstawia do tablicy w zmiennej color_buffer.
   @param R Tablica z barwą czerwoną.
   @param G Tablica z barwą zieloną.
   @param B Tablica z barwą niebieską.
 */
void SkyDisplayer::copy_from_16bit(unsigned short* R, unsigned short* G, unsigned short* B)
{
	// Jeżeli nie robiliśmy ditheringu, to nie ma po co
	if( dithering_level < 2 )
		return;

	int vert_pix = sky_data.vertical_pixels;
	int hor_pix = sky_data.horizontal_pixels;

	for( int vert = 0; vert < vert_pix; ++vert )
	{//przechodzimy obrazek w pionie

		for( int hor = 0; hor < hor_pix; ++hor )
		{//przechodzimy obrazek w poziomie
			// Skalujemy do 8 bitów na kanał
			int RED = R[vert*hor_pix + hor] / 255;
			int GREEN = G[vert*hor_pix + hor] / 255;
			int BLUE = B[vert*hor_pix + hor] / 255;

			int pixel = (RED << 16) | (GREEN << 8) | BLUE;

			color_buffer[vert*hor_pix + hor] = pixel;	//wstawiamy do pamięci
		}
	}
}



