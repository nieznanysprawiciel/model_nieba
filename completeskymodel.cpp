#include "completeskymodel.h"
#include "glm/gtx/fast_trigonometry.hpp"
#include "glm/detail/func_trigonometric.hpp"
#include "glm/gtx/vector_angle.hpp"



/*
 * dostępne tryby:
 * VERSION_XYZ  -   kolory w układzie XYZ
 * VERSION_RGB  -   kolory RGB
 * VERSION_SPECTRAL -   wersja w pełni spektralna, w tym przypadku należy podać
 * liczbę kanałów channels
*/
CompleteSkyModel::CompleteSkyModel(int Version, int channels)
{
    if( Version < 4 )
        version = Version;
    else
        version = VERSION_ERROR;

    if( version != VERSION_SPECTRAL )
        num_channels = 3;
    else
        num_channels = channels;

    albedo = new double[num_channels];

    skymodel_state = new ArHosekSkyModelState*[num_channels];
    for( int i=0; i<num_channels; ++i)
            skymodel_state[i] = new ArHosekSkyModelState;

    default_direction = vec3(0,0,-1);
    default_direction = normalize(default_direction);

	sky_intensity = 1.0;
	solar_intensity = 1.0;
}


CompleteSkyModel::~CompleteSkyModel()
{
    clear_HosekWilkie_model();
    delete[] skymodel_state;
    delete[] albedo;
}


/*inicjujemy model
    funkcja inicjuje wszystkie wartości konieczne, do działania modelu
    za pierwszym razem nalezy zawsze wywołać funkcję z pełnym zestawem argumentów
    kolejne wywołania moga modyfikować tylko niektóre parametry
    pozostawiając inne niezmienione

    buffer          -   wskaźnik na bufor o wielkości screen_sizeX*screen_sizeY
    screensizeX     -   ilość pikseli w poziomie
    screensizeY     -   ilość pikseli w pionie
    near_plane      -   odległość od bliższej płaszczyzny obsinania w rzucie perspektywicznym
                        wyrażona w pikselach
                        (dzięki temu jest obliczany kąt widzenia)
    albedo_table    -   zawiera wartości albedo, czyli wpływ odbicia światła od ziemii
        tablica może być alokowana na stosie, ponieważ zostaje przepisana (a nie sam wskaźnik)
        za zwolnienie pamieci odpowiada użytkownik

    turbid          -   turbidity, czyli gęstość atmosfery (odzwierciedla mgłę lub po prostu dużą
                        zawartość pary wodnej

    za zwolnienie bufora odpowiada użytkownik
    nie wolno zwalniać bufora, dopóki używa się funkcji generującej niebo
*/
void CompleteSkyModel::init(int* buffer, int screen_sizeX,
                            int screen_sizeY, int near_plane,
                            double *albedo_table, double turbid)
{
    set_screen( screen_sizeX, screen_sizeY, near_plane );
    set_albedo(albedo_table);
    set_turbidity(turbid);

    color_buffer = buffer;

    init_HosekWilkie_model(sun_elevation);
}

//patrz:    opis funkcji CompleteSkyModel::init
void CompleteSkyModel::init(int* buffer, int screen_sizeX,
                            int screen_sizeY, int near_plane,
                            double *albedo_table, double turbid,
                            vec3 sun)
{
    set_screen( screen_sizeX, screen_sizeY, near_plane );
    set_albedo(albedo_table);
    set_turbidity(turbid);

    color_buffer = buffer;

    sun_direction = normalize(sun);
    calculate_sun_elevation();

    init_HosekWilkie_model(sun_elevation);
}

//patrz:    opis funkcji CompleteSkyModel::init
void CompleteSkyModel::init(int* buffer, int screen_sizeX,
                                         int screen_sizeY, int near_plane,
                                         double *albedo_table, double turbid,
                                         vec3 zenith, vec3 sun)
{
    set_screen( screen_sizeX, screen_sizeY, near_plane );
    set_albedo(albedo_table);
    set_turbidity(turbid);

    color_buffer = buffer;

    sun_direction = normalize(sun);
    zenith_direction = normalize(zenith);
    calculate_sun_elevation();

    init_HosekWilkie_model(sun_elevation);
}

