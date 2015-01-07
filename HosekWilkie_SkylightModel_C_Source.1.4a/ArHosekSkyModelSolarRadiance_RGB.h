#ifndef __SOLARRADIANCERGB
#define __SOLARRADIANCERGB

#include "ArHosekSkyModel.h"

double solar_radiance_RGB(
		ArHosekSkyModelState  * state,
		int						channel,
		double                  elevation,
		double                  gamma
		);


#endif
