#include "TSP.h"
#include <cmath>

double TSP::TravelSalesManProblem(const vector<int>& visited_order,
	const vector<pair<int, int>>& city_coordinates)
{
	int city_num = visited_order.size();   /*cities计秖*/
	double dist = 0.0;

	/*羆翴程翴edge distance羆㎝*/
	for (int i = 0; i < city_num - 1; ++i) /*筂菌┮Τcities*/
	{
		int city_from = visited_order[i];  /*ㄓ*/
		int city_to = visited_order[i + 1];/*玡┕*/
		double diff_x = city_coordinates[city_from].first - city_coordinates[city_to].first;
		double diff_y = city_coordinates[city_from].second - city_coordinates[city_to].second;
		dist += sqrt(diff_x * diff_x + diff_y * diff_y);
	}

	/*干眖程翴翴ê琿edge distance*/
	int city_end = visited_order.back();
	int city_start = visited_order[0];
	double diff_x = city_coordinates[city_start].first - city_coordinates[city_end].first;
	double diff_y = city_coordinates[city_start].second - city_coordinates[city_end].second;
	dist += sqrt(diff_x * diff_x + diff_y * diff_y);

	return dist;
}