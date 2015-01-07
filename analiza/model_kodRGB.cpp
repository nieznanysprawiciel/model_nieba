typedef struct ArHosekSkyModelState
{
    ArHosekSkyModelConfiguration  configs[11];
    double                        radiances[11];
    double                        turbidity;
    double                        solar_radius;
    double                        emission_correction_factor_sky[11];
    double                        emission_correction_factor_sun[11];
    double                        albedo;
    double                        elevation;
} 
ArHosekSkyModelState;

//----------------------------------------------------------------------//
//							init										//
//----------------------------------------------------------------------//

ArHosekSkyModelState  * arhosek_rgb_skymodelstate_alloc_init(
        const double  turbidity, 
        const double  albedo, 
        const double  elevation
        )
{
    ArHosekSkyModelState* state = ALLOC(ArHosekSkyModelState);
    
    state->solar_radius = TERRESTRIAL_SOLAR_RADIUS;
    state->turbidity    = turbidity;
    state->albedo       = albedo;
    state->elevation    = elevation;

    unsigned int channel = 0;
    for( ; channel < 3; ++channel )
    {
        ArHosekSkyModel_CookConfiguration(
            datasetsRGB[channel], 
            state->configs[channel], 
            turbidity, 
            albedo, 
            elevation
            );
        
        state->radiances[channel] = 
        ArHosekSkyModel_CookRadianceConfiguration(
            datasetsRGBRad[channel],
            turbidity, 
            albedo,
            elevation
            );
    }
    
    return state;
}
void ArHosekSkyModel_CookConfiguration(
        ArHosekSkyModel_Dataset       dataset, 
        ArHosekSkyModelConfiguration  config, 
        double                        turbidity, 
        double                        albedo, 
        double                        solar_elevation
        )
{
    double  * elev_matrix;

    int     int_turbidity = (int)turbidity;
    double  turbidity_rem = turbidity - (double)int_turbidity;

    solar_elevation = pow(solar_elevation / (MATH_PI / 2.0), (1.0 / 3.0));

    // alb 0 low turb

    elev_matrix = dataset + ( 9 * 6 * (int_turbidity-1) );
    
    unsigned int i = 0;
    for( ; i < 9; ++i )
    {
        //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
        config[i] = 
        (1.0-albedo) * (1.0 - turbidity_rem) 
        * ( pow(1.0-solar_elevation, 5.0) * elev_matrix[i]  + 
           5.0  * pow(1.0-solar_elevation, 4.0) * solar_elevation * elev_matrix[i+9] +
           10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[i+18] +
           10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[i+27] +
           5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[i+36] +
           pow(solar_elevation, 5.0)  * elev_matrix[i+45]);
    }

    // alb 1 low turb
    elev_matrix = dataset + (9*6*10 + 9*6*(int_turbidity-1));
    for(i = 0; i < 9; ++i)
    {
        //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
        config[i] += 
        (albedo) * (1.0 - turbidity_rem)
        * ( pow(1.0-solar_elevation, 5.0) * elev_matrix[i]  + 
           5.0  * pow(1.0-solar_elevation, 4.0) * solar_elevation * elev_matrix[i+9] +
           10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[i+18] +
           10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[i+27] +
           5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[i+36] +
           pow(solar_elevation, 5.0)  * elev_matrix[i+45]);
    }

    if(int_turbidity == 10)
        return;

    // alb 0 high turb
    elev_matrix = dataset + (9*6*(int_turbidity));
    for(i = 0; i < 9; ++i)
    {
        //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
        config[i] += 
        (1.0-albedo) * (turbidity_rem)
        * ( pow(1.0-solar_elevation, 5.0) * elev_matrix[i]  + 
           5.0  * pow(1.0-solar_elevation, 4.0) * solar_elevation * elev_matrix[i+9] +
           10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[i+18] +
           10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[i+27] +
           5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[i+36] +
           pow(solar_elevation, 5.0)  * elev_matrix[i+45]);
    }

    // alb 1 high turb
    elev_matrix = dataset + (9*6*10 + 9*6*(int_turbidity));
    for(i = 0; i < 9; ++i)
    {
        //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
        config[i] += 
        (albedo) * (turbidity_rem)
        * ( pow(1.0-solar_elevation, 5.0) * elev_matrix[i]  + 
           5.0  * pow(1.0-solar_elevation, 4.0) * solar_elevation * elev_matrix[i+9] +
           10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[i+18] +
           10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[i+27] +
           5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[i+36] +
           pow(solar_elevation, 5.0)  * elev_matrix[i+45]);
    }
}

