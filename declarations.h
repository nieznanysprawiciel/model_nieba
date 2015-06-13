#ifndef DECLARATIONS_H
#define DECLARATIONS_H

struct SkyData
{
	float   vertical_angle;			///<Aktualny obrót
	float   horizontal_angle;		///<Aktualny obrót
	float	view_angle;				///<Kąt widzenia
    int     vertical_pixels;
    int     horizontal_pixels;
    int     near_plane;
	double	albedo[10];
	float	turbidity;
};



#endif // DECLARATIONS_H
