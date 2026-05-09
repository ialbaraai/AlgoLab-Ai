/*#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "imgui.h"
#include "../ImGui/backends/imgui_impl_sdl3.h"
#include "../ImGui/backends/imgui_impl_sdlrenderer3.h"

#include "../include/algorithms/KMC.h"
#include "../include/algorithms/KNN.h"

int main(void)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "Failed to initialize SDL!\n";

		return -1;
	}

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

	std::vector<Point_T> unknownPoints;
	unknownPoints.push_back(Point_T(280.5f, 759.3f, { 0, 0, 0, 255 }));
	unknownPoints.push_back(Point_T(880.5f, 359.3f, { 0, 0, 0, 255 }));

	KMeansClustering kmc;
	KNearestNeighbor knn;

	SDL_Window* window = SDL_CreateWindow("AlgoLab AI", 1050, 625, SDL_WINDOW_RESIZABLE);

	if (!window)
	{
		SDL_Quit();

		std::cerr << "Failed to initialize window!\n";

		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	if (!renderer)
	{
		SDL_DestroyWindow(window);

		SDL_Quit();

		std::cerr << "Failed to initialize renderer!\n";

		return -1;
	}

	SDL_Surface* icon = IMG_Load("algolab_icon.png");

	if (icon)
	{
		SDL_SetWindowIcon(window, icon);

		SDL_DestroySurface(icon);
	}

	SDL_Texture* renderingFrame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);

	if (!renderingFrame)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();

		std::cerr << "Failed to initialize rendering frame!\n";

		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
	ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("Roboto.ttf", 16.0f);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.ChildBorderSize = 0.0f;
	style.FrameBorderSize = 0.0f;

	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.15f, 0.60f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.22f, 0.70f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.22f, 0.10f, 0.50f, 1.0f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.30f, 0.15f, 0.60f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.22f, 0.70f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.10f, 0.50f, 1.0f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.22f, 0.10f, 0.45f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.22f, 0.70f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.15f, 0.60f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.08f, 0.30f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.22f, 0.10f, 0.45f, 1.0f);

	bool is_running = true;
	SDL_Event event;

	int windowX = 1920;
	int windowY = 1080;
	float renderingFrameX = 800;
	float renderingFrameY = 600;

	bool adddata = false;

	while (is_running)
	{
		SDL_GetWindowSize(window, &windowX, &windowY);
		SDL_GetTextureSize(renderingFrame, &renderingFrameX, &renderingFrameY);
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

		ImGui::PushFont(font);

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(windowX, (ImGui::CalcTextSize("AlgoLab Ai").y)));
		ImGui::Begin("Title", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		ImGui::Dummy(ImVec2((ImGui::GetWindowSize().x / 2) - (ImGui::CalcTextSize("AlgoLab Ai").x / 2), 0));
		ImGui::SameLine();
		ImGui::Text("AlgoLab Ai");

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, 25));
		ImGui::SetNextWindowSize(ImVec2(250, windowY - (ImGui::CalcTextSize("AlgoLab Ai").y + (4 * (ImGui::CalcTextSize("Add").y + 2.5)))));
		ImGui::Begin("Data", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		if (ImGui::CollapsingHeader("Centroids"))
		{
			ImGui::Indent();
			for (size_t i = 0; i < centroids.size(); ++i)
			{
				std::string name = std::string("Centroid: ") + std::to_string(i);

				if (ImGui::CollapsingHeader(name.c_str()))
				{
					float prevX = centroids[i].Position.X;
					float prevY = centroids[i].Position.Y;

					ImGui::Indent();
					if (ImGui::CollapsingHeader(("Position##centroid_" + std::to_string(i)).c_str()))
					{
						ImGui::SetNextItemWidth(100.0f);
						ImGui::InputFloat(("X##centroid_" + std::to_string(i)).c_str(), &centroids[i].Position.X, 0.0f, 0.0f, "%.2f");
						ImGui::SetNextItemWidth(100.0f);
						ImGui::InputFloat(("Y##centroid_" + std::to_string(i)).c_str(), &centroids[i].Position.Y, 0.0f, 0.0f, "%.2f");
					}

					if (ImGui::CollapsingHeader(("Color##centroid_" + std::to_string(i)).c_str()))
					{
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Red##centroid_" + std::to_string(i)).c_str(), &centroids[i].Color.Red, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Green##centroid_" + std::to_string(i)).c_str(), &centroids[i].Color.Green, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Blue##centroid_" + std::to_string(i)).c_str(), &centroids[i].Color.Blue, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Alpha##centroid_" + std::to_string(i)).c_str(), &centroids[i].Color.Alpha, 0, 255);


					}
					ImGui::Unindent();

					if (prevX != centroids[i].Position.X || prevY != centroids[i].Position.Y)
					{
						centroids[i].rerect();
					}
				}
			}
			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Points"))
		{
			ImGui::Indent();
			for (size_t i = 0; i < points.size(); ++i)
			{
				std::string name = std::string("Point: ") + std::to_string(i);
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					float prevX = points[i].Position.X;
					float prevY = points[i].Position.Y;

					ImGui::Indent();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragInt(("Cluster##point_" + std::to_string(i)).c_str(), &unknownPoints[i].Cluster, 1.0f, -1, centroids.size());
					if (ImGui::CollapsingHeader(("Position##point_" + std::to_string(i)).c_str()))
					{
						ImGui::SetNextItemWidth(100.0f);
						ImGui::InputFloat(("X##point_" + std::to_string(i)).c_str(), &points[i].Position.X, 0.0f, 0.0f, "%.2f");
						ImGui::SetNextItemWidth(100.0f);
						ImGui::InputFloat(("Y##point_" + std::to_string(i)).c_str(), &points[i].Position.Y, 0.0f, 0.0f, "%.2f");
					}

					if (ImGui::CollapsingHeader(("Color##point_" + std::to_string(i)).c_str()))
					{
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Red##point_" + std::to_string(i)).c_str(), &points[i].Color.Red, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Green##point_" + std::to_string(i)).c_str(), &points[i].Color.Green, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Blue##point_" + std::to_string(i)).c_str(), &points[i].Color.Blue, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Alpha##point_" + std::to_string(i)).c_str(), &points[i].Color.Alpha, 0, 255);
					}
					ImGui::Unindent();

					if (prevX != points[i].Position.X || prevY != points[i].Position.Y)
					{
						points[i].rerect();
					}
				}
			}
			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Unknown Points"))
		{
			ImGui::Indent();
			for (size_t i = 0; i < unknownPoints.size(); ++i)
			{
				std::string name = std::string("Unkown Point: ") + std::to_string(i);

				if (ImGui::CollapsingHeader(name.c_str()))
				{
					float prevX = unknownPoints[i].Position.X;
					float prevY = unknownPoints[i].Position.Y;

					ImGui::Indent();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragInt(("Cluster##unknown_" + std::to_string(i)).c_str(), &unknownPoints[i].Cluster, 1.0f, -1, centroids.size());
					if (ImGui::CollapsingHeader(("Position##unknown_" + std::to_string(i)).c_str()))
					{
						ImGui::SetNextItemWidth(100.0f);
						ImGui::InputFloat(("X##unknown_" + std::to_string(i)).c_str(), &unknownPoints[i].Position.X, 0.0f, 0.0f, "%.2f");
						ImGui::SetNextItemWidth(100.0f);
						ImGui::InputFloat(("Y##unknown_" + std::to_string(i)).c_str(), &unknownPoints[i].Position.Y, 0.0f, 0.0f, "%.2f");
					}

					if (ImGui::CollapsingHeader(("Color##unknown_" + std::to_string(i)).c_str()))
					{
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Red##unknown_" + std::to_string(i)).c_str(), &unknownPoints[i].Color.Red, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Green##unknown_" + std::to_string(i)).c_str(), &unknownPoints[i].Color.Green, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Blue##unknown_" + std::to_string(i)).c_str(), &unknownPoints[i].Color.Blue, 0, 255);
						ImGui::SetNextItemWidth(100.0f);
						ImGui::SliderInt(("Alpha##unknow_" + std::to_string(i)).c_str(), &unknownPoints[i].Color.Alpha, 0, 255);
					}
					ImGui::Unindent();

					if (prevX != unknownPoints[i].Position.X || prevY != unknownPoints[i].Position.Y)
					{
						unknownPoints[i].rerect();
					}
				}
			}
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, windowY - 4 * (ImGui::CalcTextSize("Add").y + 2.5)));
		ImGui::SetNextWindowSize(ImVec2(250, ImGui::CalcTextSize("Add").y + 25));
		ImGui::Begin("Addition", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		float abutton_width = 230.0f;
		float aavailable_width = ImGui::GetContentRegionAvail().x;
		float aoffset = (aavailable_width - abutton_width) * 0.5f;

		if (aoffset > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + aoffset);

		if (ImGui::Button("Add", ImVec2(abutton_width, 0)))
		{
			adddata = !adddata;
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, windowY - 2 * (ImGui::CalcTextSize("Add").y + 2.5)));
		ImGui::SetNextWindowSize(ImVec2(250, ImGui::CalcTextSize("Add").y + 25));
		ImGui::Begin("Simulation", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		float sbutton_width = 230.0f;
		float savailable_width = ImGui::GetContentRegionAvail().x;
		float soffset = (savailable_width - sbutton_width) * 0.5f;

		if (soffset > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + soffset);

		if (ImGui::Button("Run", ImVec2(sbutton_width, 0)))
		{
			kmc.Simulate(points, centroids);
			knn.Simulate(points, unknownPoints, 25);
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(windowX - 250, windowY - 25));
		ImGui::SetNextWindowPos(ImVec2(250, 25));
		ImGui::Begin("Viewport", nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse
		);
		ImGui::PopStyleVar();

		ImVec2 availabeSpace = ImGui::GetContentRegionAvail();

		constexpr float TARGET_ASPECT = 1920.0f / 1080.0f;

		int newX = (int)(availabeSpace.x - 10);
		int newY = (int)(newX / TARGET_ASPECT);

		if (newY > availabeSpace.y - 10)
		{
			newY = (int)(availabeSpace.y - 10);
			newX = (int)(newY * TARGET_ASPECT);
		}

		if (newX != (int)renderingFrameX || newY != (int)renderingFrameY)
		{
			if (renderingFrame)
				SDL_DestroyTexture(renderingFrame);

			renderingFrame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, newX, newY);

			if (!renderingFrame)
			{
				ImGui_ImplSDLRenderer3_Shutdown();
				ImGui_ImplSDL3_Shutdown();
				ImGui::DestroyContext();

				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();

				std::cout << "Error: " << SDL_GetError() << '\n';

				return -1;
			}

			renderingFrameX = newX;
			renderingFrameY = newY;
		}

		float offsetX = (availabeSpace.x - renderingFrameX) / 2.0f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

		ImGui::Image((ImTextureID)renderingFrame, ImVec2(renderingFrameX, renderingFrameY));

		ImGui::End();

		if (adddata)
		{
			ImGui::Begin("Add Data", &adddata, ImGuiWindowFlags_NoCollapse);

			if (ImGui::BeginTabBar("Add Data Structures"))
			{
				if (ImGui::BeginTabItem("Point"))
				{


					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Centroid"))
				{
					ImGui::Text("Hello, people!\n");

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::End();
		}

		ImGui::PopFont();

		ImGui::Render();

		SDL_SetRenderDrawColor(renderer, 45, 45, 75, 255);
		SDL_RenderClear(renderer);

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

		SDL_SetRenderTarget(renderer, renderingFrame);

		SDL_SetRenderDrawColor(renderer, 45, 45, 75, 255);
		SDL_RenderClear(renderer);

		for (size_t i = 0; i < centroids.size(); ++i)
		{
			centroids[i].render(renderer, renderingFrameX, renderingFrameY);
		}

		for (size_t i = 0; i < points.size(); ++i)
		{
			points[i].render(renderer, renderingFrameX, renderingFrameY);

			if (points[i].Cluster >= 0 && points[i].Cluster < centroids.size())
			{
				points[i].Color = centroids[points[i].Cluster].Color;
				SDL_SetRenderDrawColor(renderer, centroids[points[i].Cluster].Color.Red, centroids[points[i].Cluster].Color.Green, centroids[points[i].Cluster].Color.Blue, centroids[points[i].Cluster].Color.Alpha);
				points[i].renderline(centroids[points[i].Cluster], renderer, renderingFrameX, renderingFrameY);
			}
		}

		for (size_t i = 0; i < unknownPoints.size(); ++i)
		{
			unknownPoints[i].render(renderer, renderingFrameX, renderingFrameY);
			if (unknownPoints[i].Cluster >= 0 && unknownPoints[i].Cluster < centroids.size())
			{
				unknownPoints[i].Color = centroids[unknownPoints[i].Cluster].Color;
				SDL_SetRenderDrawColor(renderer, centroids[unknownPoints[i].Cluster].Color.Red, centroids[unknownPoints[i].Cluster].Color.Green, centroids[unknownPoints[i].Cluster].Color.Blue, centroids[unknownPoints[i].Cluster].Color.Alpha);
				unknownPoints[i].renderline(centroids[unknownPoints[i].Cluster], renderer, renderingFrameX, renderingFrameY);
			}
		}

		SDL_SetRenderTarget(renderer, nullptr);

		SDL_RenderPresent(renderer);
	}

	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyTexture(renderingFrame);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}*/