#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <iostream>
#include <vector>
#include "auxiliares.h"

using namespace std;

void Gauss(vector<vector<double>>& matriz, vector<double> b);

void LU(vector<vector<double>>& matriz, vector<vector<double>>& L);

vector<double> resolver_sistema(vector<vector<double>> matriz_ts, bool triangular_i, vector<double> vector_b);

vector<double> resolver_gauss(vector<vector<double>> matriz, vector<double> b);

vector<double> resolver_LU(vector<vector<double>> matriz, vector<vector<double>> l, vector<double> b);


#endif // ALGORITMOS_H