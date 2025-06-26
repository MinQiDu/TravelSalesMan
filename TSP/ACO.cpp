#include "TSP.h"
#include "ACO.h"
#include <fstream>   /*file stream*/
#include <sstream>   /*string stream*/
#include <iostream>
#include <cmath>
#include <algorithm> /*for sort*/

void algo_ACO::RunALG(const int& _iter,
	const double& _eva_rate,
	const int& _weight_pher,
	const int& _weight_heu)
{
	srand(time(NULL));
	iter = _iter;
	iter_count = 0;
	eva_rate = _eva_rate;
	weight_pher = _weight_pher;
	weight_heu = _weight_heu;
	Init();

	/*Initializeation for global_shortest_path and global_shortest_dis*/
	vector<int> init_path(city_num);
	for (int i = 0; i < city_num; ++i)
	{
		init_path[i] = i;
	}
	global_shortest_path = init_path;
	global_shortest_dis = Cal_dis(init_path);

	/*for record output*/
	vector<double> all_iter_shortest_dis;

	while (iter_count < iter) /*在世代iter次數尚未達標前*/
	{
		/*每一世代重新設定初始狀態*/
		/*dynamic eva_rate*/
		eva_rate = eva_rate - (eva_rate * 0.85) * ((double)iter_count / iter);
		recent_dis_record = vector<double>(ants, 0.0);
		recent_path_record = vector<vector<int>>(ants, vector<int>(city_num, -1));

		for (int k = 0; k < ants; ++k) /*each ants do*/
		{
			int rd_start = rand() % city_num;
			vector<int> path_k = Path_construct(rd_start);
			recent_path_record[k] = path_k;
			recent_dis_record[k] = Cal_dis(recent_path_record[k]);
		}

		/*after all ants finished their path，update shortest dis and path*/
		if (*min_element(recent_dis_record.begin(), recent_dis_record.end()) < shortest_dis)
		{
			int shortest_id = min_element(recent_dis_record.begin(), recent_dis_record.end()) - recent_dis_record.begin();
			shortest_dis = recent_dis_record[shortest_id];
			shortest_path = recent_path_record[shortest_id];
		}

		/*從recent_dis選最短dis前25%ants做local search*/
		int top_n = ants / 4;
		Apply_2_Opt(top_n); /*update shortest_dis / path & recent_dis / path record */
		
		iter_count++;
		
		cout << "[iter " << iter_count << "] shortest_dis: " << shortest_dis << endl;
		
		Update_pheromones();
		
		/*push in for record output*/
		all_iter_shortest_dis.push_back(shortest_dis);
	}

	/*create dis_result.txt*/
	ofstream file("dis_result_TSP_ACO_total" + to_string(iter) + "iter_evaporate" + to_string(static_cast<int>(eva_rate * 10)) + "_pher" + to_string(weight_pher) + "_heu" + to_string(weight_heu) + ".txt");
	for (int i = 0; i < iter; ++i)
	{
		file << i + 1 << " " << all_iter_shortest_dis[i] << "\n";
	}
	file.close();

	/*create .plt for generating plot*/
	ofstream plot("plot_ACO_dis.plt");
	plot << "set terminal png size 800, 600\n";
	plot << "set output 'dis_result_TSP_ACO_total" << iter << "iter_evaporate" << static_cast<int>(eva_rate * 10) << "_pher" << weight_pher << "_heu" << weight_heu << ".png'\n";
	plot << "set title 'Convergence with ACO on TravelSalesMan'\n";
	plot << "set xlabel 'Iteration Times'\n";
	plot << "set ylabel 'Shortest Distance'\n";
	plot << "set xrange [0:" << iter << "]\n";
	plot << "set yrange [0:600]\n";
	plot << "plot 'dis_result_TSP_ACO_total" << iter << "iter_evaporate" << static_cast<int>(eva_rate * 10) << "_pher" << weight_pher << "_heu" << weight_heu << ".txt' using 1:2 with lines title 'with ACO'";
	plot.close();

	/*create path_result.txt*/
	ofstream file_path("path_result_TSP_ACO_total" + to_string(iter) + "iter_evaporate" + to_string(static_cast<int>(eva_rate*10)) + "_pher" + to_string(weight_pher) + "_heu" + to_string(weight_heu) + ".txt");
	for (int city : shortest_path)
	{
		int x = city_coordinates[city].first;
		int y = city_coordinates[city].second;
		file_path << x << " " << y << "\n";
	}
	int x0 = city_coordinates[shortest_path[0]].first;
	int y0 = city_coordinates[shortest_path[0]].second;
	file_path << x0 << " " << y0 << "\n";
	file_path.close();

	/*create .plt for generating plot*/
	ofstream plot_path("plot_ACO_path.plt");
	plot_path << "set terminal png size 800, 600\n";
	plot_path << "set output 'path_result_TSP_ACO_total" << iter << "iter_evaporate" << static_cast<int>(eva_rate * 10) << "_pher" << weight_pher << "_heu" << weight_heu << ".png'\n";
	plot_path << "set title 'TravelSalesMan Shortest Path with ACO'\n";
	plot_path << "set label 'Distance = " << shortest_dis << "' at graph 0.35, 0.95 tc rgb 'black' front\n";
	plot_path << "set size square\n";
	plot_path << "set xrange [0:70]\n";
	plot_path << "set yrange [0:80]\n";
	plot_path << "set key off\n";
	plot_path << "set grid\n";
	plot_path << "plot 'path_result_TSP_ACO_total" << iter << "iter_evaporate" << static_cast<int>(eva_rate * 10) << "_pher" << weight_pher << "_heu" << weight_heu << ".txt' with linespoints lt rgb 'purple' pt 7 ps 1.2\n";
	plot_path.close();
}

