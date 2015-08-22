

//2-deg LMS fundamentals based on the Stiles and Burch 10-deg CMFs adjusted to 2-deg
//Dane pochodzą ze strony http://cvrl.ioo.ucl.ac.uk/ciepr8dp.htm
namespace CIE2006
{

// double XYZ_cone_fundamentals[] =
// {
// 390.0,4.15003360E-04,3.68349248E-04,9.54728799E-03,
// 395.0,1.05192281E-03,9.58658287E-04,2.38249757E-02,
// 400.0,2.40836274E-03,2.26990665E-03,5.66497751E-02,
// 405.0,4.83338951E-03,4.70010083E-03,1.22450936E-01,
// 410.0,8.72126562E-03,8.79368990E-03,2.33008318E-01,
// 415.0,1.33836930E-02,1.45277327E-02,3.81363350E-01,
// 420.0,1.84480328E-02,2.16648593E-02,5.43618105E-01,
// 425.0,2.29317499E-02,2.95714420E-02,6.74474074E-01,
// 430.0,2.81877052E-02,3.94566147E-02,8.02554955E-01,
// 435.0,3.41053893E-02,5.18198823E-02,9.03572547E-01,
// 440.0,4.02562524E-02,6.47782187E-02,9.91020299E-01,
// 445.0,4.49379920E-02,7.58812175E-02,9.91515250E-01,
// 450.0,4.98639402E-02,8.70523965E-02,9.55393000E-01,
// 455.0,5.53418498E-02,9.81933595E-02,8.60240453E-01,
// 460.0,6.47164360E-02,1.16272079E-01,7.86703893E-01,
// 465.0,8.06893625E-02,1.44540604E-01,7.38267621E-01,
// 470.0,9.94755270E-02,1.75892769E-01,6.46359219E-01,
// 475.0,1.18802152E-01,2.05398073E-01,5.16410649E-01,
// 480.0,1.40145131E-01,2.35754060E-01,3.90333407E-01,
// 485.0,1.63951537E-01,2.68063038E-01,2.90322411E-01,
// 490.0,1.91556224E-01,3.03629653E-01,2.11866834E-01,
// 495.0,2.32925516E-01,3.57060581E-01,1.60525792E-01,
// 500.0,2.88959412E-01,4.27764487E-01,1.22838921E-01,
// 505.0,3.59716421E-01,5.15586863E-01,8.88965361E-02,
// 510.0,4.43682863E-01,6.15520174E-01,6.08209513E-02,
// 515.0,5.36493787E-01,7.19154047E-01,4.28122756E-02,
// 520.0,6.28561087E-01,8.16609632E-01,2.92032829E-02,
// 525.0,7.04720332E-01,8.85549719E-01,1.93911943E-02,
// 530.0,7.70629906E-01,9.35687022E-01,1.26012838E-02,
// 535.0,8.25710771E-01,9.68858338E-01,8.09453368E-03,
// 540.0,8.81010908E-01,9.95216550E-01,5.08900051E-03,
// 545.0,9.19066658E-01,9.97192616E-01,3.16893322E-03,
// 550.0,9.40197682E-01,9.77193335E-01,1.95895533E-03,
// 555.0,9.65733133E-01,9.56583237E-01,1.20277126E-03,
// 560.0,9.81444621E-01,9.17749837E-01,7.40173919E-04,
// 565.0,9.94485919E-01,8.73204873E-01,4.55979308E-04,
// 570.0,9.99993160E-01,8.13509255E-01,2.81799951E-04,
// 575.0,9.92309909E-01,7.40291037E-01,1.75038547E-04,
// 580.0,9.69429172E-01,6.53273547E-01,1.09453897E-04,
// 585.0,9.55601819E-01,5.72596792E-01,6.89990581E-05,
// 590.0,9.27672559E-01,4.92598798E-01,4.39023849E-05,
// 595.0,8.85969334E-01,4.11245617E-01,2.82228417E-05,
// 600.0,8.33982132E-01,3.34428550E-01,1.83458912E-05,
// 605.0,7.75103324E-01,2.64872009E-01,1.20667447E-05,
// 610.0,7.05713103E-01,2.05273346E-01,8.03487566E-06,
// 615.0,6.30773388E-01,1.56242654E-01,5.41843079E-06,
// 620.0,5.54224372E-01,1.16641491E-01,0,
// 625.0,4.79940995E-01,8.55871752E-02,0,
// 630.0,4.00710831E-01,6.21120321E-02,0,
// 635.0,3.27863707E-01,4.44878727E-02,0,
// 640.0,2.65784185E-01,3.14282295E-02,0,
// 645.0,2.13283643E-01,2.18037465E-02,0,
// 650.0,1.65140826E-01,1.54479552E-02,0,
// 655.0,1.24749078E-01,1.07119605E-02,0,
// 660.0,9.30084863E-02,7.30255150E-03,0,
// 665.0,6.85099590E-02,4.97179190E-03,0,
// 670.0,4.98660938E-02,3.43667061E-03,0,
// 675.0,3.58232509E-02,2.37617227E-03,0,
// 680.0,2.53790394E-02,1.63733769E-03,0,
// 685.0,1.77201275E-02,1.12127805E-03,0,
// 690.0,1.21701123E-02,7.61050801E-04,0,
// 695.0,8.47170037E-03,5.25456816E-04,0,
// 700.0,5.89748808E-03,3.65317316E-04,0,
// 705.0,4.09129197E-03,2.53417295E-04,0,
// 710.0,2.80446659E-03,1.74401753E-04,0,
// 715.0,1.92057783E-03,1.20607982E-04,0,
// 720.0,1.32686544E-03,8.41716373E-05,0,
// 725.0,9.17776773E-04,5.89349310E-05,0,
// 730.0,6.39372955E-04,4.16048509E-05,0,
// 735.0,4.46035067E-04,2.94354152E-05,0,
// 740.0,3.10869146E-04,2.08859570E-05,0,
// 745.0,2.19329240E-04,1.50457558E-05,0,
// 750.0,1.54549195E-04,1.08200177E-05,0,
// 755.0,1.09507604E-04,7.82271489E-06,0,
// 760.0,7.79912028E-05,5.69092610E-06,0,
// 765.0,5.56263839E-05,4.13998115E-06,0,
// 770.0,3.99294560E-05,3.02682745E-06,0,
// 775.0,2.86162863E-05,2.21100363E-06,0,
// 780.0,2.07321327E-05,1.63432668E-06,0,
// 785.0,1.50432118E-05,1.21053851E-06,0,
// 790.0,1.09446148E-05,8.99169870E-07,0,
// 795.0,7.97750298E-06,6.69594072E-07,0,
// 800.0,5.85057291E-06,5.03187359E-07,0,
// 805.0,4.31101652E-06,3.80046377E-07,0,
// 810.0,3.17008616E-06,2.86328659E-07,0,
// 815.0,2.34468299E-06,2.16878259E-07,0,
// 820.0,1.74666331E-06,1.65157769E-07,0,
// 825.0,1.30240694E-06,1.25508416E-07,0,
// 830.0,9.74306403E-07,9.53411481E-08,0
// };

}

