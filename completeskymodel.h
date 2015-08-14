#ifndef COMPLETESKYMODEL_H
#define COMPLETESKYMODEL_H

//żeby było mniej komunikatów
#define GLM_FORCE_RADIANS

#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"
#include "HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModel.h"
#include "HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModelSolarRadiance_RGB.h"
#include "spectral_to_RGB/SpectralConversion.h"


#define     VERSION_RGB         0x0
#define     VERSION_XYZ         0x1
#define     VERSION_SPECTRAL    0x2
#define     VERSION_ERROR       0xFFFFFFFF

using namespace glm;

const double SPECTRAL_SCALE = 10.0;

class CompleteSkyModel
{
private:
	int*			color_buffer;	///<Bufor, w którym znajdzie się wynik
    unsigned short*  R16_buffer;     ///<Bufor dla koloru czerwonego w formacie float
    unsigned short*  G16_buffer;     ///<Bufor dla koloru zielonego w formacie float
    unsigned short*  B16_buffer;     ///<Bufor dla koloru niebieskiego w formacie float

    int     screenX;
    int     screenY;
    int     screen_near_plane;

	unsigned int     version;	///<jeden z trybów XYZ, RGB lub spectral

	int     num_channels;	///<liczba kanałów
	double* channels_wave;	///<Długości fali dla poszczególnych kanałów.
	double* albedo;			///<tablica zawierająca albedo dla poszczególnych kanałów
	double  turbidity;		///<nieprzezroczystość amtosfery
	float	sky_intensity;
	float	solar_intensity;
    double  sun_elevation;

	vec3    default_direction;	///<domyślny wektor ekranu
	quat    screen_rotation;	///<aktualny kwaternion obrotu ekranu
	vec3    sun_direction;		///<wektor w kierunku słońca
	vec3    zenith_direction;	///<wektor w kierunku zenitu

	ArHosekSkyModelState**		skymodel_state;
	SpectralConversion			spectralConversion;

	int     currentX;		///<zmienne dla funkcji next_angles (wersja jednowątkowa)
	int     currentY;		///<aktualnie przetwarzany piksel ekranu (wersja jednowątkowa)

	bool	perspective_correction;		///< W zależności od tej zmiennej używana jest wersja z korekcją perspektywy lub nie
	float	gamma_correction;			///< Korekcja gamma dla całego obrazka.

public:
	CompleteSkyModel();
    ~CompleteSkyModel();

    void init(int* buffer, int screen_sizeX,
              int screen_sizeY, int near_plane,
              double* albedo_table, double turbid);
    void init(int* buffer, int screen_sizeX,
             int screen_sizeY, int near_plane,
             double* albedo_table, double turbid,
             vec3 sun);
    void init(int* buffer, int screen_sizeX,
             int screen_sizeY, int near_plane,
             double* albedo_table, double turbid,
             vec3 zenith, vec3 sun);

	void set_model_version( unsigned int model_version ) { version = model_version; }
    void set_screen( int screen_sizeX, int screen_sizeY, int near_plane );
	void set_sky_intensity( float intensity );
	void set_solar_intensity( float intensity );
	void set_rotation( quat & screen_rot );      //do wielowątkowych funkcji
	inline void set_gamma_correction( float value ) { gamma_correction = value; }
	inline void use_perspective_correction( bool set ) { perspective_correction = set; }

    int* execute( quat & screen_rot );
    int* execute(int offset, int max ); //do wielowątkowego wykonania

    inline unsigned short* get_R16_buffer() { return R16_buffer; }
    inline unsigned short* get_G16_buffer() { return G16_buffer; }
    inline unsigned short* get_B16_buffer() { return B16_buffer; }

private:        //funkcje pomocnicze
    void init_HosekWilkie_model(double solarElevation);
    void clear_HosekWilkie_model();
    void generate_sky_RGB_XYZ();
	void generate_sky_RGB_XYZ( unsigned int offset, unsigned int max );
	void generate_sky_spectral( unsigned int offset, unsigned int max );
    void calculate_sun_elevation();

    void screen_vectors(vec3 & horizontal_step, vec3 & vertical_step,
                        vec3 & top_left_corner);
	void screen_angles( glm::vec2& delta_angles, glm::vec2& top_left_angles );
    void next_angles(vec3 & horizontal_step, vec3 & vertical_step,
                     vec3 & top_left_corner, double& theta, double& gamma);
    void next_angles(vec3 & horizontal_step, vec3 & vertical_step,
                     vec3 & top_left_corner, double& theta, double& gamma,
                     int & currentX, int & currentY);
	void next_angles(	glm::vec2 angle_step, glm::vec2 top_left_corner,
						double& theta, double& gamma,
						int & currentX, int & currentY );
	double find_sun_correction_factor( double elevation );
	unsigned int make_RGB( double& R, double& G, double& B );
	unsigned short make_16bit( double& color );

    void set_albedo( double* albedo_table );
    void set_turbidity( double new_turbidity );
};

#endif // COMPLETESKYMODEL_H
