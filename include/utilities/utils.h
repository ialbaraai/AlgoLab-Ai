#ifndef UTILS_H

#define UTILS_H

typedef int ColorIDX;

typedef struct VECTOR2_STRUCT
{
	float X;
	float Y;
} Vector2_T;

typedef struct COLOR_STRUCT
{
	ColorIDX Red;
	ColorIDX Green;
	ColorIDX Blue;
	ColorIDX Alpha;
} Color_T;

#endif