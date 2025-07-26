# Implement AntColonyOptimization to Solve the Traveling Salesman Problem

## ( I ) Introduction

- **Programming language:** C++
- **Metaheuristic algorithm:** Ant Colony Optimization (ACO)
- **Benchmark problem:** Traveling Salesman Problem (TSP)
- **Supports configurable parameters:** iteration times, evaporation rate, pheromone importance, heuristic importance

## ( II ) Main Functionality

- `void RunALG(const int &iter, const double &eva_rate, const int &weight_pher, const int &weight_heu)`\
  *Runs the ACO algorithm for **`iter`** iterations with specified parameters*

- `void Init()`\
  *1. Reads **`city_coordinates`** from .txt file* *2. Initializes heuristic values and pheromone matrix*

- `vector<int> Path_construct(int start_city)`\
  *Constructs a tour starting from **`start_city`** visiting all cities*

- `double Cal_dis(const vector<int> &visited_order)`\
  *Calculates total tour distance of the visited city order*

- `double Cal_edge(const int &from, const int &to)`\
  *Calculates distance between two cities (from → to)*

- `vector<int> SelectTopIdx(const vector<double> &recent_dis_record, const int &top_n)`\
  *Selects and returns indices of top **`n`** shortest tours*

- `void Apply_2_Opt(const int &top_n)`\
  *Applies 2-opt local search to top **`n`** tours and updates global best*

- `void Update_pheromones()`\
  *Updates pheromone levels based on ant tours*

- `vector<pair<int, int>> ReadCityCoord(const string &filename)`\
  *Reads city coordinates from file and returns vector of pairs*

## ( III ) Input

### Command-line arguments:

- **Number of Iterations:** `iter = (e.g., 1000)`
- **Pheromone Evaporation Rate:** `eva_rate = (e.g., 0.7)`
- **Pheromone Importance:** `weight_pher = (e.g., 1)`
- **Heuristic Importance:** `weight_heu = (e.g., 3)`

## ( IV ) Output

- `path_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.txt`: Coordinates of the best path
- `dis_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.txt`: Best distance per iteration
- `plot_ACO_path.plt`: Gnuplot script for visualizing path
- `plot_ACO_dis.plt`: Gnuplot script for plotting convergence
- `path_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.png`: Visualized best path
- `dis_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.png`: Convergence plot of best distances

## ( V ) How to Compile, Run, and Visualize Results

### Compile

#### Visual Studio

1. Open the solution file `TSP.sln`
2. Press `Ctrl + F5` to build

#### VSCode

1. Open CMD or PowerShell
2. Navigate to the working directory
3. Run:

```bash
g++ main.cpp TSP.cpp ACO.cpp -o TSP.exe
```

### Run

1. Place `ACO_HW_point.txt` in the executable directory
2. Open CMD or PowerShell
3. Run:

```bash
.\TSP.exe iter eva_rate weight_pher weight_heu
```

Example:

```bash
.\TSP.exe 1000 0.7 1 3
```

### Plot Results

1. Install gnuplot
2. Open CMD or PowerShell
3. Run:

```bash
gnuplot plot_ACO_path.plt
gnuplot plot_ACO_dis.plt
```

Result PNGs will be saved in the same directory

## ( VI ) File Structure

```
TSP/
|
├── main.cpp
├── TSP.cpp / TSP.h
├── ACO.cpp / ACO.h
├── ACO_HW_point.txt
├── plot_ACO_path.plt
├── plot_ACO_dis.plt
│
├── results/  ← output files (.txt, .png)
└── README.md ← this file
```

## ( VII ) Experimental Results

### Distance Convergence Plot
<img src="results/(428.872) dis_result_TSP_ACO_total1000iter_evaporate7_pher1_heu3.png" width="80%"/>

### Final Path
<img src="results/(428.872) path_result_TSP_ACO_total1000iter_evaporate7_pher1_heu3.png" width="80%"/>

## ( VIII ) Observations

- ACO adapts well to combinatorial optimization problems like TSP.
- The `evaporation rate` controls exploration vs exploitation balance.
- 2-opt local search enhances convergence of elite ants.
- `heuristic weight` heavily influences solution diversity.

## ( IX ) Key Features

- Configurable metaheuristic parameters (iterations, evaporation, weights)
- Efficient local search integration via 2-opt
- City coordinate parsing from file
- Gnuplot-compatible outputs for result visualization

## ( X ) Skills Demonstrated

- Metaheuristic algorithm implementation for combinatorial problems
- Integration of global search (ACO) with local optimization (2-opt)
- Modular programming and data abstraction in C++
- Visualization with gnuplot
- File I/O and structured performance logging

