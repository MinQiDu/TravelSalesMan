#include "TSP.h"  
#include "JADE.h"
#include <fstream>   /*file stream*/
#include <sstream>   /*string stream*/
#include <iostream>
#include <cmath>
#include <algorithm> /*for sort*/

/* Constructor ��l�ƶüƤ��� */
algo_JADE::algo_JADE()
	: gen(rd())
{
}

/* ����t��k */
void algo_JADE::RunALG(const int& _iter,
	const double& _mCR,
	const double& _mF,
	const double& _c,
	const double& _p)
{  
	mCR = _mCR;
	mF = _mF;
	c = _c;
	p = _p;
	iter = _iter;
	pop_size = 60;
	mnfes = pop_size * iter;
	nfes = 0;

	/* ��X���ĵ��G�� */
	iter_c = 0; 
	vector<double> all_iter_shortest_dis;

	/* Initialization */
	Init();

	while (nfes < mnfes) /* Evaluation() ���� Cal_path �����٨S�W�L mnfes �e���� */
	{
		donor_keys_record = Mutation(CR, F, org_keys_record);
		current_keys_record = Crossover(CR, org_keys_record, donor_keys_record);
		current_path_record = GetCityPermutaiton(current_keys_record);
		Evaluation(current_path_record, current_dis_record);
		int top_n = pop_size / 4;
		Apply_2_Opt(top_n);
		Determination(current_shortest_path, current_shortest_dis);
		ParaAdaptation();

		all_iter_shortest_dis.push_back(global_shortest_dis);
		iter_c++;
	}

	/*create dis_result.txt*/
	ofstream file("dis_result_TSP_JADE_total" + to_string(iter) + "iter_pop" + to_string(pop_size) + "_c" + to_string(int(c * 100)) + "_p" + to_string(int(p * 100)) + ".txt");
	for (int i = 0; i < iter; ++i)
	{
		file << i + 1 << " " << all_iter_shortest_dis[i] << "\n";
	}
	file.close();

	/*create .plt for generating plot*/
	ofstream plot("plot_JADE_dis.plt");
	plot << "set terminal png size 800, 600\n";
	plot << "set output 'dis_result_TSP_JADE_total" << iter << "iter_pop" << pop_size << "_c" << c*100 << "_p" << p*100 << ".png'\n";
	plot << "set title 'Convergence with JADE on TravelSalesMan'\n";
	plot << "set xlabel 'Iteration Times'\n";
	plot << "set ylabel 'Shortest Distance'\n";
	plot << "set xrange [0:" << iter << "]\n";
	plot << "set yrange [0:600]\n";
	plot << "plot 'dis_result_TSP_JADE_total" << iter << "iter_pop" << pop_size << "_c" << c*100 << "_p" << p*100 << ".txt' using 1:2 with lines title 'with JADE'";
	plot.close();

	/*create path_result.txt*/
	ofstream file_path("path_result_TSP_JADE_total" + to_string(iter) + "iter_pop" + to_string(pop_size) + "_c" + to_string(int(c * 100)) + "_p" + to_string(int(p * 100)) + ".txt");
	for (int city : global_shortest_path)
	{
		int x = city_coord[city].first;
		int y = city_coord[city].second;
		file_path << x << " " << y << "\n";
	}
	int x0 = city_coord[global_shortest_path.front()].first;
	int y0 = city_coord[global_shortest_path.front()].second;
	file_path << x0 << " " << y0 << "\n";
	file_path.close();

	/*create .plt for generating plot*/
	ofstream plot_path("plot_JADE_path.plt");
	plot_path << "set terminal png size 800, 600\n";
	plot_path << "set output 'path_result_TSP_JADE_total" << iter << "iter_pop" << pop_size << "_c" << c*100 << "_p" << p*100 << ".png'\n";
	plot_path << "set title 'TravelSalesMan Shortest Path with JADE'\n";
	plot_path << "set label 'Distance = " << global_shortest_dis << "' at graph 0.35, 0.95 tc rgb 'black' front\n";
	plot_path << "set size square\n";
	plot_path << "set xrange [0:70]\n";
	plot_path << "set yrange [0:80]\n";
	plot_path << "set key off\n";
	plot_path << "set grid\n";
	plot_path << "plot 'path_result_TSP_JADE_total" << iter << "iter_pop" << pop_size << "_c" << c*100 << "_p" << p*100 << ".txt' with linespoints lt rgb 'purple' pt 7 ps 1.2\n";
	plot_path.close();
}

