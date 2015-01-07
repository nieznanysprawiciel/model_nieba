#include <iostream>
#include <fstream>
#include <string>
#include "..\..\HosekWilkie_SkylightModel_C_Source.1.4a\ArHosekSkyModelData_Spectral.h"
#include "CIEXYZ_distribution.h"


void convert_spectral_to_XYZ();
void convert_XYZ_to_RGB();
void convert_limbDarkening();
void save_XYZ_to_file(std::string file_name);
void save_RGB_to_file(std::string file_name);
void save_limbDarkening_RGB_to_file(std::string file_name);


using namespace std;

const int pieces = 45;
const int order = 4;
const int turbidities = 10;
const int limb_darkenings = 6;


double limbDarkeningDatasetX[6];
double limbDarkeningDatasetY[6];
double limbDarkeningDatasetZ[6];

double solarDatasetX[pieces*order*turbidities];
double solarDatasetY[pieces*order*turbidities];
double solarDatasetZ[pieces*order*turbidities];

double* solarDatasetsXYZ[] =
{
    solarDatasetX,
	solarDatasetY,
	solarDatasetZ
};

double * limbDarkeningDatasetsXYZ[] =
{
	limbDarkeningDatasetX,
	limbDarkeningDatasetY,
	limbDarkeningDatasetZ
};


double limbDarkeningDatasetR[6];
double limbDarkeningDatasetG[6];
double limbDarkeningDatasetB[6];

double solarDatasetR[pieces*order*turbidities];
double solarDatasetG[pieces*order*turbidities];
double solarDatasetB[pieces*order*turbidities];

double* solarDatasetsRGB[] =
{
    solarDatasetR,
	solarDatasetG,
	solarDatasetB
};

double * limbDarkeningDatasetsRGB[] =
{
	limbDarkeningDatasetR,
	limbDarkeningDatasetG,
	limbDarkeningDatasetB
};



int main()
{
    cout << "Program converts solar radiance coefficients to RGB version" << endl;

    cout.setf(ios::scientific);

    convert_spectral_to_XYZ();
    convert_XYZ_to_RGB();
    convert_limbDarkening();

    save_XYZ_to_file("solarRadianceXYZ.h");
    save_RGB_to_file("solarRadianceRGB.h");
    save_limbDarkening_RGB_to_file("limbDarkeningRGB.h");

    return 0;
}

