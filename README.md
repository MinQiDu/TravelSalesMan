# Implement AntColonyOptimization to solve TravelSalesMan Problem

## 1. Introduction
- Using C++
- 使用 ACO 解 TSP
- 支援 iteration times, evaporation rate, pheromones weight and heuristic weight 調整

## 2. Main Function
- void RunALG(const int &iter, 
	const double &eva_rate, 
	const int &weight_pher, 
	const int &weight_heu)         
    / *implement ACO* /

- void Init()   
/ *1.create city_coordinates from .txt    
2.intitialize heuristic and pheromones record* /

- vector<int> Path_construct(int start_city)                
/ *construct a path visiting all cities* /

- double Cal_dis(const vector<int> &visited_order)                
/ *calculate total distance of this path* /

- double Cal_edge(const int& from, const int& to)                
/ *calculate total distance og this edge* /

- vector<int> SelectTopIdx(const vector<double>& recent_dis_record, const int& top_n)                
/ *Select & return top n shortest dis idx* /

- void Apply_2_Opt(const int &top_n)                
/ *1.Apply local search on top n ants    
2.update shortest_path and shortes_dis* /

- void Update_pheromones()                
/ *update pheromones level for next iter* /

- vector<pair<int, int>> ReadCityCoord(const string& filename)                
/ *transfer city_coord.txt -> city_coord vector {{x1, y1}, {x2, y2}, ...}* /

## 3. Input
### command_line argument :
- 迭代次數 | iter = 1000
- 動態蒸發率最高值 | eva_rate = 0.6
- 費洛蒙重要程度 | weight_pher = 1
- 啟發資訊重要程度 | weight_heu = 3

## 4. Output
- path_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.txt | 最佳路線座標紀錄
- dis_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.txt | 每代最佳距離紀錄
- plot_ACO_path.plt | 繪製最佳路線圖
- plot_ACO_dis.plt | 繪製最佳距離收斂圖
- path_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.png | 最佳路線圖
- dis_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.png | 最佳距離收斂圖

## 5. 執行方式
### `------------編譯------------`
#### `VisualStudio`
1. 開啟 Visual Studio 專案 `TSP.sln`
2. 按下 `Ctrl + F5` 開始編譯

#### `VSCode`
1. 在 Windows cmd 或 PowerShell 中
2. 先到正確資料夾路徑
3. 輸入 :

g++ main.cpp TSP.cpp ACO.cpp -o TSP.exe


### `------------執行------------`

1. 在 Windows cmd 或 PowerShell 中
2. 先到正確資料夾路徑
3. 將 ACO_HW_point.txt 放入執行檔所在資料夾
4. 輸入 :   
.\TSP.exe iter eva_rate weight_pher weight_heu 
eg. 
.\TSP.exe 1000 0.7 1 3

### `------------出圖------------`

1. 安裝gnuplot
2. 在 Windows cmd 或 PowerShell 中
3. 先到正確資料夾路徑
4. 輸入 : 

gnuplot plot_ACO_path.plt   
即可在資料夾中找到 
path_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.png

gnuplot plot_ACO_dis.plt   
即可在資料夾中找到
dis_result_TSP_ACO_total1000iter_evaporate0_pher1_heu3.png

## 6. 檔案結構
- TSP/   
`main.cpp`   
`TSP.cpp` / `TSP.h`      
`ACO.cpp` / `ACO.h`   