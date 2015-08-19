#ifndef SPECTRAL_CONVERSION_H
#define SPECTRAL_CONVERSION_H


#define WAVE_LENGTHS	9
#define COEFFICIENTS	((WAVE_LENGTHS - 1)*8 + 1)

#define RED_CHANNEL				0
#define GREAN_CHANNEL			1
#define BLUE_CHANNEL			2

#define X				0
#define Y				1
#define Z				2


typedef double (*SpaceTransform)[3];

typedef enum ColorSpaceRGB
{
	ColorSpace_sRGB,
	ColorSpace_CIERGB
} ColorSpaceRGB;

typedef enum
{
	CMF_CIE1931			= 0,
	CMF_CIE2006			= 1
} SpectralCoefficients;

/**@brief Konwertuje kolor z wersji spektralnej do RGB lub XYZ.

Na razie działa tylko dla wybranych długości fali.
Stała WAVE_LENGTHS nie powinna być zmieniana, no chyba że ktoś
zaimpementuje ogólniejsze algorytmy.*/
class SpectralConversion
{
private:
	double					m_transformMatrix[COEFFICIENTS][3];
	double					m_CMF[COEFFICIENTS][3];

	ColorSpaceRGB			m_destColorSpace;
	SpectralCoefficients	m_colorMatchingFunction;

protected:
	void				buildConversionRGB();
	void				buildConversionXYZ();
public:
	SpectralConversion();

	void				setDestinationColorSpace( ColorSpaceRGB space );
	void				setColorMatchingFunction( SpectralCoefficients cmf );

	double*				getConeFundamentals( SpectralCoefficients spectralCoefficientsEnum );
	SpaceTransform		getXYZTransformMatrix( SpectralCoefficients spectralCoefficientsEnum );
	SpaceTransform		getRGBTransformMatrix( ColorSpaceRGB colorSpaceEnum );

	template<unsigned int channel>
	double convertRGB( double* wavesTable );

	template<unsigned int channel>
	double convertXYZ( double* wavesTable ){wavesTable;/*Implement*/ return 0.0;}
};


template<unsigned int channel>
double SpectralConversion::convertRGB( double* wavesTable )
{
	double result = 0.0;

	double extendedWaveTable[COEFFICIENTS];
	int extWave = 0;
	for( int k = 0; k < WAVE_LENGTHS - 1; ++k )
	{
		for( int j = 0; j < 8; ++j )
		{
			double weight1 = j/8.0;		//j*5/40
			double weight2 = 1 - weight1;
			extendedWaveTable[ extWave ] = weight2 * wavesTable[ k ] + weight1 * wavesTable[ k + 1 ];
			extWave++;
		}
	}
	extendedWaveTable[ extWave ] = wavesTable[ WAVE_LENGTHS - 1 ];

	for( int i = 0; i < COEFFICIENTS; ++i )
		result += m_transformMatrix[ i ][ channel ] * extendedWaveTable[ i ];

	return result;
}


#endif // SPECTRAL_CONVERSION_H
