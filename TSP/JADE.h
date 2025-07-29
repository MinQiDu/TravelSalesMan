#ifndef JADE_H /* Include Guard: 防止多次 include 導致定義衝突，在 compiler 之前執行的條件判斷邏輯 */
#define JADE_H
#include "TSP.h"
#include <random>
#include <vector>
using namespace std;


class algo_JADE
{
public:
	algo_JADE(); /* constructor */
	void RunALG(const int& iter,
		const double& mCR,
		const double& mF,
		const double& c,
		const double& p);

private:
	random_device rd;  /*宣告亂數引擎*/
	mt19937 gen;

	/* Command-Line arguments */
	int iter;
	double mCR;
	double mF;
	double c;
	double p;

	/* variables */
	int dim;									/* 這邊的維度 = 城市個數 */
	int pop_size;								/* 一代解個數 */
	int mnfes;									/* iter * pop_size */
	int nfes;									/* current 執行 Evaluation() 中的 Cal_path 次數 */
	vector<pair<int, int>> city_coord;			/* city coordinates data */
	vector<vector<double>> city_dis;			/* 紀錄每兩個城市之間的距離 */
	vector<double> CR;							/* 此代每個個體的 CR */
	vector<double> F;							/* 此代每個個體的 F */
	vector<double> sCR;							/* 此代中成功更新的個體的 CR */
	vector<double> sF;							/* 此代中成功更新的個體的 F */
	vector<vector<double>> A;					/* 歷代中被淘汰掉的原 random keys 形式解，size = pop_size */
	vector<vector<double>> org_keys_record;		/* 原 random keys 形式解 */
	vector<vector<double>> donor_keys_record;   /* mutate 後 random keys 形式解 */
	vector<vector<double>> current_keys_record; /* crossover 後 random keys 形式新解 */
	vector<double> org_dis_record;				/* distance record of original sol */
	vector<vector<int>> org_path_record;		/* path record of original sol */
	vector<double> current_dis_record;			/* distance record of this iter */
	vector<vector<int>> current_path_record;	/* path record of this iter */
	double current_shortest_dis;				/* 紀錄 current 最短 dis */
	vector<int> current_shortest_path;			/* 紀錄 current 最短 path */
	double global_shortest_dis;					/* 歷代最短 dis 紀錄 */
	vector<int> global_shortest_path;			/* 歷代最短 path 紀錄 */

	/* functions */

	/* 1. 用 ReadCityCoord 讀入已知城市座標資訊 */
	/* 2. 用 Cal_edge 計算 city_dis 矩陣 */
	/* 3. 用 KeyConstruct 產生 pop_size 個初始 random keys 形式解 = org_keys_record */
	/* 4. 初始化 global/current_shortest_dis/path */
	void Init();                                                               

	vector<pair<int, int>> ReadCityCoord(const string& filename);              /* transfer city_coord.txt -> city_coord vector {{x1, y1}, {x2, y2}, ...} */
	vector<vector<double>> KeyConstruct(const int& pop_size);                  /* 產生此代的所有 random keys 形式解 */
	double Cal_path(const vector<int>& visited_order);                         /* calculate total distance of this path */
	double Cal_edge(const int& from, const int& to);                           /* calculate total distance of this edge */

	vector<vector<double>> Mutation(vector<double>& CR,                        /* 1. 使用 current_to_pbest 產生此代的 CR[] & F[] */
		vector<double>& F,                                                     /* 2. 傳入 org_keys_record 做變異回傳 donor_keys_record */
		const vector<vector<double>>& org_keys_record);

	vector<vector<double>> Crossover(const vector<double>& CR,                 /* org & donor 交配回傳最終 random keys 形式解 current_keys_record */
		const vector<vector<double>>& org_keys_record,
		const vector<vector<double>>& donor_keys_record);

	vector<vector<int>> GetCityPermutaiton(const vector<vector<double>>& current_keys_record);  /* continuous-to-permutation 策略，將 random keys 轉成 permutation (經過城市的順序)，current_pop -> current_path_record */

	void Evaluation(const vector<vector<int>>& current_path_record,            /* 計算 current_path_record 路徑長度, 計入 curremt_dis_record */
		vector<double>& current_dis_record);

	/* 1. Apply local search on top n individuals */
	/* 2. update current_shortest_path/dis */
	void Apply_2_Opt(const int& top_n);                                        

	vector<int> SelectTopInd(const vector<double>& current_dis_record, const int& top_n); /* Select & return top n shortest dis individuals */

	/* 1. 更新 A & sCR & sF */
	/* 2. 比較 global_shortest_dis & current_shortest_dis 長度， 更新 global_shortest_dis/path */
	void Determination(const vector<int>& current_shortest_path,             
		const double& current_shortest_dis);

	void ParaAdaptation();													   /* mCR & mF adaptation by rate c */
};

#endif