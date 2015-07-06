#include "CIEXYZ_distribution.h"
#include "SpectralConversion.h"



SpectralConversion::SpectralConversion()
{
	buildConversionXYZ();
	buildConversionRGB();
}

void SpectralConversion::buildConversionXYZ()
{
	//wype�niamy tablic� przelicznik�w
	for( int i = 0; i < COEFFICIENTS; ++i )
	{//chodzimy po d�ugo�ciach fali
		//najpierw przeskakujemy nieu�ywany obszar (8)
		//przeskakujemy do odpowiedniej d�ugo�ci fali (i*4)
		//wybieramy odpowiedni� kolumn�
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


/*

//  ========================================================================    //
void convert_spectral_to_XYZ()
{
	const int wave_lengths = 9;
    //budujemy tablic� przelicznik�w miedzy d�ugo�ciami fali, a systemem XYZ
    double CMF[wave_lengths][3];

    //wype�niamy tablic� przelicznik�w
	for( int i = 0; i < wave_lengths; ++i )
    {//chodzimy po d�ugo�ciach fali
        //najpierw przeskakujemy nieu�ywany obszar (8)
        //przeskakujemy do odpowiedniej d�ugo�ci fali (i*4*8)
        //wybieramy odpowiedni� kolumn�
        double l_lambda = XYZ_cone_fundamentals[8 + i*4*8 + 1];
        double m_lambda = XYZ_cone_fundamentals[8 + i*4*8 + 2];
        double s_lambda = XYZ_cone_fundamentals[8 + i*4*8 + 3];

        for(int j = 0; j < 3; ++j)
        {//chodzimy po cmf dla X,Y i Z
            CMF[i][j] = XYZtrans_params[j][0] * l_lambda +
                        XYZtrans_params[j][1] * m_lambda +
                        XYZtrans_params[j][2] * s_lambda;
        }
    }

    //budujemy w�a�iw� tablic�
    for( int turb = 0; turb < turbidities; ++turb )
    {//iteruejmy po warto�ciach turbidity
        int turbidity_offset = order * pieces * turb;

        for( int elev = 0; elev < pieces; ++elev )
        {//iterujemy po wysoko�iach s�onca
            int elevation_offset = turbidity_offset + order*(elev+1) - 1;
            for( int coeff = 0; coeff < order; ++coeff )
            {//iterujemy po wsp�czynnikach krzywej beziera
                for( int chan = 0; chan < 3; ++chan )
                {//iterujemy po kana�ach X, Y i Z
                    *(solarDatasetsXYZ[chan] + elevation_offset - coeff) = 0;
					for( int wave = 0; wave < wave_lengths; ++wave )
                    //iterujemy po ca�ym spektrum i dodajemy kolejne sk�adniki sumy
                        *(solarDatasetsXYZ[chan] + elevation_offset - coeff) +=
                            *(solarDatasets[wave] + elevation_offset - coeff) * CMF[wave][chan];
                }
            }
        }
    }
}

//  ========================================================================    //
void convert_XYZ_to_RGB()
{
    //musimy wszystkie wsp�czynniki przemno�yc przez macierz RGBtrans_params
    for( int turb = 0; turb < turbidities; ++turb )
    {//iteruejmy po warto�ciach turbidity
        int turbidity_offset = order * pieces * turb;

        for( int elev = 0; elev < pieces; ++elev )
        {//iterujemy po wysoko�iach s�onca
            int elevation_offset = turbidity_offset + order*(elev+1) - 1;
            for( int coeff = 0; coeff < order; ++coeff )
            {//iterujemy po wsp�czynnikach krzywej beziera
                for( int chan = 0; chan < 3; ++chan )
                {//iterujemy po kana�ach X, Y i Z
                    *(solarDatasetsRGB[chan] + elevation_offset - coeff) = 0;
                    for( int XYZchan = 0; XYZchan < 3; ++XYZchan )
                    //iterujemy po ca�ym spektrum i dodajemy kolejne sk�adniki sumy
                        *(solarDatasetsRGB[chan] + elevation_offset - coeff) +=
                            *(solarDatasetsXYZ[XYZchan] + elevation_offset - coeff) * RGBtrans_params[chan][XYZchan];
                }
            }
        }
    }
}

//  ========================================================================    //
void convert_limbDarkening()
{
    const int wave_lengths = 9;
    //budujemy tablic� przelicznik�w miedzy d�ugo�ciami fali, a systemem XYZ
    double CMF[wave_lengths][3];

    //wype�niamy tablic� przelicznik�w
    for( int i = 0; i < wave_lengths; ++i )
    {//chodzimy po d�ugo�ciach fali
        //najpierw przeskakujemy nieu�ywany obszar (8)
        //przeskakujemy do odpowiedniej d�ugo�ci fali (i*4*8)
        //wybieramy odpowiedni� kolumn�
        double l_lambda = XYZ_cone_fundamentals[8 + i*4*8 + 1];
        double m_lambda = XYZ_cone_fundamentals[8 + i*4*8 + 2];
        double s_lambda = XYZ_cone_fundamentals[8 + i*4*8 + 3];

        for(int j = 0; j < 3; ++j)
        {//chodzimy po cmf dla X,Y i Z
            CMF[i][j] = XYZtrans_params[j][0] * l_lambda +
                        XYZtrans_params[j][1] * m_lambda +
                        XYZtrans_params[j][2] * s_lambda;
        }
    }



    for( int coeff = 0; coeff < limb_darkenings; ++coeff )
    {//iterujemy po wsp�czynnikach krzywej beziera
        for( int chan = 0; chan < 3; ++chan )
        {//iterujemy po kana�ach X, Y i Z
            *(limbDarkeningDatasetsXYZ[chan] +  coeff) = 0;
            for( int wave = 0; wave < wave_lengths; ++wave )
            //iterujemy po ca�ym spektrum i dodajemy kolejne sk�adniki sumy
                *(limbDarkeningDatasetsXYZ[chan] + coeff) +=
                    *(limbDarkeningDatasets[wave] +  coeff) * CMF[wave][chan];
        }
    }

    //konwersja do RGB
    for( int coeff = 0; coeff < limb_darkenings; ++coeff )
    {//iterujemy po wsp�czynnikach krzywej beziera
        for( int chan = 0; chan < 3; ++chan )
        {//iterujemy po kana�ach X, Y i Z
            *(limbDarkeningDatasetsRGB[chan] + coeff) = 0;
            for( int XYZchan = 0; XYZchan < 3; ++XYZchan )
            //iterujemy po ca�ym spektrum i dodajemy kolejne sk�adniki sumy
                *(limbDarkeningDatasetsRGB[chan] + coeff) +=
                    *(limbDarkeningDatasetsXYZ[XYZchan] + coeff) * RGBtrans_params[chan][XYZchan];
        }
    }
}

*/