/* 1.create city_coordinates from .txt 
   2.intitialize heuristic and pheromones record */
void algo_ACO::Init()
{
	city_coordinates = ReadCityCoord("ACO_HW_point.txt"); /*取ACO_HW_point.txt資料存入city_coordinates*/
	city_num = city_coordinates.size();
	ants = 60; /*螞蟻數量*/
	Q = 100;   /*用來計算pheromones level = Q / dis*/
	
	/*Initialization for pheromones = 1.0 and heuristic record = 0.0*/
	pheromones_record = vector<vector<double>>(city_num, vector<double>(city_num, 1.0));
	heuristic_record = vector<vector<double>>(city_num, vector<double>(city_num, 0.0));

	/*Initializeation for shortest_path and shortest_dis*/
	vector<int> init_path(city_num);
	for (int c = 0; c < city_num; ++c)
	{
		init_path[c] = c;
	}
	shortest_path = init_path;
	shortest_dis = Cal_dis(init_path);

	/*set heuristic_record*/
	for (int i = 0; i < city_num; ++i)
	{
		for (int j = 0; j < city_num; ++j)
		{
			if (i != j) /*if i != j then calculate dist between ij and set heuristic[i][j] = 1.0 / dist_ij*/
			{
				double dist_ij = Cal_edge(i, j);
				heuristic_record[i][j] = 1.0 / dist_ij;
			}
		}
	}
	cout << "city_num: " << city_coordinates.size() << " | ants_num: " << ants << " | eva_rate: " << eva_rate << " | weight_pher: " << weight_pher << " | weight_heu: " << weight_heu << endl;
}