/* ustawia ilość pikseli w pionie i poziomie
 * oraz odległość od bliższej powierzchni obcinania w rzucie perspektywicznym
*/
void CompleteSkyModel::set_screen( int screen_sizeX, int screen_sizeY, int near_plane )
{
    screenX = screen_sizeX;
    screenY = screen_sizeY;
    screen_near_plane = near_plane;
}

/*
 * ustawia albiedo
 * tablica powinna zawierać tyle kanałów ile zostało podanych w konstruktorze
 * obiektu CompleteSkyModel, jeżeli będzie za mało to może wystąpić segmantation fault
 * tablica jest przepisywana, więc nie wymaga alokacji na stercie, za zwolnienie
 * pamięci odpowiada użytkownik wywołujący funkcję
*/
void CompleteSkyModel::set_albedo( double* albedo_table )
{
    for( int i=0; i<num_channels; ++i)
        albedo[i] = albedo_table[i];
}

void CompleteSkyModel::set_turbidity( double new_turbidity )
{
    turbidity = new_turbidity;
}


void CompleteSkyModel::set_sky_intensity(float intensity)
{
	sky_intensity = intensity;
}

void CompleteSkyModel::set_solar_intensity(float intensity)
{
	solar_intensity = intensity;
}

//-----------------------------------------------------------------------//
//                  funkcje rysujące niebo do bufora                     //
//-----------------------------------------------------------------------//

/* funkcje dostają w parametrze kwaternion odpowiadający obrotowi
 * domyślnego ustawienia ekranu do nowej pozyji, czyli wektora
 * [0,0,-1]
 *
 * Aby zmienić parametry typu albiedo, turbidity,
 * wektor do słońca albo do zenitu, należy ponownie wywołać funkcję init
 *
 * wartością zwracaną jest wskaźnik na tablicę intów odpowiadających barwom RGB
 * rozmiar jest określony przez podaną szerokość i wysokość ekranu w pikselach
 *
 * wskaźnik jest tym samym wskaźnikiem, który został podany przy inicjalizacji obiektu
 * w przypadku błędu zwracana jest wartość NULL
 */

int* CompleteSkyModel::execute(quat & screen_rot)
{
    if( version == VERSION_ERROR )
        return NULL;

    screen_rotation = screen_rot;

    //init_HosekWilkie_model(sun_elevation);

    if( version == VERSION_RGB || version == VERSION_XYZ )
        generate_sky_RGB_XYZ();
    else if( version == VERSION_SPECTRAL )
        generate_sky_spectral();

    //clear_HosekWilkie_model();

    return color_buffer;
}





/*
int* CompleteSkyModel::execute(quat & screen_rot, vec3 & sun )
{
    sun_direction = normalize(sun);

    calculate_sun_elevation();

    execute( screen_rot );
    return color_buffer;
}

int* CompleteSkyModel::execute( quat & screen_rot, vec3 & sun, vec3 & zenith )
{
    sun_direction = normalize(sun);
    zenith_direction = normalize(zenith);

    calculate_sun_elevation();

    execute( screen_rot );
    return color_buffer;
}*/

//-----------------------------------------------------------------------//
//                          funkcje pomocnicze                           //
//-----------------------------------------------------------------------//

void CompleteSkyModel::init_HosekWilkie_model(double solarElevation)
{
    if( version == VERSION_RGB )
        for ( int i = 0; i < num_channels; i++ )
            skymodel_state[i] =
                arhosek_rgb_skymodelstate_alloc_init(
                      skymodel_state[i],
                      turbidity,
                      albedo[i],
                      solarElevation
                    );
    else if( version == VERSION_SPECTRAL )
        for ( int i = 0; i < num_channels; i++ )
            skymodel_state[i] =
                arhosekskymodelstate_alloc_init(
					  skymodel_state[i],
                      turbidity,
                      albedo[i],
                      solarElevation
                    );
    else if( version == VERSION_XYZ )
        for ( int i = 0; i < num_channels; i++ )
            skymodel_state[i] =
                arhosek_xyz_skymodelstate_alloc_init(
                      skymodel_state[i],
                      turbidity,
                      albedo[i],
                      solarElevation
                    );
}