//2-deg matching functions coefficients
//1931 2° CIE Standard http://www.cis.rit.edu/research/mcsl2/online/cie.php
namespace CIE1931
{

double XYZ_cone_fundamentals[] =
{
390,1.078100e-002,8.000000e-004,4.925000e-002,
395,2.079200e-002,1.545700e-003,9.513500e-002,
400,3.798100e-002,2.800000e-003,1.740900e-001,
405,6.315700e-002,4.656200e-003,2.901300e-001,
410,9.994100e-002,7.400000e-003,4.605300e-001,
415,1.582400e-001,1.177900e-002,7.316600e-001,
420,2.294800e-001,1.750000e-002,1.065800e+000,
425,2.810800e-001,2.267800e-002,1.314600e+000,
430,3.109500e-001,2.730000e-002,1.467200e+000,
435,3.307200e-001,3.258400e-002,1.579600e+000,
440,3.333600e-001,3.790000e-002,1.616600e+000,
445,3.167200e-001,4.239100e-002,1.568200e+000,
450,2.888200e-001,4.680000e-002,1.471700e+000,
455,2.596900e-001,5.212200e-002,1.374000e+000,
460,2.327600e-001,6.000000e-002,1.291700e+000,
465,2.099900e-001,7.294200e-002,1.235600e+000,
470,1.747600e-001,9.098000e-002,1.113800e+000,
475,1.328700e-001,1.128400e-001,9.422000e-001,
480,9.194400e-002,1.390200e-001,7.559600e-001,
485,5.698500e-002,1.698700e-001,5.864000e-001,
490,3.173100e-002,2.080200e-001,4.466900e-001,
495,1.461300e-002,2.580800e-001,3.411600e-001,
500,4.849100e-003,3.230000e-001,2.643700e-001,
505,2.321500e-003,4.054000e-001,2.059400e-001,
510,9.289900e-003,5.030000e-001,1.544500e-001,
515,2.927800e-002,6.081100e-001,1.091800e-001,
520,6.379100e-002,7.100000e-001,7.658500e-002,
525,1.108100e-001,7.951000e-001,5.622700e-002,
530,1.669200e-001,8.620000e-001,4.136600e-002,
535,2.276800e-001,9.150500e-001,2.935300e-002,
540,2.926900e-001,9.540000e-001,2.004200e-002,
545,3.622500e-001,9.800400e-001,1.331200e-002,
550,4.363500e-001,9.949500e-001,8.782300e-003,
555,5.151300e-001,1.000100e+000,5.857300e-003,
560,5.974800e-001,9.950000e-001,4.049300e-003,
565,6.812100e-001,9.787500e-001,2.921700e-003,
570,7.642500e-001,9.520000e-001,2.277100e-003,
575,8.439400e-001,9.155800e-001,1.970600e-003,
580,9.163500e-001,8.700000e-001,1.806600e-003,
585,9.770300e-001,8.162300e-001,1.544900e-003,
590,1.023000e+000,7.570000e-001,1.234800e-003,
595,1.051300e+000,6.948300e-001,1.117700e-003,
600,1.055000e+000,6.310000e-001,9.056400e-004,
605,1.036200e+000,5.665400e-001,6.946700e-004,
610,9.923900e-001,5.030000e-001,4.288500e-004,
615,9.286100e-001,4.417200e-001,3.181700e-004,
620,8.434600e-001,3.810000e-001,2.559800e-004,
625,7.398300e-001,3.205200e-001,1.567900e-004,
630,6.328900e-001,2.650000e-001,9.769400e-005,
635,5.335100e-001,2.170200e-001,6.894400e-005,
640,4.406200e-001,1.750000e-001,5.116500e-005,
645,3.545300e-001,1.381200e-001,3.601600e-005,
650,2.786200e-001,1.070000e-001,2.423800e-005,
655,2.148500e-001,8.165200e-002,1.691500e-005,
660,1.616100e-001,6.100000e-002,1.190600e-005,
665,1.182000e-001,4.432700e-002,8.148900e-006,
670,8.575300e-002,3.200000e-002,5.600600e-006,
675,6.307700e-002,2.345400e-002,3.954400e-006,
680,4.583400e-002,1.700000e-002,2.791200e-006,
685,3.205700e-002,1.187200e-002,1.917600e-006,
690,2.218700e-002,8.210000e-003,1.313500e-006,
695,1.561200e-002,5.772300e-003,9.151900e-007,
700,1.109800e-002,4.102000e-003,6.476700e-007,
705,7.923300e-003,2.929100e-003,4.635200e-007,
710,5.653100e-003,2.091000e-003,3.330400e-007,
715,4.003900e-003,1.482200e-003,2.382300e-007,
720,2.825300e-003,1.047000e-003,1.702600e-007,
725,1.994700e-003,7.401500e-004,1.220700e-007,
730,1.399400e-003,5.200000e-004,8.710700e-008,
735,9.698000e-004,3.609300e-004,6.145500e-008,
740,6.684700e-004,2.492000e-004,4.316200e-008,
745,4.614100e-004,1.723100e-004,3.037900e-008,
750,3.207300e-004,1.200000e-004,2.155400e-008,
755,2.257300e-004,8.462000e-005,1.549300e-008,
760,1.597300e-004,6.000000e-005,1.120400e-008,
765,1.127500e-004,4.244600e-005,8.087300e-009,
770,7.951300e-005,3.000000e-005,5.834000e-009,
775,5.608700e-005,2.121000e-005,4.211000e-009,
780,3.954100e-005,1.498900e-005,3.038300e-009,
785,2.785200e-005,1.058400e-005,2.190700e-009,
790,1.959700e-005,7.465600e-006,1.577800e-009,
795,1.377000e-005,5.259200e-006,1.134800e-009,
800,9.670000e-006,3.702800e-006,8.156500e-010,
805,6.791800e-006,2.607600e-006,5.862600e-010,
810,4.770600e-006,1.836500e-006,4.213800e-010,
815,3.355000e-006,1.295000e-006,3.031900e-010,
820,2.353400e-006,9.109200e-007,2.175300e-010,
825,1.637700e-006,6.356400e-007,1.547600e-010,

};

}

