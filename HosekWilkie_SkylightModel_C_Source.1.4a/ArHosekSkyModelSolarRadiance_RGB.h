#ifndef __SOLARRADIANCERGB
#define __SOLARRADIANCERGB

#include "ArHosekSkyModel.h"

double solar_radiance_RGB(
		ArHosekSkyModelState  * state,
		int						channel,
		double                  elevation,
		double                  gamma
		);

double arhosekskymodel_solar_radiance_internal2(
		ArHosekSkyModelState  * state,
		double                  wavelength,
		double                  elevation,
		double                  gamma
		);
#endif
