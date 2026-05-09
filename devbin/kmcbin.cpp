/*#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include "imgui.h"
#include "../ImGui/backends/imgui_impl_sdl3.h"
#include "../ImGui/backends/imgui_impl_sdlrenderer3.h"

#include "../include/algorithms/KMC.h"

int main(void)
{
	SDL_Init(SDL_INIT_VIDEO);

	std::vector<Point_T> points;

	// ===== Cluster 1 (Top-left) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 150 + (rand() % 250);
		float y = 100 + (rand() % 200);
		points.push_back(Point_T(x, y, { 255, 0, 0, 255 }));
	}

	// ===== Cluster 2 (Top-center) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 800 + (rand() % 250);
		float y = 100 + (rand() % 200);
		points.push_back(Point_T(x, y, { 0, 255, 0, 255 }));
	}

	// ===== Cluster 3 (Top-right) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 1500 + (rand() % 250);
		float y = 100 + (rand() % 200);
		points.push_back(Point_T(x, y, { 0, 0, 255, 255 }));
	}

	// ===== Cluster 4 (Bottom-left) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 150 + (rand() % 250);
		float y = 700 + (rand() % 250);
		points.push_back(Point_T(x, y, { 255, 255, 0, 255 }));
	}

	// ===== Cluster 5 (Bottom-center) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 800 + (rand() % 250);
		float y = 700 + (rand() % 250);
		points.push_back(Point_T(x, y, { 255, 0, 255, 255 }));
	}

	// ===== Cluster 6 (Bottom-right) =====
	for (int i = 0; i < 50; ++i)
	{
		float x = 1500 + (rand() % 250);
		float y = 700 + (rand() % 250);
		points.push_back(Point_T(x, y, { 0, 255, 255, 255 }));
	}

	// ===== Noise =====
	for (int i = 0; i < 40; ++i)
	{
		float x = rand() % 1920;
		float y = rand() % 1080;
		points.push_back(Point_T(x, y, { 180, 180, 180, 255 }));
	}

	std::vector<Centroid_T> centroids;

	centroids.push_back(Centroid_T(200, 200, { 255, 255, 255, 255 }));
	centroids.push_back(Centroid_T(900, 150, { 255, 128, 128, 255 }));
	centroids.push_back(Centroid_T(1700, 200, { 128, 255, 128, 255 }));
	centroids.push_back(Centroid_T(200, 900, { 128, 128, 255, 255 }));
	centroids.push_back(Centroid_T(900, 850, { 255, 255, 128, 255 }));
	centroids.push_back(Centroid_T(1700, 900, { 255, 128, 255, 255 }));

	KMeansClustering kmc(points, centroids);

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

		if (ImGui::CollapsingHeader("Centroids"))
		{
			for (size_t i = 0; i < kmc.m_Centroids.size(); ++i)
			{
				std::string name = std::string("Centroid: ") + std::to_string(i);
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					float prevX = kmc.m_Centroids[i].Position.X;
					float prevY = kmc.m_Centroids[i].Position.Y;

					if (ImGui::CollapsingHeader("Position"))
					{
						ImGui::InputFloat("X ", &kmc.m_Centroids[i].Position.X);
						ImGui::InputFloat("Y ", &kmc.m_Centroids[i].Position.Y);
					}

					if (ImGui::CollapsingHeader("Color"))
					{
						ImGui::InputInt("Red", &kmc.m_Centroids[i].Color.Red);
						ImGui::InputInt("Green", &kmc.m_Centroids[i].Color.Green);
						ImGui::InputInt("Blue", &kmc.m_Centroids[i].Color.Blue);
						ImGui::InputInt("Alpha", &kmc.m_Centroids[i].Color.Alpha);
					}

					if (prevX != kmc.m_Centroids[i].Position.X || prevY != kmc.m_Centroids[i].Position.Y)
					{
						kmc.m_Centroids[i].rerect();
					}
				}
			}
		}

		if (ImGui::CollapsingHeader("Points"))
		{
			for (size_t i = 0; i < kmc.m_Points.size(); ++i)
			{
				std::string name = std::string("Point: ") + std::to_string(i);
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					float prevX = kmc.m_Points[i].Position.X;
					float prevY = kmc.m_Points[i].Position.Y;

					if (ImGui::CollapsingHeader("Position"))
					{
						ImGui::InputFloat("X ", &kmc.m_Points[i].Position.X);
						ImGui::InputFloat("Y ", &kmc.m_Points[i].Position.Y);
					}

					if (ImGui::CollapsingHeader("Color"))
					{
						ImGui::InputInt("Red", &kmc.m_Points[i].Color.Red);
						ImGui::InputInt("Green", &kmc.m_Points[i].Color.Green);
						ImGui::InputInt("Blue", &kmc.m_Points[i].Color.Blue);
						ImGui::InputInt("Alpha", &kmc.m_Points[i].Color.Alpha);
					}

					if (prevX != kmc.m_Points[i].Position.X || prevY != kmc.m_Points[i].Position.Y)
					{
						kmc.m_Points[i].rerect();
					}
				}
			}
		}

		if (ImGui::Button("Run"))
		{
			std::cout << "should simulate!\n";
			kmc.Simulate();
		}

		ImGui::End();

		ImGui::Render();

		SDL_SetRenderDrawColor(renderer, 45, 45, 75, 255);
		SDL_RenderClear(renderer);

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

		for (size_t i = 0; i < kmc.m_Centroids.size(); ++i)
		{
			kmc.m_Centroids[i].render(renderer);
		}

		for (size_t i = 0; i < kmc.m_Points.size(); ++i)
		{
			kmc.m_Points[i].render(renderer);

			if (kmc.m_Points[i].Cluster >= 0 && kmc.m_Points[i].Cluster < kmc.m_Centroids.size())
			{
				SDL_SetRenderDrawColor(renderer, kmc.m_Centroids[kmc.m_Points[i].Cluster].Color.Red, kmc.m_Centroids[kmc.m_Points[i].Cluster].Color.Green, kmc.m_Centroids[kmc.m_Points[i].Cluster].Color.Blue, kmc.m_Centroids[kmc.m_Points[i].Cluster].Color.Alpha);
				SDL_RenderLine(renderer, kmc.m_Points[i].Position.X + (kmc.m_Points[i].Size.X / 2), kmc.m_Points[i].Position.Y + (kmc.m_Points[i].Size.Y / 2), kmc.m_Centroids[kmc.m_Points[i].Cluster].Position.X + (kmc.m_Centroids[kmc.m_Points[i].Cluster].Size.X / 2), kmc.m_Centroids[kmc.m_Points[i].Cluster].Position.Y + (kmc.m_Centroids[kmc.m_Points[i].Cluster].Size.Y / 2));
			}
		}


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