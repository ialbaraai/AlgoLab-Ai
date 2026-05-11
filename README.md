# AlgoLab AI

> A desktop application for visualizing and interacting with core Machine Learning algorithms — built from scratch in C++ using [SDL3](https://github.com/libsdl-org/SDL) and [Dear ImGui](https://github.com/ocornut/imgui).

---

## Overview

AlgoLab AI is a native desktop tool that brings three foundational ML algorithms to life through real-time, interactive visualization. Every algorithm is implemented from the ground up — no ML libraries, no shortcuts. Users can import datasets via CSV, interact with data points directly on the canvas, and watch the algorithms run step by step.

---

## Algorithms

### K-Means Clustering (KMC)
Groups data points into *k* clusters by iteratively updating centroids until convergence. Supports custom cluster count and interactive point placement.

### K-Nearest Neighbor (KNN)
Classifies data points based on the majority label among their *k* closest neighbors using Euclidean distance. Supports CSV import and manual point addition.

### Linear Regression (LR)
Fits a line to data using gradient descent with input normalization for numerical stability. Visualizes the regression line updating in real time.

---

## Features

- Real-time algorithm visualization on an interactive canvas
- CSV dataset import via native file dialog
- Manual data point placement and editing
- Animated main menu with particle system
- Per-algorithm icon and themed UI
- Fixed-timestep rendering loop at 60 FPS
- Built entirely from scratch — no ML or math libraries used

---

## Tech Stack

| Component | Technology |
|---|---|
| Language | C++20 |
| Windowing & Rendering | SDL3, SDL3\_image |
| GUI | Dear ImGui (SDL3 + SDLRenderer3 backend) |
| File Dialogs | [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) |
| CSV Parsing | [rapidcsv](https://github.com/d99kris/rapidcsv) (header-only) |
| Build System | CMake 3.20+ |

---

## Project Structure

```
AlgoLab-Ai/
├── application/
│   ├── src/
│   │   ├── algorithms/        # KMC, KNN, LinearRegression — core logic
│   │   ├── applications/      # Per-algorithm GUI and interaction layer
│   │   └── utilities/        # CSV parsing and shared utilities
│   ├── include/               # Header files
│   ├── main.cpp               # Entry point
│   └── CMakeLists.txt
├── assets/                    # Icons and fonts
├── libs/                      # Vendored: ImGui, tinyfiledialogs
└── testing/                   # Sample CSV datasets for each algorithm
```

---

## Building

### Prerequisites

- CMake 3.20 or higher
- A C++20-compatible compiler (GCC 11+, Clang 13+, MSVC 2022+)
- SDL3 and SDL3\_image installed on your system

### Linux (Fedora)

```bash
sudo dnf install SDL3-devel SDL3_image-devel cmake ninja-build
```

### Build Steps

```bash
git clone https://github.com/ialbaraai/AlgoLab-Ai.git
cd AlgoLab-Ai/application
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/AlgoLabAI
```

---

## Sample Datasets

Three ready-to-use CSV datasets are included in the `testing/` directory:

| File | Algorithm |
|---|---|
| `lr_dataset.csv` | Linear Regression |
| `knn_dataset.csv` | K-Nearest Neighbor |
| `kmc_dataset.csv` | K-Means Clustering |

---

## License

This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.

---

## Author

**Albaraa** — [@ialbaraai](https://github.com/ialbaraai)

> Built in under 5 days. All algorithms implemented from scratch.
