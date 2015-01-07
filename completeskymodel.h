#ifndef COMPLETESKYMODEL_H
#define COMPLETESKYMODEL_H

//żeby było mniej komunikatów
#define GLM_FORCE_RADIANS

#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"
#include "HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModel.h"
#include "HosekWilkie_SkylightModel_C_Source.1.4a/ArHosekSkyModelSolarRadiance_RGB.h"


#define     VERSION_RGB         0x0
#define     VERSION_XYZ         0x1
#define     VERSION_SPECTRAL    0x2
#define     VERSION_ERROR       0xFFFFFFFF

using namespace glm;

class CompleteSkyModel
{
private:
	int*    color_buffer;	//bufor, w którym znajdzie się wynik

    int     screenX;
    int     screenY;
    int     screen_near_plane;

	unsigned int     version;	//jeden z trybów XYZ, RGB lub spectral

	int     num_channels;	//liczba kanałów
	double* albedo;			//tablica zawierająca albedo dla poszczególnych kanałów
	double  turbidity;		//nieprzezroczystość amtosfery
	float	sky_intensity;
	float	solar_intensity;
    double  sun_elevation;

	vec3    default_direction;	//domyślny wektor ekranu
	quat    screen_rotation;	//aktualny kwaternion obrotu ekranu
	vec3    sun_direction;		//wektor w kierunku słońca
	vec3    zenith_direction;	//wektor w kierunku zenitu

    ArHosekSkyModelState  ** skymodel_state;

	int     currentX;		//zmienne dla funkcji next_angles
	int     currentY;		//aktualnie przetwarzany piksel ekranu

public:
    CompleteSkyModel(int Version, int channels);
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

    void set_screen( int screen_sizeX, int screen_sizeY, int near_plane );
	void set_sky_intensity(float intensity);
	void set_solar_intensity(float intensity);
    void set_rotation(quat & screen_rot);      //do wielowątkowych funkcji

    int* execute( quat & screen_rot );
    int* execute(int offset, int max ); //do wielowątkowego wykonania
    //int* execute( quat & screen_rot, vec3 &sun );
    //int* execute(quat & screen_rot, vec3 &sun, vec3 &zenith );

private:        //funkcje pomocnicze
    void init_HosekWilkie_model(double solarElevation);
    void clear_HosekWilkie_model();
    void generate_sky_RGB_XYZ();
    void generate_sky_RGB_XYZ(unsigned int offset, unsigned int max);
    void generate_sky_spectral();
    void calculate_sun_elevation();

    void screen_vectors(vec3 & horizontal_step, vec3 & vertical_step,
                        vec3 & top_left_corner);
    void next_angles(vec3 & horizontal_step, vec3 & vertical_step,
                     vec3 & top_left_corner, double& theta, double& gamma);
    void next_angles(vec3 & horizontal_step, vec3 & vertical_step,
                     vec3 & top_left_corner, double& theta, double& gamma,
                     int & currentX, int & currentY);
    unsigned int make_RGB(double& R, double& G, double& B);

    void set_albedo( double* albedo_table );
    void set_turbidity( double new_turbidity );
};

#endif // COMPLETESKYMODEL_H