// http://cvrl.ioo.ucl.ac.uk/cmfs.htm
// 2-deg XYZ CMFs transformed from the CIE (2006) 2-deg LMS cone fundamentals
namespace CIE2006
{

double XYZ_cone_fundamentals[] =
{
390,3.769647E-03,4.146161E-04,1.847260E-02,
395,9.382967E-03,1.059646E-03,4.609784E-02,
400,2.214302E-02,2.452194E-03,1.096090E-01,
405,4.742986E-02,4.971717E-03,2.369246E-01,
410,8.953803E-02,9.079860E-03,4.508369E-01,
415,1.446214E-01,1.429377E-02,7.378822E-01,
420,2.035729E-01,2.027369E-02,1.051821E+00,
425,2.488523E-01,2.612106E-02,1.305008E+00,
430,2.918246E-01,3.319038E-02,1.552826E+00,
435,3.227087E-01,4.157940E-02,1.748280E+00,
440,3.482554E-01,5.033657E-02,1.917479E+00,
445,3.418483E-01,5.743393E-02,1.918437E+00,
450,3.224637E-01,6.472352E-02,1.848545E+00,
455,2.826646E-01,7.238339E-02,1.664439E+00,
460,2.485254E-01,8.514816E-02,1.522157E+00,
465,2.219781E-01,1.060145E-01,1.428440E+00,
470,1.806905E-01,1.298957E-01,1.250610E+00,
475,1.291920E-01,1.535066E-01,9.991789E-01,
480,8.182895E-02,1.788048E-01,7.552379E-01,
485,4.600865E-02,2.064828E-01,5.617313E-01,
490,2.083981E-02,2.379160E-01,4.099313E-01,
495,7.097731E-03,2.850680E-01,3.105939E-01,
500,2.461588E-03,3.483536E-01,2.376753E-01,
505,3.649178E-03,4.277595E-01,1.720018E-01,
510,1.556989E-02,5.204972E-01,1.176796E-01,
515,4.315171E-02,6.206256E-01,8.283548E-02,
520,7.962917E-02,7.180890E-01,5.650407E-02,
525,1.268468E-01,7.946448E-01,3.751912E-02,
530,1.818026E-01,8.575799E-01,2.438164E-02,
535,2.405015E-01,9.071347E-01,1.566174E-02,
540,3.098117E-01,9.544675E-01,9.846470E-03,
545,3.804244E-01,9.814106E-01,6.131421E-03,
550,4.494206E-01,9.890228E-01,3.790291E-03,
555,5.280233E-01,9.994608E-01,2.327186E-03,
560,6.133784E-01,9.967737E-01,1.432128E-03,
565,7.016774E-01,9.902549E-01,8.822531E-04,
570,7.967750E-01,9.732611E-01,5.452416E-04,
575,8.853376E-01,9.424569E-01,3.386739E-04,
580,9.638388E-01,8.963613E-01,2.117772E-04,
585,1.051011E+00,8.587203E-01,1.335031E-04,
590,1.109767E+00,8.115868E-01,8.494468E-05,
595,1.143620E+00,7.544785E-01,5.460706E-05,
600,1.151033E+00,6.918553E-01,3.549661E-05,
605,1.134757E+00,6.270066E-01,2.334738E-05,
610,1.083928E+00,5.583746E-01,1.554631E-05,
615,1.007344E+00,4.895950E-01,1.048387E-05,
620,9.142877E-01,4.229897E-01,0.000000E+00,
625,8.135565E-01,3.609245E-01,0.000000E+00,
630,6.924717E-01,2.980865E-01,0.000000E+00,
635,5.755410E-01,2.416902E-01,0.000000E+00,
640,4.731224E-01,1.943124E-01,0.000000E+00,
645,3.844986E-01,1.547397E-01,0.000000E+00,
650,2.997374E-01,1.193120E-01,0.000000E+00,
655,2.277792E-01,8.979594E-02,0.000000E+00,
660,1.707914E-01,6.671045E-02,0.000000E+00,
665,1.263808E-01,4.899699E-02,0.000000E+00,
670,9.224597E-02,3.559982E-02,0.000000E+00,
675,6.639960E-02,2.554223E-02,0.000000E+00,
680,4.710606E-02,1.807939E-02,0.000000E+00,
685,3.292138E-02,1.261573E-02,0.000000E+00,
690,2.262306E-02,8.661284E-03,0.000000E+00,
695,1.575417E-02,6.027677E-03,0.000000E+00,
700,1.096778E-02,4.195941E-03,0.000000E+00,
705,7.608750E-03,2.910864E-03,0.000000E+00,
710,5.214608E-03,1.995557E-03,0.000000E+00,
715,3.569452E-03,1.367022E-03,0.000000E+00,
720,2.464821E-03,9.447269E-04,0.000000E+00,
725,1.703876E-03,6.537050E-04,0.000000E+00,
730,1.186238E-03,4.555970E-04,0.000000E+00,
735,8.269535E-04,3.179738E-04,0.000000E+00,
740,5.758303E-04,2.217445E-04,0.000000E+00,
745,4.058303E-04,1.565566E-04,0.000000E+00,
750,2.856577E-04,1.103928E-04,0.000000E+00,
755,2.021853E-04,7.827442E-05,0.000000E+00,
760,1.438270E-04,5.578862E-05,0.000000E+00,
765,1.024685E-04,3.981884E-05,0.000000E+00,
770,7.347551E-05,2.860175E-05,0.000000E+00,
775,5.259870E-05,2.051259E-05,0.000000E+00,
780,3.806114E-05,1.487243E-05,0.000000E+00,
785,2.758222E-05,1.080001E-05,0.000000E+00,
790,2.004122E-05,7.863920E-06,0.000000E+00,
795,1.458792E-05,5.736935E-06,0.000000E+00,
800,1.068141E-05,4.211597E-06,0.000000E+00,
805,7.857521E-06,3.106561E-06,0.000000E+00,
810,5.768284E-06,2.286786E-06,0.000000E+00,
815,4.259166E-06,1.693147E-06,0.000000E+00,
820,3.167765E-06,1.262556E-06,0.000000E+00,
825,2.358723E-06,9.422514E-07,0.000000E+00,
830,1.762465E-06,7.053860E-07,0.000000E+00
};

}


