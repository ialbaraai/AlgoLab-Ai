#include "../../include/applications/kmcapp.h"
#include <iostream>
#include <string>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "imgui.h"
#include "../ImGui/backends/imgui_impl_sdl3.h"
#include "../ImGui/backends/imgui_impl_sdlrenderer3.h"
#include "../../include/utilities/csv.h"

KMCApp::KMCApp() {}

void KMCApp::run()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Failed to initialize SDL!\n";
        return;
    }

    std::vector<Point_T> points;

    std::vector<Centroid_T> centroids;

    SDL_Window* window = SDL_CreateWindow("AlgoLab AI | K-Means Clustering", 1050, 625, SDL_WINDOW_RESIZABLE);
    if (!window) { SDL_Quit(); std::cerr << "Failed to initialize window!\n"; return; }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) { SDL_DestroyWindow(window); SDL_Quit(); std::cerr << "Failed to initialize renderer!\n"; return; }

    SDL_Surface* icon = IMG_Load("kmc_icon.png");
    if (icon) { SDL_SetWindowIcon(window, icon); SDL_DestroySurface(icon); }

    SDL_Texture* renderingFrame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    if (!renderingFrame)
    {
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit();
        std::cerr << "Failed to initialize rendering frame!\n"; return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImFont* font = io.Fonts->AddFontFromFileTTF("assets/Roboto.ttf", 16.0f);

    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.FrameRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.WindowPadding = ImVec2(10.0f, 10.0f);
    style.ItemSpacing = ImVec2(8.0f, 6.0f);

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
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.08f, 0.30f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.10f, 0.45f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.15f, 0.60f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.40f, 0.90f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.75f, 0.55f, 1.00f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.05f, 0.20f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.15f, 0.60f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.38f, 0.22f, 0.70f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.75f, 0.55f, 1.00f, 1.0f);

    SelectType selectType = SelectType::None;
    int        selectIndex = -1;
    bool       showLines = true;
    bool       showNoise = true;
    int        maxIter = 1;
    bool       adddata = false;
    bool       addcsv = false;

    const ImVec4 COL_MUTED = ImVec4(0.47f, 0.47f, 0.47f, 1.0f);
    const ImVec4 COL_ACCENT = ImVec4(0.75f, 0.52f, 1.00f, 1.0f);
    const ImVec4 COL_WHITE = ImVec4(1.00f, 1.00f, 1.00f, 1.0f);
    const ImVec4 COL_DIMWHITE = ImVec4(0.80f, 0.80f, 0.80f, 1.0f);

    bool  is_running = true;
    SDL_Event event;
    int   windowX = 1920, windowY = 1080;
    float renderingFrameX = 800, renderingFrameY = 600;

    ImVec2 viewportImagePos = ImVec2(0, 0);
    ImVec2 viewportImageSize = ImVec2(800, 600);

    const float SIDEBAR_W = 250.0f;
    char const* supported_files[1] = { "*.csv" };
    std::string csvfilepath = "";
    CSV_T csvr("");

    while (is_running)
    {
        SDL_GetWindowSize(window, &windowX, &windowY);
        SDL_GetTextureSize(renderingFrame, &renderingFrameX, &renderingFrameY);

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) is_running = false;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                float mx = (float)event.button.x;
                float my = (float)event.button.y;

                bool inViewport = mx >= viewportImagePos.x && mx <= viewportImagePos.x + viewportImageSize.x &&
                    my >= viewportImagePos.y && my <= viewportImagePos.y + viewportImageSize.y;

                if (inViewport)
                {
                    float simX = ((mx - viewportImagePos.x) / viewportImageSize.x) * 1920.0f;
                    float simY = ((my - viewportImagePos.y) / viewportImageSize.y) * 1080.0f;

                    float      bestDist = 30.0f;
                    SelectType bestType = SelectType::None;
                    int        bestIdx = -1;

                    for (size_t i = 0; i < centroids.size(); ++i)
                    {
                        float dx = centroids[i].Position.X - simX;
                        float dy = centroids[i].Position.Y - simY;
                        float d = std::sqrt(dx * dx + dy * dy);
                        if (d < bestDist) { bestDist = d; bestType = SelectType::Centroid; bestIdx = (int)i; }
                    }

                    for (size_t i = 0; i < points.size(); ++i)
                    {
                        float dx = points[i].Position.X - simX;
                        float dy = points[i].Position.Y - simY;
                        float d = std::sqrt(dx * dx + dy * dy);
                        if (d < bestDist) { bestDist = d; bestType = SelectType::Point; bestIdx = (int)i; }
                    }
                    selectType = bestType;
                    selectIndex = bestIdx;
                }
            }
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::PushFont(font);

        float titleH = ImGui::GetTextLineHeight() + 8.0f;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)windowX, titleH));
        ImGui::Begin("##title", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar);
        float titleTextW = ImGui::CalcTextSize("AlgoLab Ai").x;
        ImGui::SetCursorPosX(((float)windowX - titleTextW) * 0.5f);
        ImGui::TextColored(COL_DIMWHITE, "AlgoLab Ai");
        ImGui::End();

        float btnH = ImGui::GetFrameHeight() + style.WindowPadding.y * 2.0f;
        float sidebarH = (float)windowY - titleH - btnH;

        ImGui::SetNextWindowPos(ImVec2(0, titleH));
        ImGui::SetNextWindowSize(ImVec2(SIDEBAR_W, sidebarH));
        ImGui::Begin("##sidebar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);

        if (ImGui::BeginTabBar("##maintabs"))
        {
            if (ImGui::BeginTabItem("Stats"))
            {
                ImGui::Spacing();
                ImGui::TextColored(COL_MUTED, "OVERVIEW");
                ImGui::Separator();
                ImGui::Spacing();

                int noiseCount = 0;
                for (auto& p : points) if (p.Cluster < 0) noiseCount++;
                int assigned = (int)points.size() - noiseCount;

                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.08f, 0.30f, 1.0f));

                auto StatCard = [&](const char* id, const char* label, int value)
                    {
                        ImGui::BeginChild(id, ImVec2((SIDEBAR_W - style.WindowPadding.x * 2 - style.ItemSpacing.x) * 0.5f, 50), ImGuiChildFlags_None);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                        ImGui::TextColored(COL_ACCENT, "%d", value);
                        ImGui::TextColored(COL_MUTED, "%s", label);
                        ImGui::EndChild();
                    };

                StatCard("##sc1", "Clusters", (int)centroids.size());
                ImGui::SameLine();
                StatCard("##sc2", "Points", (int)points.size());
                StatCard("##sc3", "Assigned", assigned);
                ImGui::SameLine();
                StatCard("##sc4", "Noise", noiseCount);

                ImGui::PopStyleColor();
                ImGui::PopStyleVar();

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::TextColored(COL_MUTED, "CLUSTERS");
                ImGui::Separator();
                ImGui::Spacing();

                std::vector<int> clusterCounts((int)centroids.size(), 0);
                for (auto& p : points)
                    if (p.Cluster >= 0 && p.Cluster < (int)centroids.size())
                        clusterCounts[p.Cluster]++;

                for (size_t i = 0; i < centroids.size(); ++i)
                {
                    auto& c = centroids[i];
                    ImVec2 cursor = ImGui::GetCursorScreenPos();
                    ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(cursor.x + 6.0f, cursor.y + 9.0f), 5.0f, IM_COL32(c.Color.Red, c.Color.Green, c.Color.Blue, 255));
                    ImGui::Dummy(ImVec2(14.0f, 0.0f));
                    ImGui::SameLine();
                    ImGui::TextColored(COL_DIMWHITE, c.Name.c_str());
                    ImGui::SameLine();
                    float countW = ImGui::CalcTextSize("000 pts").x;
                    ImGui::SetCursorPosX(SIDEBAR_W - style.WindowPadding.x - countW);
                    ImGui::TextColored(COL_MUTED, "%d pts", clusterCounts[i]);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Edit"))
            {
                ImGui::Spacing();
                ImGui::TextColored(COL_MUTED, "SELECTED");
                ImGui::Separator();
                ImGui::Spacing();

                if (selectType == SelectType::None)
                {
                    ImGui::TextColored(COL_MUTED, "Click a point or centroid\non the canvas to select it.");
                }
                else
                {
                    bool isCentroid = (selectType == SelectType::Centroid);
                    int  idx = selectIndex;

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.08f, 0.30f, 1.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
                    ImGui::BeginChild("##selinfo", ImVec2(0, isCentroid ? 68 : 84), ImGuiChildFlags_None);

                    if (isCentroid)
                    {
                        auto& c = centroids[idx];
                        ImGui::TextColored(COL_ACCENT, c.Name.c_str());
                        ImGui::TextColored(COL_MUTED, "X:"); ImGui::SameLine();
                        ImGui::TextColored(COL_WHITE, "%.1f", c.Position.X);
                        ImGui::SameLine(0, 16);
                        ImGui::TextColored(COL_MUTED, "Y:"); ImGui::SameLine();
                        ImGui::TextColored(COL_WHITE, "%.1f", c.Position.Y);
                        ImVec4 dc = ImVec4(c.Color.Red / 255.f, c.Color.Green / 255.f, c.Color.Blue / 255.f, 1.f);
                        ImGui::ColorButton("##previewcol", dc, ImGuiColorEditFlags_NoPicker, ImVec2(14, 14));
                        ImGui::SameLine();
                        ImGui::TextColored(COL_MUTED, "#%02X%02X%02X", c.Color.Red, c.Color.Green, c.Color.Blue);
                    }
                    else
                    {
                        auto& p = points[idx];
                        ImGui::TextColored(COL_ACCENT, p.Name.c_str());
                        ImGui::TextColored(COL_MUTED, "X:"); ImGui::SameLine();
                        ImGui::TextColored(COL_WHITE, "%.1f", p.Position.X);
                        ImGui::SameLine(0, 16);
                        ImGui::TextColored(COL_MUTED, "Y:"); ImGui::SameLine();
                        ImGui::TextColored(COL_WHITE, "%.1f", p.Position.Y);
                        ImGui::TextColored(COL_MUTED, "Cluster:"); ImGui::SameLine();
                        ImGui::TextColored(COL_WHITE, "%d", p.Cluster);
                        ImVec4 dc = ImVec4(p.Color.Red / 255.f, p.Color.Green / 255.f, p.Color.Blue / 255.f, 1.f);
                        ImGui::ColorButton("##previewcol2", dc, ImGuiColorEditFlags_NoPicker, ImVec2(14, 14));
                        ImGui::SameLine();
                        ImGui::TextColored(COL_MUTED, "#%02X%02X%02X", p.Color.Red, p.Color.Green, p.Color.Blue);
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();

                    ImGui::Spacing();
                    ImGui::TextColored(COL_MUTED, "POSITION");
                    ImGui::Separator();
                    ImGui::Spacing();

                    float& px = isCentroid ? centroids[idx].Position.X : points[idx].Position.X;
                    float& py = isCentroid ? centroids[idx].Position.Y : points[idx].Position.Y;
                    float prevX = px, prevY = py;

                    ImGui::TextColored(COL_MUTED, "X"); ImGui::SetNextItemWidth(-1);
                    ImGui::InputFloat("##editpx", &px, 0, 0, "%.2f");
                    ImGui::TextColored(COL_MUTED, "Y"); ImGui::SetNextItemWidth(-1);
                    ImGui::InputFloat("##editpy", &py, 0, 0, "%.2f");

                    if (px != prevX || py != prevY) { if (isCentroid) centroids[idx].rerect(); else points[idx].rerect(); }

                    ImGui::Spacing();
                    ImGui::TextColored(COL_MUTED, "COLOR");
                    ImGui::Separator();
                    ImGui::Spacing();

                    int& r = isCentroid ? centroids[idx].Color.Red : points[idx].Color.Red;
                    int& g = isCentroid ? centroids[idx].Color.Green : points[idx].Color.Green;
                    int& b = isCentroid ? centroids[idx].Color.Blue : points[idx].Color.Blue;
                    int& a = isCentroid ? centroids[idx].Color.Alpha : points[idx].Color.Alpha;

                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "R"); ImGui::SameLine(); ImGui::SetNextItemWidth(-1); ImGui::SliderInt("##er", &r, 0, 255);
                    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "G"); ImGui::SameLine(); ImGui::SetNextItemWidth(-1); ImGui::SliderInt("##eg", &g, 0, 255);
                    ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "B"); ImGui::SameLine(); ImGui::SetNextItemWidth(-1); ImGui::SliderInt("##eb", &b, 0, 255);
                    ImGui::TextColored(COL_MUTED, "A"); ImGui::SameLine(); ImGui::SetNextItemWidth(-1); ImGui::SliderInt("##ea", &a, 0, 255);

                    if (!isCentroid)
                    {
                        ImGui::Spacing();
                        ImGui::TextColored(COL_MUTED, "CLUSTER");
                        ImGui::Separator();
                        ImGui::Spacing();
                        ImGui::SetNextItemWidth(-1);
                        ImGui::DragInt("##ecluster", &points[idx].Cluster, 1.0f, -1, (int)centroids.size() - 1);
                    }
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Settings"))
            {
                ImGui::Spacing();
                ImGui::TextColored(COL_MUTED, "ALGORITHM");
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::TextColored(COL_MUTED, "K (clusters)");
                ImGui::BeginDisabled(true);
                int k = (int)centroids.size();
                ImGui::SetNextItemWidth(-1);
                ImGui::InputInt("##sk", &k);
                ImGui::EndDisabled();

                ImGui::Spacing();
                ImGui::TextColored(COL_MUTED, "Max iterations");
                ImGui::SetNextItemWidth(-1);
                ImGui::InputInt("##smaxiter", &maxIter);
                if (maxIter < 1) maxIter = 1;

                ImGui::Spacing();
                ImGui::TextColored(COL_MUTED, "DISPLAY");
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Checkbox("Show lines to centroid", &showLines);
                ImGui::Checkbox("Show noise points", &showNoise);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, (float)windowY - btnH));
        ImGui::SetNextWindowSize(ImVec2(SIDEBAR_W, btnH));
        ImGui::Begin("##buttons", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);
        float bw = (SIDEBAR_W - style.WindowPadding.x * 2.0f - style.ItemSpacing.x) * 0.5f;
        if (ImGui::Button("Add", ImVec2(bw, 0))) { adddata = !adddata; addcsv = false; }
        ImGui::SameLine();
        if (ImGui::Button("Run", ImVec2(bw, 0))) { for (int i = 0; i < maxIter; ++i) this->m_KMC.Simulate(points, centroids); }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowPos(ImVec2(SIDEBAR_W, titleH));
        ImGui::SetNextWindowSize(ImVec2((float)windowX - SIDEBAR_W, (float)windowY - titleH));
        ImGui::Begin("##viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::PopStyleVar();

        ImVec2 avail = ImGui::GetContentRegionAvail();
        constexpr float TARGET_ASPECT = 1920.0f / 1080.0f;
        int newX = (int)(avail.x - 10);
        int newY = (int)(newX / TARGET_ASPECT);
        if (newY > avail.y - 10) { newY = (int)(avail.y - 10); newX = (int)(newY * TARGET_ASPECT); }

        if (newX != (int)renderingFrameX || newY != (int)renderingFrameY)
        {
            if (renderingFrame) SDL_DestroyTexture(renderingFrame);
            renderingFrame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, newX, newY);
            renderingFrameX = (float)newX;
            renderingFrameY = (float)newY;
        }

        float imgOffX = (avail.x - renderingFrameX) * 0.5f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imgOffX);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
        viewportImagePos = ImGui::GetCursorScreenPos();
        viewportImageSize = ImVec2(renderingFrameX, renderingFrameY);
        ImGui::Image((ImTextureID)renderingFrame, viewportImageSize);
        ImGui::End();

        if (adddata)
        {
            ImGui::SetNextWindowSize(ImVec2(500, 420), ImGuiCond_Always);
            ImGui::Begin("Add Data", &adddata, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            if (ImGui::BeginTabBar("##addtabs"))
            {
                if (ImGui::BeginTabItem("Point"))
                {
                    static char nameBuf[128] = "";
                    static float x = 0.0f;
                    static float y = 0.0f;

                    static bool nameValid = true;
                    static bool xValid = true;
                    static bool yValid = true;

                    ImGui::TextColored(COL_MUTED, "POINT DATA");

                    ImGui::Text("Name:");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputText("##point_name", nameBuf, sizeof(nameBuf));

                    nameValid = strlen(nameBuf) > 0;
                    if (!nameValid)
                        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Name cannot be empty.");

                    ImGui::Text("X Coordinate:");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputFloat("##point_x", &x);

                    xValid = (x >= 0.0f && x <= 1920.0f);
                    if (!xValid)
                        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "X must be between 0 and 1920.");

                    ImGui::Text("Y Coordinate:");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputFloat("##point_y", &y);

                    yValid = (y >= 0.0f && y <= 1080.0f);
                    if (!yValid)
                        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Y must be between 0 and 1080.");

                    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);
                    ImGui::Separator();

                    float btn1 = 100.0f;
                    float btn2 = 140.0f;
                    float btn3 = 180.0f;

                    float spacing = ImGui::GetStyle().ItemSpacing.x;

                    float totalWidth = btn1 + spacing + btn2 + spacing + btn3;

                    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

                    if (ImGui::Button("Cancel", ImVec2(btn1, 0)))
                        adddata = false;

                    ImGui::SameLine();

                    bool canAdd = nameValid && xValid && yValid;
                    if (!canAdd) ImGui::BeginDisabled();

                    ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
                    if (ImGui::Button("Add Point", ImVec2(btn2, 0)))
                    {
                        points.push_back(Point_T(std::string(nameBuf), x, y, { 35,35,35,255 }));

                        strcpy(nameBuf, "");
                        x = 0.0f;
                        y = 0.0f;
                    }
                    ImGui::PopStyleColor();

                    if (!canAdd) ImGui::EndDisabled();

                    ImGui::SameLine();

                    if (ImGui::Button("Implement from CSV", ImVec2(btn3, 0)))
                    {
                        adddata = false;
                        addcsv = true;
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Centroid"))
                {
                    static char nameBuf[128] = "";
                    static float x = 0.0f;
                    static float y = 0.0f;

                    static bool nameValid = true;
                    static bool xValid = true;
                    static bool yValid = true;

                    ImGui::TextColored(COL_MUTED, "CENTROID DATA");

                    ImGui::Text("Name:");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputText("##centroid_name", nameBuf, sizeof(nameBuf));

                    nameValid = strlen(nameBuf) > 0;
                    if (!nameValid)
                        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Name cannot be empty.");

                    ImGui::Text("X Coordinate:");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputFloat("##centroid_x", &x);

                    xValid = (x >= 0.0f && x <= 1920.0f);
                    if (!xValid)
                        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "X must be between 0 and 1920.");

                    ImGui::Text("Y Coordinate:");
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputFloat("##centroid_y", &y);

                    yValid = (y >= 0.0f && y <= 1080.0f);
                    if (!yValid)
                        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Y must be between 0 and 1080.");

                    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);
                    ImGui::Separator();

                    float btn1 = 100.0f;
                    float btn2 = 140.0f;
                    float btn3 = 180.0f;

                    float spacing = ImGui::GetStyle().ItemSpacing.x;

                    float totalWidth = btn1 + spacing + btn2 + spacing + btn3;

                    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

                    if (ImGui::Button("Cancel", ImVec2(btn1, 0)))
                        adddata = false;

                    ImGui::SameLine();

                    bool canAdd = nameValid && xValid && yValid;
                    if (!canAdd) ImGui::BeginDisabled();

                    ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
                    if (ImGui::Button("Add Centroid", ImVec2(btn2, 0)))
                    {
                        centroids.push_back(Centroid_T(std::string(nameBuf), x, y, { 25,25,25,255 }));

                        strcpy(nameBuf, "");
                        x = 0.0f;
                        y = 0.0f;
                    }
                    ImGui::PopStyleColor();

                    if (!canAdd) ImGui::EndDisabled();

                    ImGui::SameLine();

                    if (ImGui::Button("Implement from CSV", ImVec2(btn3, 0)))
                    {
                        adddata = false;
                        addcsv = true;
                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

        if (addcsv)
        {
            static bool xValid = true;
            static bool yValid = true;
            ImGui::SetNextWindowSize(ImVec2(500, 420), ImGuiCond_Always);
            ImGui::Begin("Add from CSV", &addcsv, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
            ImGui::TextColored(COL_MUTED, "SOURCE FILE");
            char buf[256];
            strcpy(buf, csvfilepath.empty() ? "No file selected..." : csvfilepath.c_str());
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##filepath", buf, sizeof(buf), ImGuiInputTextFlags_ReadOnly);

            if (ImGui::Button("Choose file", ImVec2(-1, 30)))
            {
                const char* returnedfile = tinyfd_openFileDialog("Choose a CSV file", "", 1, supported_files, "(*.csv)", 0);
                if (returnedfile) { csvfilepath = returnedfile; csvr = CSV_T(csvfilepath.c_str()); csvr.read(); }
            }
            ImGui::Separator();
            if (csvr.isread())
            {
                std::vector<std::string> headers = csvr.header();
                auto const& dataMap = csvr.map();
                bool hasData = !dataMap.empty();

                if (!csvfilepath.empty() && !headers.empty())
                {
                    ImGui::TextColored(COL_MUTED, "COORDINATE MAPPING");
                    static int selectedLabelIdx = 0, selectedXIdx = 0, selectedYIdx = 0;

                    auto RenderHeaderCombo = [&](const char* label, const char* comboId, int& currentIdx) {
                        ImGui::Text("%s", label); ImGui::SetNextItemWidth(-1);
                        if (ImGui::BeginCombo(comboId, headers[currentIdx].c_str()))
                        {
                            for (int i = 0; i < (int)headers.size(); i++)
                                if (ImGui::Selectable(headers[i].c_str(), currentIdx == i)) currentIdx = i;
                            ImGui::EndCombo();
                        }
                        };

                    RenderHeaderCombo("Point Label (Name):", "##labelCombo", selectedLabelIdx);
                    RenderHeaderCombo("X Axis (Numerical):", "##xCombo", selectedXIdx);
                    if (hasData) { xValid = const_cast<CSVMap_T&>(dataMap[0]).convert<float>(selectedXIdx).success(); if (!xValid) ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "X Column is non-numeric."); }
                    RenderHeaderCombo("Y Axis (Numerical):", "##yCombo", selectedYIdx);
                    if (hasData) { yValid = const_cast<CSVMap_T&>(dataMap[0]).convert<float>(selectedYIdx).success(); if (!yValid) ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Y Column is non-numeric."); }

                    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);
                    ImGui::Separator();
                    if (ImGui::Button("Cancel", ImVec2(100, 0))) { addcsv = false; }
                    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
                    ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
                    if (ImGui::Button("Generate Points", ImVec2(140, 0)))
                    {
                        for (CSVMap_T& curdata : csvr.map()) points.push_back(Point_T(curdata.m_Row[selectedLabelIdx], curdata.convert<float>(selectedXIdx).value(), curdata.convert<float>(selectedYIdx).value(), { 35, 35, 35, 255 }));
                        addcsv = false;
                    }
                    ImGui::PopStyleColor();
                }
                else if (!csvfilepath.empty()) ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Error: No headers found.");
            }
            ImGui::End();
        }

        ImGui::PopFont();
        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_SetRenderTarget(renderer, renderingFrame);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        for (auto& c : centroids) c.render(renderer, renderingFrameX, renderingFrameY);
        for (auto& p : points)
        {
            if (!showNoise && p.Cluster < 0) continue;
            p.render(renderer, renderingFrameX, renderingFrameY);
            if (showLines && p.Cluster >= 0 && p.Cluster < (int)centroids.size())
            {
                p.Color = centroids[p.Cluster].Color;
                SDL_SetRenderDrawColor(renderer, p.Color.Red, p.Color.Green, p.Color.Blue, p.Color.Alpha);
                p.renderline(centroids[p.Cluster], renderer, renderingFrameX, renderingFrameY);
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
}