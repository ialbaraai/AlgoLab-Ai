#include "../../include/applications/algolab.h"

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <cstdlib>

#include "imgui.h"
#include "../ImGui/backends/imgui_impl_sdl3.h"
#include "../ImGui/backends/imgui_impl_sdlrenderer3.h"

#include "../../include/applications/kmcapp.h"
#include "../../include/applications/knnapp.h"
#include "../../include/applications/linearregressionapp.h"

struct Particle {
	float x, y, vx, vy, radius;
};

int AlgoLab::run()
{
	do
	{
		std::vector<Particle> particles;

		srand((unsigned)time(nullptr));

		for (int i = 0; i < 60; i++) {
			particles.push_back({
				(float)(rand() % 500),
				(float)(rand() % 500),
				((rand() % 100) - 50) / 400.0f,
				((rand() % 100) - 50) / 400.0f,
				(float)(2 + rand() % 3)
				});
		}
		Uint64 lastTick = SDL_GetTicks();

		this->selected_algorithm = -1;

		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			std::cerr << "Error while initializing SDL: " << SDL_GetError() << '\n';

			return 1;
		}

		SDL_Window* window = SDL_CreateWindow("AlgoLab Ai", 500, 500, 0);

		if (!window)
		{
			SDL_Quit();

			std::cerr << "Error while creating window: " << SDL_GetError() << '\n';

			return 1;
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

		if (!renderer)
		{
			SDL_DestroyWindow(window);
			SDL_Quit();

			std::cerr << "Error while creating renderer: " << SDL_GetError() << '\n';

			return 1;
		}

		SDL_Surface* icon = IMG_Load("assets/algolab_icon.png");
		SDL_Texture* icontexture = nullptr;

		if (icon)
		{
			icontexture = SDL_CreateTextureFromSurface(renderer, icon);
			SDL_SetWindowIcon(window, icon);

			SDL_DestroySurface(icon);
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer3_Init(renderer);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowBorderSize = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.FrameBorderSize = 0.0f;

		style.Colors[ImGuiCol_Button] = ImVec4(0.30f, 0.15f, 0.60f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.22f, 0.70f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.10f, 0.50f, 1.0f);

		ImFont* robotoLarge = io.Fonts->AddFontFromFileTTF("assets/Roboto.ttf", 28.0f);
		ImFont* roboto = io.Fonts->AddFontFromFileTTF("assets/Roboto.ttf", 16.0f);

		bool is_running = true;
		SDL_Event event;

		const char* algorithms[] = { "KMC | K-Means Clustering", "KNN | K-Nearest Neighbor", "LR | Linear Regression"};
		int selection = -1;

		while (is_running)
		{
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL3_ProcessEvent(&event);

				if (event.type == SDL_EVENT_QUIT) is_running = false;
			}

			ImGui_ImplSDLRenderer3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(500, 500));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);

			ImGui::PushFont(robotoLarge);

			float total_height =
				125.0f + 10.0f +
				ImGui::CalcTextSize("AlgoLab Ai").y + 10.0f +
				ImGui::GetFrameHeightWithSpacing() * 2 +
				ImGui::CalcTextSize("LAUNCH").y + 5.0f;

			ImGui::SetCursorPosY((ImGui::GetWindowSize().y - total_height) * 0.5f);

			ImGui::BeginGroup();

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 125.0f) * 0.5f);
			if (icontexture)
				ImGui::Image((ImTextureID)icontexture, ImVec2(125, 125));

			ImGui::Dummy(ImVec2(0, 5));

			float text_width = ImGui::CalcTextSize("AlgoLab Ai").x;
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - text_width) * 0.5f);
			ImGui::Text("AlgoLab Ai");

			ImGui::PopFont();
			ImGui::PushFont(roboto);

			ImGui::Dummy(ImVec2(0, 5));

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300.0f) * 0.5f);
			ImGui::SetNextItemWidth(300.0f);
			ImGui::Combo("##algorithms", &selection, algorithms, 3);

			ImGui::Dummy(ImVec2(0, 5));

			if (selection < 0) ImGui::BeginDisabled();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300.0f) * 0.5f);
			if (ImGui::Button("LAUNCH", ImVec2(300.0f, ImGui::CalcTextSize("LAUNCH").y + 5.0f)))
			{
				if (selection >= 0)
				{
					this->selected_algorithm = selection;
					is_running = false;
				}
			}
			ImGui::PopStyleVar();
			if (selection < 0) ImGui::EndDisabled();

			ImGui::EndGroup();
			ImGui::PopFont();

			ImGui::End();
			ImGui::PopStyleColor();

			ImGui::Render();

			Uint64 now = SDL_GetTicks();
			float dt = (now - lastTick) / 1000.0f;
			lastTick = now;

			for (auto& p : particles) {
				p.x += p.vx * dt * 60.0f;
				p.y += p.vy * dt * 60.0f;
				if (p.x < 0) p.x += 500.0f;
				if (p.x > 500) p.x -= 500.0f;
				if (p.y < 0) p.y += 500.0f;
				if (p.y > 500) p.y -= 500.0f;
			}

			SDL_SetRenderDrawColor(renderer, 15, 23, 42, 255);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			for (size_t i = 0; i < particles.size(); i++)
			{
				for (size_t j = i + 1; j < particles.size(); j++)
				{
					float dx = particles[i].x - particles[j].x;
					float dy = particles[i].y - particles[j].y;

					float dist = std::sqrt(dx * dx + dy * dy);

					if (dist < 80.0f)
					{
						Uint8 alpha = (Uint8)((1.0f - dist / 80.0f) * 60.0f);
						SDL_SetRenderDrawColor(renderer, 140, 100, 220, alpha);
						SDL_RenderLine(renderer, particles[i].x, particles[i].y,
							particles[j].x, particles[j].y);
					}
				}
			}

			for (auto& p : particles) {
				SDL_SetRenderDrawColor(renderer, 120, 80, 200, 90);

				for (float dy = -p.radius; dy <= p.radius; dy += 1.0f)
				{
					float halfW = std::sqrt(p.radius * p.radius - dy * dy);

					SDL_RenderLine(renderer,
						(int)(p.x - halfW), (int)(p.y + dy),
						(int)(p.x + halfW), (int)(p.y + dy));
				}
			}

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

			if (ImGui::GetDrawData())
				ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

			SDL_RenderPresent(renderer);
		}

		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		if (icontexture != nullptr)
			SDL_DestroyTexture(icontexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		if (this->selected_algorithm == 0)
		{
			KMCApp kmcapp;
			kmcapp.run();
		}
		else if (this->selected_algorithm == 1)
		{
			KNNApp knnapp;
			knnapp.run();
		}
		else if (this->selected_algorithm == 2)
		{
			LinearRegressionApp lrapp;
			lrapp.run();
		}

	} while (this->selected_algorithm >= 0);

	return 0;
}