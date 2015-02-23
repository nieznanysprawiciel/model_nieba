#include "QtSkyDisplay.h"
#include <QTime>
#include <QString>
#include <cstdlib>
#include <ctime>
#include "sky_thread.h"
//#include <math.h>

QtSkyDisplay::QtSkyDisplay(int pixX, int pixY, QLabel *time)
{
    time_of_execution = time;

    sky_data.horizontal_angle   =   0;
    sky_data.vertical_angle     =   0;
    sky_data.horizontal_pixels  =   pixX;
    sky_data.vertical_pixels    =   pixY;
    sky_data.near_plane         =   pixX;

    albiedo[0] = 0.8;
    albiedo[1] = 0.8;
    albiedo[2] = 0.8;

    turbidity = 1;
	dithering_level = 0;
    ideal_threads = QThread::idealThreadCount();

    color_buffer = new int[pixX*pixY];
	vec3 zenith = vec3(0,1,0);
	sun = vec3(0,1,-1);

	for(int i= 0 ; i < pixX*pixY; ++i)
        color_buffer[i] = 0xFFFFFFFF;

    sky_model = new CompleteSkyModel(VERSION_RGB, 3);
    sky_model->init(color_buffer, pixX, pixY, pixX, albiedo, turbidity, zenith, sun);
}

QtSkyDisplay::~QtSkyDisplay()
{
	delete[] color_buffer;
    delete sky_model;
}

void QtSkyDisplay::sky_display(/*QPainter* painter*/)
{
    vec3    euler_angles(sky_data.vertical_angle,sky_data.horizontal_angle,0.0);
    quat    rotation(euler_angles);
    QTime   start_of_exec;
    int     time_elapsed;

    sky_model->init(color_buffer, sky_data.horizontal_pixels,
                    sky_data.vertical_pixels, sky_data.near_plane,
					albiedo, turbidity, sun);

    start_of_exec.start();
    sky_model->execute(rotation);
    time_elapsed = start_of_exec.elapsed();

	dithering();
    //random_noise();

    time_of_execution->setText( QString::number(time_elapsed) );
}

void QtSkyDisplay::sky_display_multithreads()
{
    vec3    euler_angles(sky_data.vertical_angle,sky_data.horizontal_angle,0.0);
    quat    rotation(euler_angles);

    sky_model->init(color_buffer, sky_data.horizontal_pixels,
                    sky_data.vertical_pixels, sky_data.near_plane,
                    albiedo, turbidity, sun);

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
    //liczba pikseli mogła być niepodzielna przez 8, dlatego ostatni wątek idzie do końca tablicy
    threads[threads_count - 1] =  new sky_thread(sky_model, offset,
                                sky_data.horizontal_pixels*sky_data.vertical_pixels);


    for( int i = 0; i < threads_count; ++i )
        connect(threads[i],SIGNAL(finished()),this,SLOT(end_thread()));

    start_of_exec.restart();
    for( int i = 0; i < threads_count; ++i )
        threads[i]->start();
///////////////

}

void QtSkyDisplay::end_thread()
{
    int time_elapsed;

    --threads_count;
    if( threads_count > 0 )
        return;

    time_elapsed = start_of_exec.elapsed();

    //kasujemy wątki
    for(int i = 0; i < ideal_threads; ++i)
        delete threads[i];

	unsigned short* R = sky_model->get_R16_buffer();
	unsigned short* G = sky_model->get_G16_buffer();
	unsigned short* B = sky_model->get_B16_buffer();
	dithering(R, G, B);
	copy_from_16bit(R, G, B);
    //random_noise();

    time_of_execution->setText( QString::number(time_elapsed) );
    emit sky_completed();
}

void QtSkyDisplay::paint_event(QPainter* painter)
{
    QRect copy_rect(0,0,sky_data.horizontal_pixels,sky_data.vertical_pixels);
    QImage sky_image((uchar*)color_buffer, sky_data.horizontal_pixels,
                     sky_data.vertical_pixels, QImage::Format_RGB32);

    painter->drawImage(copy_rect,sky_image);
}


void QtSkyDisplay::set_params(int pixX, int pixY, float view_angle,
                double albiedoR, double albiedoG, double albiedoB,
                float horizontal_angle, float vertical_angle,
                int turbid)
{
	sky_data.horizontal_angle   =   radians(-horizontal_angle);
    sky_data.vertical_angle     =   radians(vertical_angle);

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

    albiedo[0] = albiedoR;
    albiedo[1] = albiedoG;
    albiedo[2] = albiedoB;

    turbidity = turbid;
}

void QtSkyDisplay::set_sky_intensity(float intensity)
{
	sky_model->set_sky_intensity( intensity );
}

void QtSkyDisplay::set_solar_intensity(float intensity)
{
	sky_model->set_solar_intensity(intensity);
}

void QtSkyDisplay::set_dithering(int level)
{
	//dithering_level = level;
	if( level != 0 )
		dithering_level = 255;		// Przekształcamy obrazek z 16 bit na kanał do RGB
	else
		dithering_level = 0;
}