namespace CIE2006
{

// Dla współczynników LMS, a nie CMF.
// double XYZtrans_params[3][3] =
// {
	// {1.94735469,	-1.41445123,	0.36476327},
	// {0.68990272,	0.34832189,	    0.0},
	// {0.0,	        0.0,		    1.93485343}
// };

double XYZtrans_params[3][3] =
{//współczynniki są zapisane bezpośrednio w postaci docelowej, nie musimy skalować
	{1.0,	0.0,	0.0},
	{0.0,	1.0,    0.0},
	{0.0,   0.0,    1.0}
};

}

namespace CIE1931
{

double XYZtrans_params[3][3] =
{//współczynniki są zapisane bezpośrednio w postaci docelowej, nie musimy skalować
	{1.0,	0.0,	0.0},
	{0.0,	1.0,    0.0},
	{0.0,   0.0,    1.0}
};

}


namespace COLORSPACE_sRGB
{
// sRGB
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{3.2404542,		-1.5371385,		-0.4985314},
	{-0.9692660,	1.8760108,		0.0415560},
	{0.0556434,		-0.2040259,		1.0572252}
};

}


namespace COLORSPACE_CIERGB
{
// CIE RGB
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{2.3706743,		-0.9000405,		-0.4706338},
	{-0.5138850,	1.4253036,		0.0885814},
	{0.0052982,		-0.0146949,		1.0093968}
};

}

