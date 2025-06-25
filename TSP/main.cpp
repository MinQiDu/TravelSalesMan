#include "TSP.h"
#include "ACO.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		cerr << "Usage: ./your_program <iter> <eva_rate> <pher> <heu>\n";
		return 1; /*argument不完整，回傳error*/
	}
	int iter = atoi(argv[1]);
	double eva_rate = atof(argv[2]);
	int weight_pher = atoi(argv[3]);
	int weight_heu = atoi(argv[4]);

	algo_ACO algo;
	algo.RunALG(iter, eva_rate, weight_pher, weight_heu);

	return 0;
}