void QtSkyDisplay::save_image(QString& file_name)
{
	QImage sky_image((uchar*)color_buffer, sky_data.horizontal_pixels,
					 sky_data.vertical_pixels, QImage::Format_RGB32);
	sky_image.save(file_name,0,100);
}

void QtSkyDisplay::set_solar_elevation(float elevation)
{
	float rad_elevation = glm::radians(elevation);
	sun.x = 0.0;
	sun.y = glm::sin(rad_elevation);
	sun.z = -glm::cos(rad_elevation);
}

/**Używa algorytmu Floyda-Steinberga do obrazka.
Aby włączyć dithering trzeba ustawić zmienną dithering_level
na wartość większą niż 1.
Zmienna ta oznacza jaka będzie przerwa między dopuszczalnymi
poziomami składowych RGB.
Czyli poziom 3 oznacza, że pierwsza dopuszczalna wartość
mniejsza od 255 wynosi 252.*/

void QtSkyDisplay::dithering()
{
	if( dithering_level < 2 )	//nie mamy co robić
		return;

	int vert_pix = sky_data.vertical_pixels;
	int hor_pix = sky_data.horizontal_pixels;

	//Będziemy przetwarzać piksele w poziomie. Błędy propagują się
	//najdalej o jeden piksel w związku z czym potrzebujemy tablicy,
	//która pomieści dwa wiersze obrazka
	float* error_table = new float[hor_pix<<1];


	for( int channel = 0; channel < 3; ++ channel )
	{//dla uproszczenia załóżmy 0 - B, 1 - G, 2 - R
		int mask = 0xFF;	//maska do wydobywania składowej
		int shift = 8*channel;	//przesunięcie maski, żeby dostać się do koloru channel

		for( int i=0; i < hor_pix*2; ++i )
			error_table[i] = 0;		//zerujemy pamięć

		for( int vert = 0; vert < vert_pix; ++vert )
		{//przechodzimy obrazek w pionie

			for( int hor = 0; hor < hor_pix; ++hor )
			{//przechodzimy obrazek w poziomie
				int pixel = color_buffer[vert*hor_pix + hor];		//pobieramy piksel
				int pix = pixel & (mask << shift);	//wydobywamy składową
				pix = pix >> shift;					//przesuwamy ją do zakresu 0-255
				float fpix = (float)pix + error_table[hor];	//konwertujemy na float i dodajemy błąd

				float pix_error;
				pix_error = fmod(fpix, (float)dithering_level);	//liczymy błąd
				if( pix_error < (float)dithering_level/(float)2)
					pix = (int)(fpix - pix_error);
				else
					pix = (int)(fpix - pix_error + dithering_level),
					pix_error -= dithering_level;

				//pix = (int)(fpix/dithering_level)*dithering_level;	//ustalamy wartość koloru

				if( pix > 255 )
				{//jeżeli wartość wyszła poza zakres, to obcinamy
				//a to co zostanie propagujemy razem z błędem
					pix_error += pix - 255;
					pix = 255;
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

				pix = pix << shift;		//przesuwamy składową na właściwe miejsce
				pixel = pixel & (~(mask << shift));	//zerujemy miejsce na naszą składową
				pixel = pixel | pix;	//wstawiamy składową na miejsce
				color_buffer[vert*hor_pix + hor] = pixel;	//wstawiamy do pamięci
			}

			//przenosimy roboczy wiersz tabeli do góry, a dolny wiersz zerujemy
			for( int i = 0; i < hor_pix; ++i )
				error_table[i] = error_table[hor_pix + i], error_table[hor_pix + i] = 0;
		}
	}


	delete [] error_table;
}

/**@brief Dithering z użyciem tablic o zwiększonym zakresie kolorów.


*/
void QtSkyDisplay::dithering(unsigned short* R, unsigned short* G, unsigned short* B)
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
				short pixel = color_buff[vert*hor_pix + hor];		//pobieramy piksel
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
void QtSkyDisplay::copy_from_16bit(unsigned short* R, unsigned short* G, unsigned short* B)
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

void QtSkyDisplay::random_noise()
{
    int vert_pix = sky_data.vertical_pixels;
    int hor_pix = sky_data.horizontal_pixels;
    srand((int)time);

    for( int i = 0; i < vert_pix*hor_pix; ++i )
    {
        int color = color_buffer[i];
        short R = (color & 0xFF0000) >> 16;
        short G = (color & 0xFF00) >> 8;
        short B = color & 0xFF;

        //Na razie random jest taki sam dla wszystkich
        int random = rand() % 100;
        if( random < 25 )       random = 1;
        else if( random < 50 )  random = -1;
        else                    random = 0;

        R += random;
        G += random;
        B += random;
        if( R > 255 )   R = 255;
        if( G > 255 )   G = 255;
        if( B > 255 )   B = 255;
        if( R < 0 )     R = 0;
        if( G < 0 )     G = 0;
        if( B < 0 )     B = 0;

        color = (R << 16) | (G << 8) | B;
        color_buffer[i] = color;
    }
}
