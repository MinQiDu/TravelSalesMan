#ifndef TSP_H
#define TSP_H

#include <vector>
using namespace std;

class TSP
{
public :
	double TravelSalesManProblem(
		const vector<int> &visited_order, 
		const vector<pair<int, int>> & city_coordinates);
};

#endif