void CompleteSkyModel::clear_HosekWilkie_model()
{
    for ( int i = 0; i < num_channels; i++ )
        arhosekskymodelstate_free( skymodel_state[i] );
}

void CompleteSkyModel::generate_sky_RGB_XYZ(/*int offset, int length*/)
{
    vec3    horizontal_step;
    vec3    vertical_step;
    vec3    top_left_corner;
    double  theta;
    double  gamma;

    screen_vectors( horizontal_step, vertical_step, top_left_corner);

    unsigned int max_loop = screenX*screenY;
    for( unsigned int i = 0; i < max_loop;  ++i )
    {
        double R;
        double G;
        double B;

        next_angles( horizontal_step, vertical_step,
                     top_left_corner,
                     theta, gamma );

        R = arhosek_tristim_skymodel_radiance( skymodel_state[0], theta, gamma, 0 );
        G = arhosek_tristim_skymodel_radiance( skymodel_state[1], theta, gamma, 1 );
        B = arhosek_tristim_skymodel_radiance( skymodel_state[2], theta, gamma, 2 );

		R *= sky_intensity;
		G *= sky_intensity;
		B *= sky_intensity;

		R += solar_intensity * solar_radiance_RGB( skymodel_state[0], 0, (glm::half_pi<double>()-theta), gamma);
		G += solar_intensity * solar_radiance_RGB( skymodel_state[1], 1, (glm::half_pi<double>()-theta), gamma);
		B += solar_intensity * solar_radiance_RGB( skymodel_state[2], 2, (glm::half_pi<double>()-theta), gamma);

        color_buffer[i] = make_RGB(R,G,B);
    }

}

void CompleteSkyModel::generate_sky_spectral()
{
    /*vec3    horizontal_step;
    vec3    vertical_step;
    vec3    top_left_corner;
    double  theta;
    double  gamma;*/

}

//potrzebne dla funkcji inicjującej arhosek_rgb_skymodelstate_alloc_init
void CompleteSkyModel::calculate_sun_elevation()
{
    sun_elevation = angle( sun_direction, zenith_direction );
    sun_elevation = radians(90.0) - sun_elevation;
}

/*oblicza przekztałcone wektory:
 * - kroku i jeden piksel horyzontalnie
 * - kroku o jeden piksel pionowo
 * - lewego górnego rogu ekranu
 *
 * wektory są przemnażane przez kwaternion obrotu
 *
 * funkcja inicjuje dane dla późniejszych wywołań next_angles
*/
void CompleteSkyModel::screen_vectors(vec3 & horizontal_step, vec3 & vertical_step,
                    vec3 & top_left_corner)
{
    horizontal_step = vec3(1,0,0);
    vertical_step = vec3(0,-1,0);

    //ustalamy długość wektora ekranu na odległóść od bliższej płaszczyzny odcinania
    top_left_corner = default_direction*(float)screen_near_plane;
    //tworzymy wektordo lewej górnej części ekranu
    top_left_corner = top_left_corner
            - horizontal_step*((float)screenX/2)
            - vertical_step*((float)screenY/2);

    /*test*/
    float x = top_left_corner.x;
    float y = top_left_corner.y;
    float z = top_left_corner.z;

    /*do tej pory operowaliśmy w pozycji domyślnej
    teraz przemnażając przez kwaternion obrotu ekranu ustawimy
    faktyczny wektor lewego górnego rogu oraz kroki po osiach X i Y*/
    horizontal_step = screen_rotation*horizontal_step;
    vertical_step   = screen_rotation*vertical_step;
    top_left_corner = screen_rotation*top_left_corner;

    /*test*/
    x = top_left_corner.x;
    y = top_left_corner.y;
    z = top_left_corner.z;

    //inicjacja zmiennych dla funkcji next_angles
    currentX = 0;
    currentY = 0;
}

/*oblicza kolejne kąty gamma i theta, których wymaga model
 * funkcja przechowuje wewnątrz aktualny stan, trzeba wywołać funkcję
 * screen_vectors, która inicjuje ten stan odpowiednimi danymi
 */
