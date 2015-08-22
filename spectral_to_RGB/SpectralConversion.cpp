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
		case CMF_CIE1931JuddVos:
			return CIE1931JuddVos::XYZ_cone_fundamentals;
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
		case CMF_CIE1931JuddVos:
			return CIE1931JuddVos::XYZtrans_params;
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
		case ColorSpace_AdobeRGB:
			return COLORSPACE_ADOBE_RGB::RGBtrans_params;
		case ColorSpace_AppleRGB:
			return COLORSPACE_APPLE_RGB::RGBtrans_params;
		case ColorSpace_BestRGB:
			return COLORSPACE_BEST_RGB::RGBtrans_params;
		case ColorSpace_BruceRGB:
			return COLORSPACE_BRUCE_RGB::RGBtrans_params;
		case ColorSpace_ColorMatchRGB:
			return COLORSPACE_COLORMATCH_RGB::RGBtrans_params;
		case ColorSpace_DonRGB:
			return COLORSPACE_DON_RGB_4::RGBtrans_params;
		case ColorSpace_ECIRGB:
			return COLORSPACE_ECI_RGB::RGBtrans_params;
		case ColorSpace_EktaSpacePS5:
			return COLORSPACE_EKTA_SPACE_PS5::RGBtrans_params;
		case ColorSpace_NTSCRGB:
			return COLORSPACE_NTSC_RGB::RGBtrans_params;
		case ColorSpace_PalSecamRGB:
			return COLORSPACE_PALSECAM_RGB::RGBtrans_params;
		case ColorSpace_ProPhotoRGB:
			return COLORSPACE_PRO_PHOTO_RGB::RGBtrans_params;
		case ColorSpace_SMPTE_C_RGB:
			return COLORPSPACE_SMPTE_RGB::RGBtrans_params;
		case ColorSpace_WideGamutRGB:
			return COLORSPACE_WIDE_GAMUT_RGB::RGBtrans_params;
		default:
			return COLORSPACE_CIERGB::RGBtrans_params;
	}
}

void SpectralConversion::setDestinationColorSpace( ColorSpaceRGB space )
{
	if( space == m_destColorSpace )
		return;

	m_destColorSpace = space;

	buildConversionRGB();
}

void SpectralConversion::setColorMatchingFunction( SpectralCoefficients cmf )
{
	if( cmf == m_colorMatchingFunction )
		return;

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


