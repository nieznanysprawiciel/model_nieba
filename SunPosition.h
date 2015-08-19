#pragma once

//#include <DirectXMath.h>
#include <ctime>

/**@brief Klasa Wylicza wektor w kierunku s�o�ca dla obserwatora stoj�cego na ziemi.*/
class SunPosition
{
private:
	float		m_latitude;					///< Szeroko�� geograficzna na jakiej znajduje si� obserwator.
	float		m_longitude;					///< D�ugo�� geograficzna obserwatora.

	double		m_solarDeclination;
	double		m_daytime;
public:
	SunPosition();
	~SunPosition();

	double computeAzimuth();
	double computeElevation();

	double setSunConditions( float latit, float longit, tm* time );
	void setSunConditions(double time );
};