/* 1. �� ReadCityCoord Ū�J�w�������y�и�T */
/* 2. �� Cal_edge �p�� city_dis �x�} */
/* 3. �� KeyConstruct ���� pop_size �Ӫ�l random keys �Φ��� = org_keys_record */
/* 4. ��l�� global/current_shortest_dis/path */
void algo_JADE::Init()
{
	CR.clear();
	F.clear();
	/* Ū�J TSP_HW_point.txt ��Ʀs�J city_coord */
	city_coord = ReadCityCoord("TSP_HW_point.txt"); 

	dim = city_coord.size();

	city_dis.resize(dim);
	/* �� Cal_edge �p�� city_dis �x�} */
	for (int i = 0; i < dim; ++i)
	{
		city_dis[i].resize(dim);
		for (int j = 0; j < dim; ++j)
		{
			if (i == j)
			{
				city_dis[i][j] = 0.0;
			}

			else
			{
				city_dis[i][j] = Cal_edge(i, j);
			}
		}
	}

	/* ���� pop_size �Ӫ�l random keys �Φ��� = org_keys_record */
	org_keys_record = KeyConstruct(pop_size);

	/* �]�w org_path/dis_record */
	org_path_record = GetCityPermutaiton(org_keys_record);
	org_dis_record.resize(pop_size);
	for (int i = 0; i < pop_size; ++i) org_dis_record[i] = Cal_path(org_path_record[i]);
	
	current_shortest_path.resize(dim);
	global_shortest_path.resize(dim);
	/* ��l�� global/current_shortest_dis/path */
	for (int i = 0; i < dim; ++i)
	{
		current_shortest_path[i] = i;
		global_shortest_path[i] = i;
	}

	current_shortest_dis = Cal_path(current_shortest_path);
	global_shortest_dis = Cal_path(global_shortest_path);

	CR.resize(pop_size);
	F.resize(pop_size);
	current_dis_record.resize(pop_size);
	current_path_record.resize(pop_size);  /* �Ω� Apply_2_Opt�BEvaluation */
	current_keys_record.resize(pop_size);
	donor_keys_record.resize(pop_size);

	cout << "city_num: " << dim << " | pop_num: " << pop_size << " | mCR: " << mCR << " | mF: " << mF << " | c: " << c << " | p: " << p << endl;
}

/* transfer city_coord.txt -> city_coord vector {{x1, y1}, {x2, y2}, ...} */
/* �� ifstream �Ӷ}���ɮסC�p�G���ѡA�N��X���~�T�� cerr �æ^�ǪŪ� coordinates */
vector<pair<int, int>> algo_JADE::ReadCityCoord(const string& filename)
{
	city_coord.clear();
	ifstream file(filename);
	if (!file.is_open())
	{
		cerr << "Failed to open file : " << filename << "�нT�{�ɮ׬O�_�P�����ɦb�P�@��Ƨ��C" << endl;
		return city_coord; /*empty vector*/
	}

	string line;
	while (getline(file, line)) /*�� getline() �@��Ū�i�@���s�Jline*/
	{
		istringstream iss(line);/*�� line �� istringstream �ন�u�i�H�@�Ӥ@��Ū�Ʀr������v*/
		int id, x, y;           /*Ū�J�T�ӼƦr�Gid, x, y*/
		if (iss >> id >> x >> y)
		{
			city_coord.push_back({ x, y });
		}
	}
	file.close();
	return city_coord;
}

/* ���ͦ��N���Ҧ� random keys �Φ��� */
vector<vector<double>> algo_JADE::KeyConstruct(const int& pop_size)
{
	vector<vector<double>> all_keys;
	uniform_real_distribution<double> dist_real(0.0, 1.0);

	all_keys.resize(pop_size);
	for (int i = 0; i < pop_size; ++i)
	{
		all_keys[i].resize(dim); /* ���n��l�� */
		for (int y = 0; y < dim; ++y)
		{
			all_keys[i][y] = dist_real(gen);
		}
	}

	return all_keys;
}

