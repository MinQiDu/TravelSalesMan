#ifndef ACO_H
#define ACO_H

#include "TSP.h"
#include <string>

class algo_ACO
{
public :
	void RunALG(const int &iter, 
		const double &eva_rate, 
		const int &weight_pher, 
		const int &weight_dis);              /*implement ACO*/

private :
	int iter;                                /*要執行幾代螞蟻群來決定最佳解*/
	int iter_count;                          /*當前iter數量*/
	int ants;                                /*螞蟻數量*/
	double eva_rate;                         /*evaporation rate*/
	int city_num;                            /*city_num according to city_cood*/
	
	int weight_pher;                         /*計算機率時的費洛蒙比重*/
	int weight_dis;                          /*計算機率時的距離比重(greedy)*/
	vector<pair<int, int>> city_coordinates; /*city coordinates data*/

	double shortest_dis;                     /*紀錄當前最短距離*/
	vector<int> shortest_path;               /*紀錄當前最短路徑*/

	vector<double> recent_dis_record;        /*distance record of this iter*/
	vector<vector<int>> recent_path_record;  /*path record of this iter*/

	vector<vector<double>> heuristic_record; /*heu(ij) : 每段ij的1/distance(i,j)*/
	vector<vector<double>> pheromones_record;/*phe(ij) : 每段ij的pheromones level*/
	vector<vector<double>> next_pheromones;  /*sum all ants' path[i,j] pheromones level for next iter*/
	//vector<vector<double>> edge_possibility;


	void Init();                             /* 1.create city_coordinates from .txt 2.intitialize heuristic and pheromones record */
	                                         /*While (iter_count < iter_max)*/
	vector<double> Cal_possibility(const double &a, const double &b); /*calculate第k隻Ant從節點i移動到節點j的機率*/
	vector<int> Path_construct(int start_city);                       /*construct a path visiting all cities*/
	double Cal_dis(const vector<int> &visited_order);                 /*calculate total distance of this path*/
	void Apply_2_Opt();                      /*Apply local search on specific ants*/
	                                         /*and update shortest_path and shortes_dis*/
	void Update_pheromones();                /*update pheromones level for next iter*/
	vector<pair<int, int>> ReadCityCoord(const string& filename);
};

#endif
