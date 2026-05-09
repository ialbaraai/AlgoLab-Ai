#ifndef POINT_H

#define POINT_H

#include "utils.h"
#include "centroid.h"
#include <SDL3/SDL.h>

class Point_T
{
public:
	std::string Name;
	int Cluster;
	Vector2_T Position;
	Vector2_T Size;
	Color_T Color;
	SDL_FRect Rectangle;

	Point_T(const std::string& name, float x, float y, Color_T color)
	{
		this->Name = name;
		this->Position = { x, y };
		this->Size = { 10, 10 };
		this->Cluster = -1;
		this->Color = color;

		this->Rectangle = { this->Position.X, this->Position.Y, this->Size.X, this->Size.Y };
	}
	Point_T(const std::string& name, float x, float y, Color_T color, int cluster)
	{
		this->Name = name;
		this->Position = { x, y };
		this->Size = { 10, 10 };
		this->Cluster = cluster;
		this->Color = color;

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
		this->Rectangle.w = this->Size.X;
		this->Rectangle.h = this->Size.Y;
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

	void renderline(const Centroid_T& centroid, SDL_Renderer* renderer, int texWidth, int texHeight)
	{
		if (!renderer) return;

		float scaleX = (float)texWidth / 1920.0f;
		float scaleY = (float)texHeight / 1080.0f;

		SDL_FRect drawRect;
		drawRect.x = this->Rectangle.x * scaleX;
		drawRect.y = this->Rectangle.y * scaleY;
		drawRect.w = this->Rectangle.w * scaleX;
		drawRect.h = this->Rectangle.h * scaleY;

		SDL_FRect centroidRect;
		centroidRect.x = centroid.Rectangle.x * scaleX;
		centroidRect.y = centroid.Rectangle.y * scaleY;
		centroidRect.w = centroid.Rectangle.w * scaleX;
		centroidRect.h = centroid.Rectangle.h * scaleY;

		SDL_RenderLine(renderer, drawRect.x + (drawRect.w / 2), drawRect.y + (drawRect.h / 2), centroidRect.x + (centroidRect.w / 2), centroidRect.y + (centroidRect.h / 2));
	}
};

#endif