namespace COLORSPACE_ADOBE_RGB
{
// ADOBE RGB D65
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 2.0413690, -0.5649464, -0.3446944},
	{-0.9692660,  1.8760108,  0.0415560},
	{ 0.0134474, -0.1183897,  1.0154096}
};

}

namespace COLORSPACE_APPLE_RGB
{
// Apple RGB D65
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 2.9515373, -1.2894116, -0.4738445},
	{-1.0851093,  1.9908566,  0.0372026},
	{ 0.0854934, -0.2694964,  1.0912975}
};

}

namespace COLORSPACE_BEST_RGB
{
// Best RGB D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 1.7552599, -0.4836786, -0.2530000},
	{-0.5441336,  1.5068789,  0.0215528},
	{ 0.0063467, -0.0175761,  1.2256959}
};

}

namespace COLORSPACE_BETA_RGB
{
// Beta RGB D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 1.6832270, -0.4282363, -0.2360185},
	{-0.7710229,  1.7065571,  0.0446900},
	{ 0.0400013, -0.0885376,  1.2723640}
};

}

namespace COLORSPACE_BRUCE_RGB
{
// Bruce RGB D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 2.7454669, -1.1358136, -0.4350269},
	{-0.9692660,  1.8760108,  0.0415560},
	{ 0.0112723, -0.1139754,  1.0132541}
};

}