double ArHosekSkyModel_CookRadianceConfiguration(
        ArHosekSkyModel_Radiance_Dataset  dataset, 
        double                            turbidity, 
        double                            albedo, 
        double                            solar_elevation
        )
{
    double* elev_matrix;

    int int_turbidity = (int)turbidity;
    double turbidity_rem = turbidity - (double)int_turbidity;
    double res;
    solar_elevation = pow(solar_elevation / (MATH_PI / 2.0), (1.0 / 3.0));

    // alb 0 low turb
    elev_matrix = dataset + (6*(int_turbidity-1));
    //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
    res = (1.0-albedo) * (1.0 - turbidity_rem) *
        ( pow(1.0-solar_elevation, 5.0) * elev_matrix[0] +
         5.0*pow(1.0-solar_elevation, 4.0)*solar_elevation * elev_matrix[1] +
         10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[2] +
         10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[3] +
         5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[4] +
         pow(solar_elevation, 5.0) * elev_matrix[5]);

    // alb 1 low turb
    elev_matrix = dataset + (6*10 + 6*(int_turbidity-1));
    //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
    res += (albedo) * (1.0 - turbidity_rem) *
        ( pow(1.0-solar_elevation, 5.0) * elev_matrix[0] +
         5.0*pow(1.0-solar_elevation, 4.0)*solar_elevation * elev_matrix[1] +
         10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[2] +
         10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[3] +
         5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[4] +
         pow(solar_elevation, 5.0) * elev_matrix[5]);
    if(int_turbidity == 10)
        return res;

    // alb 0 high turb
    elev_matrix = dataset + (6*(int_turbidity));
    //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
    res += (1.0-albedo) * (turbidity_rem) *
        ( pow(1.0-solar_elevation, 5.0) * elev_matrix[0] +
         5.0*pow(1.0-solar_elevation, 4.0)*solar_elevation * elev_matrix[1] +
         10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[2] +
         10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[3] +
         5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[4] +
         pow(solar_elevation, 5.0) * elev_matrix[5]);

    // alb 1 high turb
    elev_matrix = dataset + (6*10 + 6*(int_turbidity));
    //(1-t).^3* A1 + 3*(1-t).^2.*t * A2 + 3*(1-t) .* t .^ 2 * A3 + t.^3 * A4;
    res += (albedo) * (turbidity_rem) *
        ( pow(1.0-solar_elevation, 5.0) * elev_matrix[0] +
         5.0*pow(1.0-solar_elevation, 4.0)*solar_elevation * elev_matrix[1] +
         10.0*pow(1.0-solar_elevation, 3.0)*pow(solar_elevation, 2.0) * elev_matrix[2] +
         10.0*pow(1.0-solar_elevation, 2.0)*pow(solar_elevation, 3.0) * elev_matrix[3] +
         5.0*(1.0-solar_elevation)*pow(solar_elevation, 4.0) * elev_matrix[4] +
         pow(solar_elevation, 5.0) * elev_matrix[5]);
    return res;
}

//----------------------------------------------------------------------//
//							wywo³anie g³ówne							//
//----------------------------------------------------------------------//


double arhosek_tristim_skymodel_radiance(
    ArHosekSkyModelState  * state,
    double                  theta, 
    double                  gamma, 
    int                     channel
    )
{
    return
        ArHosekSkyModel_GetRadianceInternal(
            state->configs[channel], 
            theta, 
            gamma 
            ) 
        * state->radiances[channel];
}






double ArHosekSkyModel_GetRadianceInternal(
        ArHosekSkyModelConfiguration  configuration, 
        double                        theta, 
        double                        gamma
        )
{
    const double expM = exp(configuration[4] * gamma);
    const double rayM = cos(gamma)*cos(gamma);
    const double mieM = (1.0 + cos(gamma)*cos(gamma)) / pow((1.0 + configuration[8]*configuration[8] - 2.0*configuration[8]*cos(gamma)), 1.5);
    const double zenith = sqrt(cos(theta));

    return (1.0 + configuration[0] * exp(configuration[1] / (cos(theta) + 0.01))) *
            (configuration[2] + configuration[3] * expM + configuration[5] * rayM + configuration[6] * mieM + configuration[7] * zenith);
}





//------------------------------------------------------------//
//	radiance model
//	u³o¿one w kolejnoœci wywo³añ

double arhosekskymodel_radiance(
        ArHosekSkyModelState  * state,
        double                  theta, 
        double                  gamma, 
        double                  wavelength
        )
{
    int low_wl = (wavelength - 320.0 ) / 40.0;

    if ( low_wl < 0 || low_wl >= 11 )
        return 0.0f;

    double interp = fmod((wavelength - 320.0 ) / 40.0, 1.0);

    double val_low = 
          ArHosekSkyModel_GetRadianceInternal(
                state->configs[low_wl],
                theta,
                gamma
              )
        * state->radiances[low_wl]
        * state->emission_correction_factor_sky[low_wl];

    if ( interp < 1e-6 )
        return val_low;

    double result = ( 1.0 - interp ) * val_low;

    if ( low_wl+1 < 11 )
    {
        result +=
              interp
            * ArHosekSkyModel_GetRadianceInternal(
                    state->configs[low_wl+1],
                    theta,
                    gamma
                  )
            * state->radiances[low_wl+1]
            * state->emission_correction_factor_sky[low_wl+1];
    }

    return result;
}