void CompleteSkyModel::next_angles(vec3 & horizontal_step, vec3 & vertical_step,
                 vec3 & top_left_corner, double& theta, double& gamma)
{
    vec3    view_direction;

    view_direction = top_left_corner
            + (float)currentX*horizontal_step
            + (float)currentY*vertical_step;

    /*test*/
    float x = view_direction.x;
    float y = view_direction.y;
    float z = view_direction.z;

    view_direction = normalize(view_direction);

    /*test*/
    x = view_direction.x;
    y = view_direction.y;
    z = view_direction.z;

	gamma = angle(sun_direction,view_direction);
	theta = angle(zenith_direction,view_direction);

    ++currentX;
    if(currentX >= screenX)
        currentX = 0, ++currentY;
}

/*konwertuje kolory z trzech liczb typu double na unsigned int
 * jeżeli poszczegolne składowe przekraczają wartość maksymalną
 * to przycina je do 255
*/
unsigned int CompleteSkyModel::make_RGB(double &R, double &G, double &B)
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int pix_color;

	red = (unsigned int)R;
	green = (unsigned int)G;
	blue = (unsigned int)B;

    if(red > 255)
        red = 255;
    if(green > 255)
        green = 255;
    if(blue > 255)
        blue = 255;

    pix_color = (0xFF<<24)+ (red<<16) + (green<<8) + blue;
    return pix_color;
}


//==================================================================//
//  multithreading
//==================================================================//


void CompleteSkyModel::set_rotation(quat & screen_rot)
{
    screen_rotation = screen_rot;
}

//Ta sama funkcja tylko z obsługą wielowątkowości
int* CompleteSkyModel::execute( int offset, int max)
{
    if( version == VERSION_ERROR )
        return NULL;

    if( version == VERSION_RGB || version == VERSION_XYZ )
        generate_sky_RGB_XYZ(offset, max);
    /*else if( version == VERSION_SPECTRAL )
        generate_sky_spectral();    //wersja niedostępna*/

    return color_buffer;
}



void CompleteSkyModel::generate_sky_RGB_XYZ(unsigned int offset, unsigned int max)
{
    vec3    horizontal_step;
    vec3    vertical_step;
    vec3    top_left_corner;
    double  theta;
    double  gamma;
    int     curX;
    int     curY;

    //obliczamy pozycję początkową dla naszego wątku
    curY = offset / screenX;        //dzielenie całkowite
    curX = offset - curY*screenX;

    screen_vectors( horizontal_step, vertical_step, top_left_corner );

    unsigned int max_loop = max;
    for( unsigned int i = offset; i < max_loop;  ++i )
    {
        double R;
        double G;
        double B;

        next_angles( horizontal_step, vertical_step,
                     top_left_corner,
                     theta, gamma, curX, curY );

        R = arhosek_tristim_skymodel_radiance( skymodel_state[0], theta, gamma, 0 );
        G = arhosek_tristim_skymodel_radiance( skymodel_state[1], theta, gamma, 1 );
        B = arhosek_tristim_skymodel_radiance( skymodel_state[2], theta, gamma, 2 );

        R *= sky_intensity;
        G *= sky_intensity;
        B *= sky_intensity;

        R += solar_intensity * solar_radiance_RGB( skymodel_state[0], 0, (glm::half_pi<double>()-theta), gamma);
        G += solar_intensity * solar_radiance_RGB( skymodel_state[1], 1, (glm::half_pi<double>()-theta), gamma);
        B += solar_intensity * solar_radiance_RGB( skymodel_state[2], 2, (glm::half_pi<double>()-theta), gamma);

        color_buffer[i] = make_RGB(R,G,B);
    }
}


/*oblicza kolejne kąty gamma i theta, których wymaga model
 * funkcja przechowuje wewnątrz aktualny stan, trzeba wywołać funkcję
 * screen_vectors, która inicjuje ten stan odpowiednimi danymi
 */
void CompleteSkyModel::next_angles(vec3 & horizontal_step, vec3 & vertical_step,
                 vec3 & top_left_corner, double& theta, double& gamma,
                 int & currentX, int & currentY)
{
    vec3    view_direction;

    view_direction = top_left_corner
            + (float)currentX*horizontal_step
            + (float)currentY*vertical_step;

    view_direction = normalize(view_direction);

    gamma = angle(sun_direction,view_direction);
    theta = angle(zenith_direction,view_direction);

    ++currentX;
    if(currentX >= screenX)
        currentX = 0, ++currentY;
}

