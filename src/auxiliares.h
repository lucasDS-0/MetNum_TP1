#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

void push_back_column(vector<vector<double>>& matriz, const vector<double>& vector_b);

ostream &operator<<(ostream& os, const vector<vector<double>>& matrix);

ostream &operator<<(ostream& os, const vector<double>& vector);

ostream &operator<<(ostream& os, const vector<vector<pair<double, pair<int,int>>>>& matrix);

ostream &operator<<(ostream& os, const vector<pair<double, pair<int,int>>>& vector);

pair<vector<vector<double>>, vector<double>> armar_sistema(vector<double> p_interna, vector<double> p_externa, 
                                                           double r_in, double r_ex, int m);

vector<vector<double>> calcular_temperaturas(vector<double> pared_interna, 
                                             vector<double> pared_externa,
                                             pair<vector<vector<double>>, vector<double>> sistema, 
                                             vector<double> vec_sol, int n, int m);

#endif // AUXILIARES_H