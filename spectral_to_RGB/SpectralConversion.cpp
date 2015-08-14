#include "CIEXYZ_distribution.h"
#include "SpectralConversion.h"



SpectralConversion::SpectralConversion()
{
	buildConversionXYZ();
	buildConversionRGB();
}

void SpectralConversion::buildConversionXYZ()
{
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