namespace COLORSPACE_COLORMATCH_RGB
{
// ColorMatch RGB D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 2.6422874, -1.2234270, -0.3930143},
	{-1.1119763,  2.0590183,  0.0159614},
	{ 0.0821699, -0.2807254,  1.4559877}
};

}

namespace COLORSPACE_DON_RGB_4
{
// DON RGB 4 D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 1.7603902, -0.4881198, -0.2536126},
	{-0.7126288,  1.6527432,  0.0416715},
	{ 0.0078207, -0.0347411,  1.2447743}
};

}

namespace COLORSPACE_ECI_RGB
{
// ECI RGB D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 1.7827618, -0.4969847, -0.2690101},
	{-0.9593623,  1.9477962, -0.0275807},
	{ 0.0859317, -0.1744674,  1.3228273}
};

}

namespace COLORSPACE_EKTA_SPACE_PS5
{
// Ekta Space PS5 D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 2.0043819, -0.7304844, -0.2450052},
	{-0.7110285,  1.6202126,  0.0792227},
	{ 0.0381263, -0.0868780,  1.2725438}
};

}

namespace COLORSPACE_NTSC_RGB
{
// NTSC RGB C
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 1.9099961, -0.5324542, -0.2882091},
	{-0.9846663,  1.9991710, -0.0283082},
	{ 0.0583056, -0.1183781,  0.8975535}
};

}

namespace COLORSPACE_PALSECAM_RGB
{
// PLA/SECAM RGB D65
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 3.0628971, -1.3931791, -0.4757517},
	{-0.9692660,  1.8760108,  0.0415560},
	{ 0.0678775, -0.2288548,  1.0693490}
};

}

namespace COLORSPACE_PRO_PHOTO_RGB
{
// PRO PHOTO RGB D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 1.3459433, -0.2556075, -0.0511118},
	{-0.5445989,  1.5081673,  0.0205351},
	{ 0.0000000,  0.0000000,  1.2118128}
};

}

namespace COLORPSPACE_SMPTE_RGB
{
// SMPTE-C RGB D65
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 3.5053960, -1.7394894, -0.5439640},
	{-1.0690722,  1.9778245,  0.0351722},
	{ 0.0563200, -0.1970226,  1.0502026}
};

}

namespace COLORSPACE_WIDE_GAMUT_RGB
{
// WIDE GAMUT RGB D50
//http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
double RGBtrans_params[3][3] =
{//linear transform
	{ 1.4628067, -0.1840623, -0.2743606},
	{-0.5217933,  1.4472381,  0.0677227},
	{ 0.0349342, -0.0968930,  1.2884099}
};

}