vector<int> algo_ACO::Path_construct(int start_city)
{
	vector<int> path_k;
	path_k.push_back(start_city);

	vector<int> allowed_k; 
	for (int i = 0; i < city_num; ++i) /*create allowed_k shows cities haven't been visited yet*/
	{
		if (i != start_city) { allowed_k.push_back(i); }
	}

	int next_city;

	while (path_k.size() < city_num) /*until all cities be visited*/
	{
		/*calculate edge_possibility*/
		vector<double> edge_possibility;
		double all = 0.0;
		
		for (int city : allowed_k)
		{
			all += pow(pheromones_record[start_city][city], weight_pher) * pow(heuristic_record[start_city][city], weight_heu);
		}

		for (int city : allowed_k)
		{
			double each = pow(pheromones_record[start_city][city], weight_pher) * pow(heuristic_record[start_city][city], weight_heu);
			edge_possibility.push_back(each / all);
		}

		/*implement wheel roulete to choose next city*/
		auto spin = [&](double r) -> int
			{
				double acc = 0.0;
				for (int j = 0; j < allowed_k.size(); ++j)
				{
					acc += edge_possibility[j];
					if (acc > r) { return allowed_k[j]; }
				}
				/*很重要!保底!避免因為acc一直沒有超過r而沒有更動到spin選出新的city，導致next city跟前一個city一樣*/
				return allowed_k.back();
			};

		next_city = spin((double)rand() / RAND_MAX);

		/*push next city in path_k*/
		path_k.push_back(next_city);

		/*update allowed_k*/
		allowed_k.erase(remove(allowed_k.begin(), allowed_k.end(), next_city), allowed_k.end());
		
		/*update start city for next run*/
		start_city = next_city;
	}
	return path_k;
}

double algo_ACO::Cal_dis(const vector<int>& visited_order)
{
	TSP tsp;
	double total_dis = tsp.TravelSalesManProblem(visited_order, city_coordinates);

	return total_dis;
}

double algo_ACO::Cal_edge(const int &from, const int &to)
{
	double diff_x = city_coordinates[from].first - city_coordinates[to].first;
	double diff_y = city_coordinates[from].second - city_coordinates[to].second;
	double edge_dis = sqrt(diff_x * diff_x + diff_y * diff_y);

	return edge_dis;
}

vector<int> algo_ACO::SelectTopIdx(const vector<double>& recent_dis_record, const int& top_n)
{
	vector<int> top_n_idx;      /*用來裝最終top n個idx*/
	vector<pair<double, int>> dis_idx; /*裝{dis, idx}以便後面排序選出top n個最短距離的ants*/

	/*create {{dis, idx}, {dis, idx},...}*/
	for (int i = 0; i < ants; ++i)
	{
		dis_idx.push_back({ recent_dis_record[i], i });
	}

	/*依照dis由小排到大*/
	sort(dis_idx.begin(), dis_idx.end());

	/*把top n的idx存入top_n_idx*/
	for (int n = 0; n < top_n && n < ants; ++n)
	{
		top_n_idx.push_back(dis_idx[n].second);
	}
	/*debug*/
	/*
	for (int idx : top_n_idx)
		cout << idx << " ";
	cout << endl;

	for (int i = 0; i < city_num; ++i)
		cout << dis_idx[i].first << " " << dis_idx[i].second << " ";
	cout << endl;
	*/
	return top_n_idx;
}

/* 1.Apply local search on top n ants
   2.Update shortest_path and shortes_dis*/
void algo_ACO::Apply_2_Opt(const int &top_n)
{
	vector<int> top_n_idx = SelectTopIdx(recent_dis_record, top_n);

	for (int idx : top_n_idx)
	{
		vector<int> opt_path = recent_path_record[idx];

		for (int i = 0; i < city_num - 3; ++i)
		{
			for (int j = i + 2; j < city_num - 1; ++j)
			{
				double dis_org = Cal_edge(opt_path[i], opt_path[i + 1])
					           + Cal_edge(opt_path[j], opt_path[j + 1]);
				double dis_swap = Cal_edge(opt_path[i], opt_path[j])
					            + Cal_edge(opt_path[i + 1], opt_path[j + 1]);

				if (dis_org > dis_swap)
				{
					for (int k = 0; k < (j - i) / 2; ++k)
					{
						swap(opt_path[j - k], opt_path[i + k + 1]);
					}
				}
			}
		}
		/*update recent_dis& path record*/
		recent_path_record[idx] = opt_path;
		recent_dis_record[idx] = Cal_dis(opt_path);
	}
	/*after opt all top n path, update shortest_dis & path*/
	for (int idx : top_n_idx)
	{
		if (recent_dis_record[idx] < shortest_dis)
		{
			shortest_dis = recent_dis_record[idx];
			shortest_path = recent_path_record[idx];
		}
	}

	/*update global_shortest_dis & global_shortest_path*/
	if (shortest_dis < global_shortest_dis)
	{
		global_shortest_dis = shortest_dis;
		global_shortest_path = shortest_path;
	}

}

