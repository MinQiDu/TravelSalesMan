#ifndef JADE_H /* Include Guard: ����h�� include �ɭP�w�q�Ĭ�A�b compiler ���e���檺����P�_�޿� */
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
	random_device rd;  /*�ŧi�üƤ���*/
	mt19937 gen;

	/* Command-Line arguments */
	int iter;
	double mCR;
	double mF;
	double c;
	double p;

	/* variables */
	int dim;									/* �o�䪺���� = �����Ӽ� */
	int pop_size;								/* �@�N�ѭӼ� */
	int mnfes;									/* iter * pop_size */
	int nfes;									/* current ���� Evaluation() ���� Cal_path ���� */
	vector<pair<int, int>> city_coord;			/* city coordinates data */
	vector<vector<double>> city_dis;			/* �����C��ӫ����������Z�� */
	vector<double> CR;							/* ���N�C�ӭ��骺 CR */
	vector<double> F;							/* ���N�C�ӭ��骺 F */
	vector<double> sCR;							/* ���N�����\��s�����骺 CR */
	vector<double> sF;							/* ���N�����\��s�����骺 F */
	vector<vector<double>> A;					/* ���N���Q�^�O������ random keys �Φ��ѡAsize = pop_size */
	vector<vector<double>> org_keys_record;		/* �� random keys �Φ��� */
	vector<vector<double>> donor_keys_record;   /* mutate �� random keys �Φ��� */
	vector<vector<double>> current_keys_record; /* crossover �� random keys �Φ��s�� */
	vector<double> org_dis_record;				/* distance record of original sol */
	vector<vector<int>> org_path_record;		/* path record of original sol */
	vector<double> current_dis_record;			/* distance record of this iter */
	vector<vector<int>> current_path_record;	/* path record of this iter */
	double current_shortest_dis;				/* ���� current �̵u dis */
	vector<int> current_shortest_path;			/* ���� current �̵u path */
	double global_shortest_dis;					/* ���N�̵u dis ���� */
	vector<int> global_shortest_path;			/* ���N�̵u path ���� */

	/* functions */

	/* 1. �� ReadCityCoord Ū�J�w�������y�и�T */
	/* 2. �� Cal_edge �p�� city_dis �x�} */
	/* 3. �� KeyConstruct ���� pop_size �Ӫ�l random keys �Φ��� = org_keys_record */
	/* 4. ��l�� global/current_shortest_dis/path */
	void Init();                                                               

	vector<pair<int, int>> ReadCityCoord(const string& filename);              /* transfer city_coord.txt -> city_coord vector {{x1, y1}, {x2, y2}, ...} */
	vector<vector<double>> KeyConstruct(const int& pop_size);                  /* ���ͦ��N���Ҧ� random keys �Φ��� */
	double Cal_path(const vector<int>& visited_order);                         /* calculate total distance of this path */
	double Cal_edge(const int& from, const int& to);                           /* calculate total distance of this edge */

	vector<vector<double>> Mutation(vector<double>& CR,                        /* 1. �ϥ� current_to_pbest ���ͦ��N�� CR[] & F[] */
		vector<double>& F,                                                     /* 2. �ǤJ org_keys_record ���ܲ��^�� donor_keys_record */
		const vector<vector<double>>& org_keys_record);

	vector<vector<double>> Crossover(const vector<double>& CR,                 /* org & donor ��t�^�ǳ̲� random keys �Φ��� current_keys_record */
		const vector<vector<double>>& org_keys_record,
		const vector<vector<double>>& donor_keys_record);

	vector<vector<int>> GetCityPermutaiton(const vector<vector<double>>& current_keys_record);  /* continuous-to-permutation �����A�N random keys �ন permutation (�g�L����������)�Acurrent_pop -> current_path_record */

	void Evaluation(const vector<vector<int>>& current_path_record,            /* �p�� current_path_record ���|����, �p�J curremt_dis_record */
		vector<double>& current_dis_record);

	/* 1. Apply local search on top n individuals */
	/* 2. update current_shortest_path/dis */
	void Apply_2_Opt(const int& top_n);                                        

	vector<int> SelectTopInd(const vector<double>& current_dis_record, const int& top_n); /* Select & return top n shortest dis individuals */

	/* 1. ��s A & sCR & sF */
	/* 2. ��� global_shortest_dis & current_shortest_dis ���סA ��s global_shortest_dis/path */
	void Determination(const vector<int>& current_shortest_path,             
		const double& current_shortest_dis);

	void ParaAdaptation();													   /* mCR & mF adaptation by rate c */
};

#endif