#ifndef CENTROID_H

#define CENTROID_H

#include "utils.h"
#include <SDL3/SDL.h>
#include <string>

class Centroid_T
{
public:
	std::string Name;
	double Inertia;
	Vector2_T Position;
	Vector2_T Size;
	Color_T Color;
	SDL_FRect Rectangle;

	Centroid_T(const std::string& name, float x, float y, Color_T color)
	{
		this->Name = name;
		this->Position = { x, y };
		this->Size = { 25, 25 };
		this->Inertia = 0.0;
		this->Color = { color.Red, color.Green, color.Blue, color.Alpha };

		this->Rectangle = { this->Position.X, this->Position.Y, this->Size.X, this->Size.Y };
	}

	Vector2_T center()
	{
		return Vector2_T(this->Position.X + (this->Size.X / 2), this->Position.Y + (this->Size.Y / 2));
	}

	void rerect()
	{
		this->Rectangle.x = this->Position.X;
		this->Rectangle.y = this->Position.Y;
	}

	void render(SDL_Renderer* renderer, int texWidth, int texHeight)
	{
		if (!renderer) return;

		float scaleX = (float)texWidth / 1920.0f;
		float scaleY = (float)texHeight / 1080.0f;

		SDL_FRect drawRect;
		drawRect.x = this->Rectangle.x * scaleX;
		drawRect.y = this->Rectangle.y * scaleY;
		drawRect.w = this->Rectangle.w * scaleX;
		drawRect.h = this->Rectangle.h * scaleY;

		SDL_SetRenderDrawColor(renderer,
			this->Color.Red,
			this->Color.Green,
			this->Color.Blue,
			this->Color.Alpha);

		SDL_RenderFillRect(renderer, &drawRect);
	}
};

#endif