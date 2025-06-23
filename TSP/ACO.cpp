#include "TSP.h"
#include "ACO.h"
#include <fstream> /*file stream*/
#include <sstream> /*string stream*/
#include <iostream>

void algo_ACO::RunALG(const int& _iter,
	const double& _eva_rate,
	const int& _weight_pher,
	const int& _weight_dis)
{
	srand(time(NULL));
	iter = _iter;
	eva_rate = _eva_rate;
	weight_pher = _weight_pher;
	weight_dis = _weight_dis;
	Init();

	while (iter_count < iter) /*在世代iter次數尚未達標前*/
	{
		/*每一世代重新設定*/
		recent_dis_record = vector<double>(ants, 0.0);
		recent_path_record = vector<vector<int>>(ants, vector<int>(city_num, -1));
		next_pheromones = vector<vector<double>>(city_num, vector<double>(city_num, 0.0));
		Cal_possibility(weight_pher, weight_dis); /*每一世代重新計算一次possibility*/
		
		for (int k = 0; k < ants; ++k) /*each ants do*/
		{
			int rd_start = rand() % city_num;
			recent_path_record[k].push_back(rd_start);
			Path_construct(rd_start);

			for (int i = 0; i < city_num - 1; ++i) /*add ants k 於有經過的Edge(from, to)上留下的費洛蒙*/
			{
				int from = recent_path_record[k][i];
				int to = recent_path_record[k][i + 1];
				next_pheromones[from][to] += 50 / recent_dis_record[k];
			}
		}
		/*選最短dis的五隻ants做local search*/
		Apply_2_Opt();
		Update_pheromones();
	}
}

/* 1.create city_coordinates from .txt 
   2.intitialize heuristic and pheromones record */
void algo_ACO::Init()
{
	city_coordinates = ReadCityCoord("ACO_HW_point.txt"); /*取ACO_HW_point.txt資料存入city_coordinates*/
	city_num = city_coordinates.size();
	ants = 30;
	
	/*Initialization for pheromones = 1.0 and heuristic record = 0.0*/
	pheromones_record = vector<vector<double>>(city_num, vector<double>(city_num, 1.0));
	heuristic_record = vector<vector<double>>(city_num, vector<double>(city_num, 0.0));

	for (int i = 0; i < city_num; ++i)
	{
		for (int j = 0; j < city_num; ++j)
		{
			if (i != j) /*if i != j then calculate dist between ij and set heuristic[i][j] = 1.0 / dist_ij*/
			{
				TSP tsp;
				double dist_ij = tsp.TravelSalesManProblem({ i, j }, city_coordinates);
				heuristic_record[i][j] = 1.0 / dist_ij;
			}
		}
	}
	cout << "城市數量: " << city_coordinates.size() << ", 螞蟻數量: " << ants << endl;
}

vector<double> algo_ACO::Cal_possibility(const double& a, const double& b)
{
	for (int i = 0; i < city_num; ++i)
	{
		for (int j = 0; j < city_num; ++j)
		{

		}
	}
}

vector<int> algo_ACO::Path_construct(int start_city)
{
	vector<int> allowed_k;
	for (int i = 0; i < city_num; ++i)
	{
		if (i != start_city) { allowed_k.push_back(i); }
	}

	int next_city;

	for (int j = 0; j < city_num; ++j) /*until all cities be visited*/
	{
		/*calculate edge_possibility*/
		Cal_possibility(weight_pher, weight_dis);
		/*implement wheel roulete to choose next city*/
		/*update allowed_k*/
		allowed_k.erase(remove(allowed_k.begin(), allowed_k.end(), next_city), allowed_k.end());
		/*update start city for next run*/
		start_city = next_city;
	}
}

double algo_ACO::Cal_dis(const vector<int>& visited_order)
{

}

void algo_ACO::Apply_2_Opt()
{

}

/*update pheromones level of each edge(i, j)*/
void algo_ACO::Update_pheromones()
{
	for (int i = 0; i < city_num; ++i)
	{
		for (int j = 0; j < city_num; ++j)
		{
			pheromones_record[i][j] = (1 - eva_rate) * pheromones_record[i][j] + next_pheromones[i][j];
		}
	}
}

/*用 ifstream 來開啟檔案。如果失敗，就輸出錯誤訊息 cerr 並回傳空的 coordinates*/
vector<pair<int, int>> algo_ACO::ReadCityCoord(const string& filename)
{
	city_coordinates.clear();
	ifstream file(filename);
	if (!file.is_open())
	{
		cerr << "Failed to open file : " << filename << "請確認檔案是否與執行檔在同一資料夾。" << endl;
		return city_coordinates; /*empty vector*/
	}

	string line;
	while (getline(file, line)) /*用 getline() 一次讀進一整行存入line*/
	{
		istringstream iss(line);/*把 line 用 istringstream 轉成「可以一個一個讀數字的物件」*/
		int id, x, y;           /*讀入三個數字：id, x, y*/
		if (iss >> id >> x >> y)
		{
			city_coordinates.push_back({ x, y });
		}
	}
	file.close();
	return city_coordinates;
}