/*#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include "imgui.h"
#include "../ImGui/backends/imgui_impl_sdl3.h"
#include "../ImGui/backends/imgui_impl_sdlrenderer3.h"

#include "../include/algorithms/KMC.h"
#include "../include/algorithms/KNN.h"

int main(void)
{
	SDL_Init(SDL_INIT_VIDEO);

	std::vector<Point_T> points;

	// ===== Cluster 1 (Top-left) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 150 + (rand() % 250);
		float y = 100 + (rand() % 200);

		Color_T color = { 255, 0, 0, 255 };
		points.push_back(Point_T(x, y, color, 1));
	}

	// ===== Cluster 2 (Top-center) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 800 + (rand() % 250);
		float y = 100 + (rand() % 200);

		Color_T color = { 0, 255, 0, 255 };
		points.push_back(Point_T(x, y, color, 2));
	}

	// ===== Cluster 3 (Top-right) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 1500 + (rand() % 250);
		float y = 100 + (rand() % 200);

		Color_T color = { 0, 0, 255, 255 };
		points.push_back(Point_T(x, y, color, 3));
	}

	// ===== Cluster 4 (Bottom-left) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 150 + (rand() % 250);
		float y = 700 + (rand() % 250);

		Color_T color = { 255, 255, 0, 255 };
		points.push_back(Point_T(x, y, color, 4));
	}

	// ===== Cluster 5 (Bottom-center) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 800 + (rand() % 250);
		float y = 700 + (rand() % 250);

		Color_T color = { 255, 0, 255, 255 };
		points.push_back(Point_T(x, y, color, 5));
	}

	// ===== Cluster 6 (Bottom-right) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 1500 + (rand() % 250);
		float y = 700 + (rand() % 250);

		Color_T color = { 0, 255, 255, 255 };
		points.push_back(Point_T(x, y, color, 6));
	}

	// ===== Noise =====
	for (int i = 0; i < 40; ++i)
	{
		float x = rand() % 1920;
		float y = rand() % 1080;

		Color_T color = { 180, 180, 180, 255 };
		points.push_back(Point_T(x, y, color, 0)); // 0 = noise/unclassified
	}

	Point_T unknownPoint(700, 900, { 0, 0, 0, 255 });

	KNearestNeighbor knn(points, unknownPoint, 18);

	SDL_Window* window = SDL_CreateWindow("AlgoLab AI", 800, 600, SDL_WINDOW_RESIZABLE);

	if (!window)
	{
		SDL_Quit();

		std::cerr << "Failed to initialize window!\n";

		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	if (!renderer)
	{
		SDL_DestroyWindow(window);

		SDL_Quit();

		std::cerr << "Failed to initialize renderer!\n";

		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	bool is_running = true;
	SDL_Event event;

	while (is_running)
	{
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
			{
				is_running = false;
			}
		}

		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("K-Means Clustering");

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Points"))
		{
			for (size_t i = 0; i < points.size(); ++i)
			{
				std::string name = std::string("Point: ") + std::to_string(i);
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					float prevX = points[i].Position.X;
					float prevY = points[i].Position.Y;

					if (ImGui::CollapsingHeader("Position"))
					{
						ImGui::InputFloat("X ", &points[i].Position.X);
						ImGui::InputFloat("Y ", &points[i].Position.Y);
					}

					if (ImGui::CollapsingHeader("Color"))
					{
						ImGui::InputInt("Red", &points[i].Color.Red);
						ImGui::InputInt("Green", &points[i].Color.Green);
						ImGui::InputInt("Blue", &points[i].Color.Blue);
						ImGui::InputInt("Alpha", &points[i].Color.Alpha);
					}

					if (prevX != points[i].Position.X || prevY != points[i].Position.Y)
					{
						points[i].rerect();
					}
				}
			}
		}

		if (ImGui::CollapsingHeader("Unknown Point"))
		{
			float prevX = knn.m_TargetPoint->Position.X;
			float prevY = knn.m_TargetPoint->Position.X;

			if (ImGui::CollapsingHeader("Position"))
			{
				ImGui::InputFloat("X ", &knn.m_TargetPoint->Position.X);
				ImGui::InputFloat("Y ", &knn.m_TargetPoint->Position.Y);
			}

			if (ImGui::CollapsingHeader("Color"))
			{
				ImGui::InputInt("Red", &knn.m_TargetPoint->Color.Red);
				ImGui::InputInt("Green", &knn.m_TargetPoint->Color.Green);
				ImGui::InputInt("Blue", &knn.m_TargetPoint->Color.Blue);
				ImGui::InputInt("Alpha", &knn.m_TargetPoint->Color.Alpha);
			}

			if (prevX != knn.m_TargetPoint->Position.X || prevY != knn.m_TargetPoint->Position.Y)
			{
				knn.m_TargetPoint->rerect();
			}
		}

		if (ImGui::Button("Run"))
		{
			std::cout << "should simulate!\n";
			knn.Simulate();
		}

		ImGui::End();

		ImGui::Render();

		SDL_SetRenderDrawColor(renderer, 45, 45, 75, 255);
		SDL_RenderClear(renderer);

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

		for (size_t i = 0; i < points.size(); ++i)
		{
			points[i].render(renderer);
		}

		knn.m_TargetPoint->render(renderer);

		SDL_RenderPresent(renderer);
	}

	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}*/