//solar radiance 
double arhosekskymodel_solar_radiance(
        ArHosekSkyModelState  * state,
        double                  theta, 
        double                  gamma, 
        double                  wavelength
        )
{
    double  direct_radiance =
        arhosekskymodel_solar_radiance_internal2(
            state,
            wavelength,
            ((MATH_PI/2.0)-theta),
            gamma
            );

    double  inscattered_radiance =
        arhosekskymodel_radiance(
            state,
            theta,
            gamma,
            wavelength
            );
    
    return  direct_radiance + inscattered_radiance;
}


double arhosekskymodel_solar_radiance_internal2(
        ArHosekSkyModelState  * state,
        double                  wavelength,
        double                  elevation,
        double                  gamma
        )
{
    assert(
           wavelength >= 320.0
        && wavelength <= 720.0
        && state->turbidity >= 1.0
        && state->turbidity <= 10.0
        );
            
    
    int     turb_low  = (int) state->turbidity - 1;
    double  turb_frac = state->turbidity - (double) (turb_low + 1);
    
    if ( turb_low == 9 )
    {
        turb_low  = 8;
        turb_frac = 1.0;
    }

    int    wl_low  = (int) ((wavelength - 320.0) / 40.0);
    double wl_frac = fmod(wavelength, 40.0) / 40.0;
    
    if ( wl_low == 10 )
    {
        wl_low = 9;
        wl_frac = 1.0;
    }

    double direct_radiance =
          ( 1.0 - turb_frac )
        * (    (1.0 - wl_frac)
             * arhosekskymodel_sr_internal(
                     state,
                     turb_low,
                     wl_low,
                     elevation
                   )
           +   wl_frac
             * arhosekskymodel_sr_internal(
                     state,
                     turb_low,
                     wl_low+1,
                     elevation
                   )
          )
      +   turb_frac
        * (    ( 1.0 - wl_frac )
             * arhosekskymodel_sr_internal(
                     state,
                     turb_low+1,
                     wl_low,
                     elevation
                   )
           +   wl_frac
             * arhosekskymodel_sr_internal(
                     state,
                     turb_low+1,
                     wl_low+1,
                     elevation
                   )
          );

    double ldCoefficient[6];
    
    int i = 0;
    for ( i = 0; i < 6; i++ )
        ldCoefficient[i] =
              (1.0 - wl_frac) * limbDarkeningDatasets[wl_low  ][i]
            +        wl_frac  * limbDarkeningDatasets[wl_low+1][i];
    
    // sun distance to diameter ratio, squared

    const double sol_rad_sin = sin(state->solar_radius);
    const double ar2 = 1 / ( sol_rad_sin * sol_rad_sin );
    const double singamma = sin(gamma);
    double sc2 = 1.0 - ar2 * singamma * singamma;
    if (sc2 < 0.0 ) sc2 = 0.0;
    double sampleCosine = sqrt (sc2);
    
    //   The following will be improved in future versions of the model:
    //   here, we directly use fitted 5th order polynomials provided by the
    //   astronomical community for the limb darkening effect. Astronomers need
    //   such accurate fittings for their predictions. However, this sort of
    //   accuracy is not really needed for CG purposes, so an approximated
    //   dataset based on quadratic polynomials will be provided in a future
    //   release.

    double  darkeningFactor =
          ldCoefficient[0]
        + ldCoefficient[1] * sampleCosine
        + ldCoefficient[2] * pow( sampleCosine, 2.0 )
        + ldCoefficient[3] * pow( sampleCosine, 3.0 )
        + ldCoefficient[4] * pow( sampleCosine, 4.0 )
        + ldCoefficient[5] * pow( sampleCosine, 5.0 );

    direct_radiance *= darkeningFactor;

    return direct_radiance;
}



const int pieces = 45;
const int order = 4;

double arhosekskymodel_sr_internal(
        ArHosekSkyModelState  * state,
        int                     turbidity,
        int                     wl,
        double                  elevation
        )
{
    int pos =
        (int) (pow(2.0*elevation / MATH_PI, 1.0/3.0) * pieces); // floor
    
    if ( pos > 44 ) pos = 44;
    
    const double break_x =
        pow(((double) pos / (double) pieces), 3.0) * (MATH_PI * 0.5);

    const double  * coefs =
        solarDatasets[wl] + (order * pieces * turbidity + order * (pos+1) - 1);

    double res = 0.0;
    const double x = elevation - break_x;
    double x_exp = 1.0;

    int i = 0;
    for (i = 0; i < order; ++i)
    {
        res += x_exp * *coefs--;
        x_exp *= x;
    }

    return res * state->emission_correction_factor_sun[wl];
}