/*update pheromones level of each edge(i, j)*/
void algo_ACO::Update_pheromones()
{
	acc_pheromones = vector<vector<double>>(city_num, vector<double>(city_num, 0.0));
	
	/* 1.acc_pheromones : accumulated pheromones level for each edge*/
	for (int k = 0; k < ants; ++k)
	{
		for (int i = 0; i < city_num - 1; ++i) //add ants k 於有經過的Edge(from, to)上留下的費洛蒙
		{
			int from = recent_path_record[k][i];
			int to = recent_path_record[k][i + 1];
			acc_pheromones[from][to] += Q / recent_dis_record[k];
			acc_pheromones[to][from] += Q / recent_dis_record[k];
		}
		//補上回原點的edge的pheromone累積
		int from = recent_path_record[k].back();
		int to = recent_path_record[k][0];
		acc_pheromones[from][to] += Q / recent_dis_record[k];
		acc_pheromones[to][from] += Q / recent_dis_record[k];
	}
	
	/* 2.new pheromones = pheromones evaporation + add acc_pheromones*/
	for (int i = 0; i < city_num; ++i)
	{
		for (int j = 0; j < city_num; ++j)
		{
			pheromones_record[i][j] = (1 - eva_rate) * pheromones_record[i][j] + acc_pheromones[i][j];
		}
	}
	
	/* 3.額外加上global_path_record中走過的edge的pheromones*/
	for (int i = 0; i < city_num - 1; ++i)
	{
		int from = global_shortest_path[i];
		int to = global_shortest_path[i + 1];
		pheromones_record[from][to] += Q  / global_shortest_dis;
		pheromones_record[to][from] += Q / global_shortest_dis;
	}
	int from = global_shortest_path.back();
	int to = global_shortest_path[0];
	pheromones_record[from][to] += Q / global_shortest_dis;
	pheromones_record[to][from] += Q / global_shortest_dis;
	

	/* 4.[MIN, MAX] range restriction*/
	double PHEROMONE_MIN = 0.000001;
	double PHEROMONE_MAX = 6000;
	/*dynamic [MIN, MAX] range*/
	if (iter_count < iter * 0.6)
	{
		PHEROMONE_MIN += iter_count * 0.001; /*前段探索*/
		PHEROMONE_MAX -= iter_count * 0.5;
	}
	else
	{
		PHEROMONE_MIN -= iter_count * 0.002; /*後段收斂*/
		PHEROMONE_MAX += iter_count * 0.5;
	}
	
	double min_pher = DBL_MAX;
	double max_pher = DBL_MIN;

	for (int i = 0; i < city_num; ++i)
	{
		for (int j = 0; j < city_num; ++j)
		{
			//pheromones_record[i][j] = max(PHEROMONE_MIN, min(PHEROMONE_MAX, pheromones_record[i][j]));

			if (i != j) {
				min_pher = min(min_pher, pheromones_record[i][j]);
				max_pher = max(max_pher, pheromones_record[i][j]);
			}
		
		}
	}
	cout << "[iter " << iter_count << "] pheromones range: " << min_pher << " ~ " << max_pher << endl;
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