/* calculate total distance of this path */
double algo_JADE::Cal_path(const vector<int>& visited_order)
{
	double sum = 0.0;

	for (int i = 0; i < dim - 1; ++i)
	{
		sum += city_dis[visited_order[i]][visited_order[i + 1]];
	}

	sum += city_dis[visited_order.front()][visited_order.back()]; /* �ɤW�^���I�Z�� */

	return sum;
}

/* calculate distance of this edge */
double algo_JADE::Cal_edge(const int& from, const int& to)
{
	double diff_x = city_coord[from].first - city_coord[to].first;
	double diff_y = city_coord[from].second - city_coord[to].second;
	double edge_dis = sqrt(diff_x * diff_x + diff_y * diff_y);

	return edge_dis;
}

/* 1. �ϥ� current_to_pbest ���ͦ��N�� CR[] & F[] */
/* 2. �ǤJ org_keys_record ���ܲ��^�� donor_keys_record */
vector<vector<double>> algo_JADE::Mutation(vector<double>& CR, vector<double>& F, const vector<vector<double>>& org_keys_record)
{
	normal_distribution<double> dist_CR(mCR, 0.1); /* �`�A���� for �ͦ����骺 CR */
	cauchy_distribution<double> dist_F(mF, 0.1);   /* �_����� for �ͦ����骺 F */
	//uniform_int_distribution<int> dist_rdidx(0, dim - 1);/* ���ä��� for �ͦ��H��index for swap */
	vector<vector<double>> _donor_keys_record;
	//_donor_keys_record = org_keys_record;
	_donor_keys_record.resize(pop_size);

	/* swap mutation */
	/*int swap_times = 1;
	if (iter_c <= 40) swap_times = 30;
	else if (iter_c <= 100) swap_times == 15;
	else if (iter_c <= 500) swap_times == 10;
	else if (iter_c <= 700) swap_times == 5;
	else if (iter_c <= 800) swap_times == 3;
	else if (iter_c <= 1000) swap_times == 1;*/

	for (int i = 0; i < pop_size; ++i)
	{
		_donor_keys_record[i].resize(dim);

		/* �ͦ����N�Ҧ����骺 CR[i] & F[i] */
		CR[i] = dist_CR(gen);
		if (CR[i] < 0.0) CR[i] = 0.0;              /* truncate */
		else if (CR[i] > 1.0) CR[i] = 1.0;

		do {
			F[i] = dist_F(gen);
		} while (F[i] <= 0.0 || F[i] > 1.0);       /* truncate */

		/* swap mutation */
		/*for (int j = 0; j < swap_times; ++j)
		{
			int idx1 = dist_rdidx(gen);
			int idx2;

			do {
				idx2 = dist_rdidx(gen);
			} while (idx2 == idx1);

			swap(_donor_keys_record[i][idx1], _donor_keys_record[i][idx2]);
		}*/

		/* current-to-pbest mutation */
		/* ��Xx_pbest, x_r1, x_r2 �Ӳ��� donor_keys_record */
		/* donor_keys_record[i][j] = x[i][j] + F[i] * (x_pbest[j] - org_keys_record[i][j]) + F[i] * (x_r1[j] - x_r2[j]) */
		/* x_pbest */
		int p_num = p * pop_size;
		if (p_num < 2) p_num = 2;                                   /* �T�O�ܤ֦�2�ӭԿ���� */
		vector<pair<double, int>> dis_ind(pop_size);                /* pair = { distance, individual } */
		for (int j = 0; j < pop_size; ++j)                          /* �إ� dis_ind �H�ѱƧǦC�X���� distance �ƦW */
		{
			dis_ind[j] = { org_dis_record[j], j };
		}
		sort(dis_ind.begin(), dis_ind.end());                       /* distance �Ѥp->�j */

		int idx_pbest;
		do {
			uniform_int_distribution<int> dist_pbest(0, p_num - 1); /* ��Ƨ��ä��� for �ͦ�index */
			idx_pbest = dis_ind[dist_pbest(gen)].second;            /* pbest �b org_keys_record ���� index */
		} while (idx_pbest == i);                                   /* �T�Oidx_pbest���e����i���P */
		vector<double> x_pbest = org_keys_record[idx_pbest];                    /* �o�� x_pbest */

		/* x_r1 */
		uniform_int_distribution<int> dist_r1(0, pop_size - 1);
		int idx_r1;
		do {
			idx_r1 = dist_r1(gen);
		} while (idx_r1 == i || idx_r1 == idx_pbest);
		vector<double> x_r1 = org_keys_record[idx_r1];                          /* �o�� x_r1 */

		/* x_r2 */
		uniform_int_distribution<int> dist_r2(0, pop_size + A.size() - 1);
		vector<vector<double>> org_A = org_keys_record;                         /* ���ƻs org_keys_record */
		org_A.insert(org_A.end(), A.begin(), A.end());              /* �A���W A */
		int idx_r2;
		do {
			idx_r2 = dist_r2(gen);
		} while (idx_r2 == i || idx_r2 == idx_pbest || idx_r2 == idx_r1);
		vector<double> x_r2 = org_A[idx_r2];                        /* �o�� x_r2 */

		for (int k = 0; k < dim; ++k)                               /* ��X�����骺 donor �� */
		{
			_donor_keys_record[i][k] = org_keys_record[i][k] + F[i] * (x_pbest[k] - org_keys_record[i][k]) + F[i] * (x_r1[k] - x_r2[k]);
		}
	}
	return _donor_keys_record;
}

