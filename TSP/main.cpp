#include "TSP.h"
#include "ACO.h"
#include "JADE.h"
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main(int argc, char *argv[])
{
	int iter = 0;
	while (iter == 0)
	{
		iter = atoi(argv[1]);
		//double eva_rate = atof(argv[2]);
		//int weight_pher = atoi(argv[3]);
		//int weight_heu = atoi(argv[4]);
	}
	
	string algo_type;

	bool canrun = 1;
	while (canrun)
	{
		cout << "Choose Algorithm ( ACO / JADE )" << endl;
		getline(cin, algo_type);
		transform(algo_type.begin(), algo_type.end(), algo_type.begin(), toupper); /* 輸入大小寫都可以，最後切回大寫 */

		if (algo_type == "ACO")
		{
			canrun = 0;

			string e, wp, wh;
			cout << "Please enter eva_rate = ";
			getline(cin, e);
			cout << "Please enter weight_pher = ";
			getline(cin, wp);
			cout << "Please enter weight_heu = ";
			getline(cin, wh);

			double eva_rate = stod(e);
			int weight_pher = stoi(wp);
			int weight_heu = stoi(wh);

			algo_ACO algo;
			algo.RunALG(iter, eva_rate, weight_pher, weight_heu);
		}

		else if (algo_type == "JADE")
		{
			canrun = 0;

			string cr, f, _c, _p;
			cout << "Please enter initial mCR = ";
			getline(cin, cr);
			cout << "Please enter initial mF = ";
			getline(cin, f);
			cout << "Please enter the value of adaptation rate c = ";
			getline(cin, _c);
			cout << "Please enter the value of the top % pop chosen for pbest = ";
			getline(cin, _p);

			double mCR = stod(cr);
			double mF = stod(f);
			double c = stod(_c);
			double p = stod(_p);

			algo_JADE algo;
			algo.RunALG(iter, mCR, mF, c, p);
		}
	}

	return 0;
}