#include "TSP.h"
#include "ACO.h"
#include <fstream> /*file stream*/
#include <sstream> /*string stream*/
#include <iostream>
#include <cmath>

void algo_ACO::RunALG(const int& _iter,
	const double& _eva_rate,
	const int& _weight_pher,
	const int& _weight_heu)
{
	srand(time(NULL));
	iter = _iter;
	eva_rate = _eva_rate;
	weight_pher = _weight_pher;
	weight_heu = _weight_heu;
	Init();

	while (iter_count < iter) /*�b�@�Niter���Ʃ|���F�Ыe*/
	{
		/*�C�@�@�N���s�]�w��l���A*/
		recent_dis_record = vector<double>(ants, 0.0);
		recent_path_record = vector<vector<int>>(ants, vector<int>(city_num, -1));
		next_pheromones = vector<vector<double>>(city_num, vector<double>(city_num, 0.0));
		
		for (int k = 0; k < ants; ++k) /*each ants do*/
		{
			int rd_start = rand() % city_num;
			vector<int> path_k = Path_construct(rd_start);
			recent_path_record[k] = path_k;
			recent_dis_record[k] = Cal_dis(recent_path_record[k]);

			for (int i = 0; i < city_num - 1; ++i) /*add ants k �󦳸g�L��Edge(from, to)�W�d�U���O���X*/
			{
				int from = recent_path_record[k][i];
				int to = recent_path_record[k][i + 1];
				next_pheromones[from][to] += 50 / recent_dis_record[k];
			}

			/*update shortest dis and path*/
			if (*min_element(recent_dis_record.begin(), recent_dis_record.end()) < shortest_dis)
			{
				int shortest_id = min_element(recent_dis_record.begin(), recent_dis_record.end()) - recent_dis_record.begin();
				shortest_dis = recent_dis_record[shortest_id];
				shortest_path = recent_path_record[shortest_id];
			}
		}
		/*��̵udis������ants��local search*/
		Apply_2_Opt();

		Update_pheromones();
	}
}

/* 1.create city_coordinates from .txt 
   2.intitialize heuristic and pheromones record */
void algo_ACO::Init()
{
	city_coordinates = ReadCityCoord("ACO_HW_point.txt"); /*��ACO_HW_point.txt��Ʀs�Jcity_coordinates*/
	city_num = city_coordinates.size();
	ants = 30;
	
	/*Initialization for pheromones = 1.0 and heuristic record = 0.0*/
	pheromones_record = vector<vector<double>>(city_num, vector<double>(city_num, 1.0));
	heuristic_record = vector<vector<double>>(city_num, vector<double>(city_num, 0.0));

	/*Initializeation for shortest_path and shortest_dis*/
	vector<int> init_path;
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
				double dist_ij = Cal_dis({ i, j });
				heuristic_record[i][j] = 1.0 / dist_ij;
			}
		}
	}
	cout << "�����ƶq: " << city_coordinates.size() << ", ���Ƽƶq: " << ants << endl;
}

vector<int> algo_ACO::Path_construct(int start_city)
{
	vector<int> path_k(city_num);
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
		vector<vector<double>> edge_possibility = vector<vector<double>>(city_num, vector<double>(city_num, 0.0));
		double all;
		
		for (int city : allowed_k)
		{
			all += pow(pheromones_record[start_city][city], weight_pher) * pow(heuristic_record[start_city][city], weight_heu);
		}

		for (int city : allowed_k)
		{
			double each = pow(pheromones_record[start_city][city], weight_pher) * pow(heuristic_record[start_city][city], weight_heu);
			edge_possibility[start_city][city] = each / all;
		}

		/*implement wheel roulete to choose next city*/
		auto spin = [&](double r) -> int
			{
				double acc = 0.0;
				for (int city : allowed_k)
				{
					acc += edge_possibility[start_city][city];
					if (acc > r) { return city; }
				}
				return city_num - 1;
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

/*�� ifstream �Ӷ}���ɮסC�p�G���ѡA�N��X���~�T�� cerr �æ^�ǪŪ� coordinates*/
vector<pair<int, int>> algo_ACO::ReadCityCoord(const string& filename)
{
	city_coordinates.clear();
	ifstream file(filename);
	if (!file.is_open())
	{
		cerr << "Failed to open file : " << filename << "�нT�{�ɮ׬O�_�P�����ɦb�P�@��Ƨ��C" << endl;
		return city_coordinates; /*empty vector*/
	}

	string line;
	while (getline(file, line)) /*�� getline() �@��Ū�i�@���s�Jline*/
	{
		istringstream iss(line);/*�� line �� istringstream �ন�u�i�H�@�Ӥ@��Ū�Ʀr������v*/
		int id, x, y;           /*Ū�J�T�ӼƦr�Gid, x, y*/
		if (iss >> id >> x >> y)
		{
			city_coordinates.push_back({ x, y });
		}
	}
	file.close();
	return city_coordinates;
}