/* org & donor ��t�^�ǳ̲� random keys �Φ��� current_keys_record */
vector<vector<double>> algo_JADE::Crossover(const vector<double>& CR, const vector<vector<double>>& org_keys_record, const vector<vector<double>>& donor_keys_record)
{
	uniform_int_distribution<int> dist_int(0, dim - 1);    /* ��Ƨ��ä��� for �ͦ�index */
	uniform_real_distribution<double> dist_real(0.0, 1.0); /* ��Ƨ��ä��� for �ͦ� r */
	vector<vector<double>> _current_keys_record;
	_current_keys_record.resize(pop_size);

	for (int i = 0; i < pop_size; ++i)
	{
		_current_keys_record[i].resize(dim);
		int jrand = dist_int(gen);       /* �O�ҥ����ܤ֤@��idx���ܲ� */

		for (int j = 0; j < dim; ++j)
		{
			double r = dist_real(gen);

			if (r < CR[i] || jrand == j) /* ���H��[0, 1]�B�I�Ƥp�� cross_rate �άO�H�� idx �����e idx */
			{
				_current_keys_record[i][j] = donor_keys_record[i][j]; /* �N�ܲ����s�� */
			}
			else
			{
				_current_keys_record[i][j] = org_keys_record[i][j];       /* �_�h������� */
			}
		}
	}
	return _current_keys_record;
}

/* continuous-to-permutation �����A�N random keys �ন permutation (�g�L����������)�Acurrent_keys_record -> current_path_record */
vector<vector<int>> algo_JADE::GetCityPermutaiton(const vector<vector<double>>& _current_keys_record)
{
	vector<vector<int>> cpr;
	cpr.resize(pop_size);
	for (int i = 0; i < pop_size; ++i)
	{
		cpr[i].resize(dim);
		vector<pair<double, int>> key_city(dim);    /* pair = { random key, ������ city } */
		for (int j = 0; j < dim; ++j)               /* �إ� key_city �H�ѱƧǦC�X���� distance �ƦW */
		{
			key_city[j] = { _current_keys_record[i][j], j };
		}
		sort(key_city.begin(), key_city.end());     /* random keys �Ѥp->�j */
		for (int k = 0; k < dim; ++k)
		{
			cpr[i][k] = key_city[k].second;         /* �ഫ�� city id */
		}
	}

	return cpr;
}

/* �p�� current_path_record ���|����, �p�J curremt_dis_record */
void algo_JADE::Evaluation(const vector<vector<int>>& current_path_record, vector<double>& current_dis_record)
{
	for (int i = 0; i < pop_size; ++i)
	{
		current_dis_record[i] = Cal_path(current_path_record[i]);
		nfes++;
	}
}