//  ========================================================================    //
void convert_spectral_to_XYZ()
{
    const int wave_lengths = 9;
    //budujemy tablicê przeliczników miedzy d³ugoœciami fali, a systemem XYZ
    double CMF[wave_lengths][3];

    //wype³niamy tablicê przeliczników
    for( int i = 0; i < wave_lengths; ++i )
    {//chodzimy po d³ugoœciach fali
        //najpierw przeskakujemy nieu¿ywany obszar (8)
        //przeskakujemy do odpowiedniej d³ugoœci fali (i*4*8)
        //wybieramy odpowiedni¹ kolumnê
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

    //budujemy w³aœiw¹ tablicê
    for( int turb = 0; turb < turbidities; ++turb )
    {//iteruejmy po wartoœciach turbidity
        int turbidity_offset = order * pieces * turb;

        for( int elev = 0; elev < pieces; ++elev )
        {//iterujemy po wysokoœiach s³onca
            int elevation_offset = turbidity_offset + order*(elev+1) - 1;
            for( int coeff = 0; coeff < order; ++coeff )
            {//iterujemy po wspó³czynnikach krzywej beziera
                for( int chan = 0; chan < 3; ++chan )
                {//iterujemy po kana³ach X, Y i Z
                    *(solarDatasetsXYZ[chan] + elevation_offset - coeff) = 0;
                    for( int wave = 0; wave < wave_lengths; ++wave )
                    //iterujemy po ca³ym spektrum i dodajemy kolejne sk³adniki sumy
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
    //musimy wszystkie wspó³czynniki przemno¿yc przez macierz RGBtrans_params
    for( int turb = 0; turb < turbidities; ++turb )
    {//iteruejmy po wartoœciach turbidity
        int turbidity_offset = order * pieces * turb;

        for( int elev = 0; elev < pieces; ++elev )
        {//iterujemy po wysokoœiach s³onca
            int elevation_offset = turbidity_offset + order*(elev+1) - 1;
            for( int coeff = 0; coeff < order; ++coeff )
            {//iterujemy po wspó³czynnikach krzywej beziera
                for( int chan = 0; chan < 3; ++chan )
                {//iterujemy po kana³ach X, Y i Z
                    *(solarDatasetsRGB[chan] + elevation_offset - coeff) = 0;
                    for( int XYZchan = 0; XYZchan < 3; ++XYZchan )
                    //iterujemy po ca³ym spektrum i dodajemy kolejne sk³adniki sumy
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
    //budujemy tablicê przeliczników miedzy d³ugoœciami fali, a systemem XYZ
    double CMF[wave_lengths][3];

    //wype³niamy tablicê przeliczników
    for( int i = 0; i < wave_lengths; ++i )
    {//chodzimy po d³ugoœciach fali
        //najpierw przeskakujemy nieu¿ywany obszar (8)
        //przeskakujemy do odpowiedniej d³ugoœci fali (i*4*8)
        //wybieramy odpowiedni¹ kolumnê
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
    {//iterujemy po wspó³czynnikach krzywej beziera
        for( int chan = 0; chan < 3; ++chan )
        {//iterujemy po kana³ach X, Y i Z
            *(limbDarkeningDatasetsXYZ[chan] +  coeff) = 0;
            for( int wave = 0; wave < wave_lengths; ++wave )
            //iterujemy po ca³ym spektrum i dodajemy kolejne sk³adniki sumy
                *(limbDarkeningDatasetsXYZ[chan] + coeff) +=
                    *(limbDarkeningDatasets[wave] +  coeff) * CMF[wave][chan];
        }
    }

    //konwersja do RGB
    for( int coeff = 0; coeff < limb_darkenings; ++coeff )
    {//iterujemy po wspó³czynnikach krzywej beziera
        for( int chan = 0; chan < 3; ++chan )
        {//iterujemy po kana³ach X, Y i Z
            *(limbDarkeningDatasetsRGB[chan] + coeff) = 0;
            for( int XYZchan = 0; XYZchan < 3; ++XYZchan )
            //iterujemy po ca³ym spektrum i dodajemy kolejne sk³adniki sumy
                *(limbDarkeningDatasetsRGB[chan] + coeff) +=
                    *(limbDarkeningDatasetsXYZ[XYZchan] + coeff) * RGBtrans_params[chan][XYZchan];
        }
    }
}

//  ========================================================================    //
void save_XYZ_to_file(std::string file_name)
{
    fstream File;
    File.open(file_name.c_str(),std::fstream::out);
    if( !File.good() )
    {
        cout << "Nie uda³o siê otworzyæ pliku: " << file_name << endl;
        return;
    }

    int i = 0;

    File <<"double solarDatasetX[] ="<<endl << "{" << endl;

    for( i = 0; i < pieces*order*turbidities - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File << "       " << (double)solarDatasetX[i] << "," << endl;
    }

    File << "       " << (double)solarDatasetX[i] << endl << "};" << endl<<endl<<endl;


    File <<"double solarDatasetY[] ="<<endl << "{" << endl;

    for( i = 0; i < pieces*order*turbidities - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File << "       " << (double)solarDatasetY[i] << "," << endl;
    }

    File << "       " << (double)solarDatasetY[i] << endl << "};" << endl<<endl<<endl;



    File <<"double solarDatasetZ[] ="<<endl << "{" << endl;

    for( i = 0; i < pieces*order*turbidities - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File << "       " << (double)solarDatasetZ[i] << "," << endl;
    }

    File << "       " << (double)solarDatasetZ[i] << endl << "};" << endl<<endl<<endl;

    File.close();
}

//  ========================================================================    //
void save_RGB_to_file(std::string file_name)
{
    fstream File;
    File.open(file_name.c_str(),std::fstream::out);

    if( !File.good() )
    {
        cout << "Nie uda³o siê otworzyæ pliku: " << file_name << endl;
        return;
    }

    int i = 0;

    File <<"double solarDatasetR[] ="<<endl << "{" << endl;

    for( i = 0; i < pieces*order*turbidities - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File << "       " << solarDatasetR[i] << "," << endl;
    }

    File << "       " << solarDatasetR[i] << endl << "};" << endl<<endl<<endl;


    File <<"double solarDatasetG[] ="<<endl << "{" << endl;

    for( i = 0; i < pieces*order*turbidities - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File << "       " << solarDatasetG[i] << "," << endl;
    }

    File << "       " << solarDatasetG[i] << endl << "};" << endl<<endl<<endl;



    File <<"double solarDatasetB[] ="<<endl << "{" << endl;

    for( i = 0; i < pieces*order*turbidities - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File << "       " << solarDatasetB[i] << "," << endl;
    }

    File << "       " << solarDatasetB[i] << endl << "};" << endl<<endl<<endl;

    File.close();
}

void save_limbDarkening_RGB_to_file(std::string file_name)
{
    fstream File;
    File.open(file_name.c_str(),std::fstream::out);
    int i = 0;


    File <<"double limbDarkeningDatasetR[] ="<<endl << "{   ";

    for( i = 0; i < limb_darkenings - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File <<limbDarkeningDatasetR[i] << ",   ";
    }
    File <<limbDarkeningDatasetR[i] << "};" << endl<<endl;


    File <<"double limbDarkeningDatasetG[] ="<<endl << "{   ";

    for( i = 0; i < limb_darkenings - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File <<limbDarkeningDatasetG[i] << ",   ";
    }
    File <<limbDarkeningDatasetG[i] << "};" << endl<<endl;


    File <<"double limbDarkeningDatasetB[] ="<<endl << "{   ";

    for( i = 0; i < limb_darkenings - 1; ++i )
    {//ostatni nie ma przecinka, musi byæ przepisany osobno
        File <<limbDarkeningDatasetB[i] << ",   ";
    }
    File <<limbDarkeningDatasetB[i]<< "};" << endl<<endl;


    File.close();
}
