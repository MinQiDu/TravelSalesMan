#include "TSP.h"
#include <cmath>

double TSP::TravelSalesManProblem(const vector<int>& visited_order,
	const vector<pair<int, int>>& city_coordinates)
{
	int city_num = visited_order.size();   /*cities���ƶq*/
	double dist = 0.0;

	for (int i = 0; i < city_num - 1; ++i) /*�M���Ҧ�cities*/
	{
		int city_from = visited_order[i];  /*�Ӧ�*/
		int city_to = visited_order[i + 1];/*�e��*/
		double diff_x = city_coordinates[city_from].first - city_coordinates[city_to].first;
		double diff_y = city_coordinates[city_from].second - city_coordinates[city_to].second;
		dist += sqrt(diff_x * diff_x + diff_y * diff_y);
	}
	return dist;
}