/* 1. Apply local search on top n individuals */
/* 2. update current_shortest_path/dis */
void algo_JADE::Apply_2_Opt(const int& top_n)
{
	vector<int> top_n_ind = SelectTopInd(current_dis_record, top_n);

	for (int idx : top_n_ind)
	//for (int idx = 0; idx < pop_size; ++idx)
	{
		vector<int> opt_path = current_path_record[idx];
		vector<double> opt_key = current_keys_record[idx];

		for (int i = 0; i < dim - 3; ++i)
		{
			for (int j = i + 2; j < dim - 1; ++j)
			{
				double dis_org = Cal_edge(opt_path[i], opt_path[i + 1])
					+ Cal_edge(opt_path[j], opt_path[j + 1]);
				double dis_swap = Cal_edge(opt_path[i], opt_path[j])
					+ Cal_edge(opt_path[i + 1], opt_path[j + 1]);

				if (dis_org > dis_swap)
				{
					for (int k = 0; k < (j - i) / 2; ++k)
					{
						swap(opt_path[j - k], opt_path[i + k + 1]); /* ��s current_path_record */
						swap(opt_key[j - k], opt_key[i + k + 1]);   /* �]�n��s current_keys_record */
					}
				}
			}
		}
		/* update current_dis/path/keys_record */
		current_path_record[idx] = opt_path;
		current_dis_record[idx] = Cal_path(opt_path);
		current_keys_record[idx] = opt_key;
	}
	/* after opt all top n path, update shortest_dis & path */
	for (int idx : top_n_ind)
	//for (int idx = 0; idx < pop_size; ++idx)
	{
		if (current_dis_record[idx] < current_shortest_dis)
		{
			current_shortest_dis = current_dis_record[idx];
			current_shortest_path = current_path_record[idx];
		}
	}
}

/* Select & return top n shortest dis individuals */
vector<int> algo_JADE::SelectTopInd(const vector<double>& current_dis_record, const int& top_n)
{
	vector<int> top_n_ind;             /* �ΨӸ˳̲�top n��idx */
	vector<pair<double, int>> dis_ind; /* ��{ dis, individual } �H�K�᭱�Ƨǿ�X top n �ӳ̵u�Z���� individuals */

	/* create {{dis, ind}, {dis, ind},...} */
	for (int i = 0; i < pop_size; ++i)
	{
		dis_ind.push_back({ current_dis_record[i], i });
	}

	/* �̷�dis�Ѥp�ƨ�j */
	sort(dis_ind.begin(), dis_ind.end());

	/* �� top n �� ind �s�J top_n_ind */
	for (int n = 0; n < top_n && n < pop_size; ++n)
	{
		top_n_ind.push_back(dis_ind[n].second);
	}

	return top_n_ind;
}

/* 1. ��s A & sCR & sF */
/* 2. ��� global_shortest_dis & current_shortest_dis ���סA ��s global_shortest_dis/path */
void algo_JADE::Determination(const vector<int>& current_shortest_path, const double& current_shortest_dis)
{
	for (int i = 0; i < pop_size; ++i)
	{
		if (current_dis_record[i] < org_dis_record[i])       /* ��current���u���ѡA��s */
		{
			A.push_back(org_keys_record[i]);           /* �^�O����Ѧs�J A */
			org_path_record[i] = current_path_record[i];
			org_dis_record[i] = current_dis_record[i];
			org_keys_record[i] = current_keys_record[i];
			sF.push_back(F[i]);            /* ���\��s���骺 F �s�J sF */
			sCR.push_back(CR[i]);          /* ���\��s���骺 CR �s�J sCR */
		}
		if (current_shortest_dis < global_shortest_dis) /* �� current_shortest_dis �u��̨θѡA��s */
		{
			global_shortest_dis = current_shortest_dis;
			global_shortest_path = current_shortest_path;
		}
	}
}

/* mCR & mF adaptation by rate c */
void algo_JADE::ParaAdaptation()
{
	/* maintain size of A <= pop_size */
	while (A.size() > pop_size)
	{
		uniform_int_distribution<int> dist_idx(0, A.size() - 1);
		int idx = dist_idx(gen);
		A.erase(A.begin() + idx);
	}

	/* mCR & mF adaptation */
	/* mCR */
	double sum_sCR = 0.0;
	for (double cr : sCR) sum_sCR += cr;
	double mean_sCR = sum_sCR / sCR.size(); /* arithmetic mean */
	mCR = (1 - c) * mCR + c * mean_sCR;     /* �o��smCR */

	/* mF */
	double sum_sF2 = 0.0;
	double sum_sF = 0.0;
	for (double f : sF)
	{
		sum_sF2 += f * f;
		sum_sF += f;
	}
	double mean_sF = sum_sF2 / sum_sF;     /* Lehmer mean */
	mF = (1 - c) * mF + c * mean_sF;       /* �o��smF */

	sCR.clear();						   /* �M�� for �U�@�N */
	sF.clear();
}

