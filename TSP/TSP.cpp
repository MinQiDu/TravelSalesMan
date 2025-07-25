#include "TSP.h"
#include <cmath>

double TSP::TravelSalesManProblem(const vector<int>& visited_order,
	const vector<pair<int, int>>& city_coordinates)
{
	int city_num = visited_order.size();   /*cities的數量*/
	double dist = 0.0;

	/*加總原點到最後一點edge distance總和*/
	for (int i = 0; i < city_num - 1; ++i) /*遍歷所有cities*/
	{
		int city_from = visited_order[i];  /*來自*/
		int city_to = visited_order[i + 1];/*前往*/
		double diff_x = city_coordinates[city_from].first - city_coordinates[city_to].first;
		double diff_y = city_coordinates[city_from].second - city_coordinates[city_to].second;
		dist += sqrt(diff_x * diff_x + diff_y * diff_y);
	}

	/*補上從最後一點回原點那一段edge distance*/
	int city_end = visited_order.back();
	int city_start = visited_order[0];
	double diff_x = city_coordinates[city_start].first - city_coordinates[city_end].first;
	double diff_y = city_coordinates[city_start].second - city_coordinates[city_end].second;
	dist += sqrt(diff_x * diff_x + diff_y * diff_y);

	return dist;
}