#include "CIEXYZ_distribution.h"
#include "SpectralConversion.h"



double* SpectralConversion::getConeFundamentals( SpectralCoefficients spectralCoefficientsEnum )
{
	switch( spectralCoefficientsEnum )
	{
		case CMF_CIE1931:
			return CIE1931::XYZ_cone_fundamentals;
		case CMF_CIE2006:
			return CIE2006::XYZ_cone_fundamentals;
		default:
			return CIE1931::XYZ_cone_fundamentals;
	}
}

SpaceTransform SpectralConversion::getXYZTransformMatrix( SpectralCoefficients spectralCoefficientsEnum )
{
	switch( spectralCoefficientsEnum )
	{
		case CMF_CIE1931:
			return CIE1931::XYZtrans_params;
		case CMF_CIE2006:
			return CIE2006::XYZtrans_params;
		default:
			return CIE1931::XYZtrans_params;
	}
}

SpaceTransform SpectralConversion::getRGBTransformMatrix( ColorSpaceRGB colorSpaceEnum )
{
	switch( colorSpaceEnum )
	{
		case ColorSpace_sRGB:
			return COLORSPACE_sRGB::RGBtrans_params;
		case ColorSpace_CIERGB:
			return COLORSPACE_CIERGB::RGBtrans_params;
		default:
			return COLORSPACE_CIERGB::RGBtrans_params;
	}
}

void SpectralConversion::setDestinationColorSpace( ColorSpaceRGB space )
{
	m_destColorSpace = space;

	buildConversionRGB();
}

void SpectralConversion::setColorMatchingFunction( SpectralCoefficients cmf )
{
	m_colorMatchingFunction = cmf;

	buildConversionXYZ();
	buildConversionRGB();
}

//===========================================================//
//						constructor							//
SpectralConversion::SpectralConversion()
{
	buildConversionXYZ();
	buildConversionRGB();
}

void SpectralConversion::buildConversionXYZ()
{
	double* XYZ_cone_fundamentals = getConeFundamentals( m_colorMatchingFunction );
	SpaceTransform XYZtrans_params = getXYZTransformMatrix( m_colorMatchingFunction );

	//wype³niamy tablicê przeliczników
	for( int i = 0; i < COEFFICIENTS; ++i )
	{//chodzimy po d³ugoœciach fali
		//najpierw przeskakujemy nieu¿ywany obszar (8)
		//przeskakujemy do odpowiedniej d³ugoœci fali (i*4)
		//wybieramy odpowiedni¹ kolumnê
		double l_lambda = XYZ_cone_fundamentals[8 + i*4 + 1];
		double m_lambda = XYZ_cone_fundamentals[8 + i*4 + 2];
		double s_lambda = XYZ_cone_fundamentals[8 + i*4 + 3];

		for(int j = 0; j < 3; ++j)
		{//chodzimy po cmf dla X,Y i Z
			m_CMF[i][j] =	XYZtrans_params[j][0] * l_lambda +
							XYZtrans_params[j][1] * m_lambda +
							XYZtrans_params[j][2] * s_lambda;
		}
	}
}

void SpectralConversion::buildConversionRGB()
{
	SpaceTransform RGBtrans_params = getRGBTransformMatrix( m_destColorSpace );

	for( int i = 0; i < COEFFICIENTS; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			m_transformMatrix[i][j] = 0.0;
			for( int k = 0; k < 3; ++k )
				m_transformMatrix[i][j] += m_CMF[i][k] * RGBtrans_params[j][k];
		}
	}
}


