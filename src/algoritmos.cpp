#include "algoritmos.h"
#include <iostream>

using namespace std;

    /////////////////////////////////////////
    //               EG y LU               //
    /////////////////////////////////////////

// Realiza la eliminación gaussiana de una matriz
void Gauss(vector<vector<double>>& matriz, vector<double> b) {
    int n = matriz.size();
    push_back_column(matriz, b);    // Agregamos el vector b a la matriz

    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            double m = matriz[j][i]/matriz[i][i];
            for (int k = i; k < n + 1; k++) {
                double valor = m*matriz[i][k];
                matriz[j][k] = matriz[j][k] - valor;
            }
        }
    }
}

// Realiza la factorización LU de una matriz
void LU(vector<vector<double>>& matriz, vector<vector<double>>& L) {
    int n = matriz.size();
    for (int i = 0; i < n; i++) L[i][i] = 1;    // Agrega 1s en la diagonal de la matriz L
    
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            double m = matriz[j][i]/matriz[i][i];
            for (int k = i; k < n; k++) {
                double valor = m * matriz[i][k];
                matriz[j][k] = matriz[j][k] - valor;
            }
            L[j][i] = m;
        }
    }
}

    /////////////////////////////////////////
    //        Resolución del sistema       //
    /////////////////////////////////////////

// Resuelve una matriz triangular superior/inferior.
vector<double> resolver_sistema(vector<vector<double>> matriz_t, bool triangular_i, vector<double> vector_b) {
   
    int n = matriz_t.size();
    vector<double> solucion = vector<double>(n,0);
    double suma_fila = 0;
    
    if (matriz_t.size() == matriz_t[0].size())                          // Si no tiene el vector b, se le agrega
        push_back_column(matriz_t, vector_b);
    
    if (triangular_i) {                                                 // Triangular Inferior
        solucion[0] = matriz_t[0][n]/matriz_t[0][0];
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i ; j++) 
                suma_fila += matriz_t[i][j]*solucion[j]; 
            solucion[i] = (matriz_t[i][n] - suma_fila)/matriz_t[i][i];
            suma_fila = 0;
        }
    } else {                                                            // Triangular Superior
        solucion[n-1] = matriz_t[n-1][n]/matriz_t[n-1][n-1];
        for (int i = n-2; i >= 0; i--) {
            for (int j = i+1; j < n ; j++)
                suma_fila += matriz_t[i][j]*solucion[j]; 
            solucion[i] = (matriz_t[i][n] - suma_fila)/matriz_t[i][i];
            suma_fila = 0;
        }
    }

    return solucion;
}

// Resuelve un sistema con Gauss
vector<double> resolver_gauss(vector<vector<double>> matriz, vector<double> b) {
    Gauss(matriz, b);
    vector<double> solucion_gauss = resolver_sistema(matriz, false, b);

    return solucion_gauss;
}

// Resuelve un sistema con factorización LU
vector<double> resolver_LU(vector<vector<double>> matriz, vector<vector<double>> l,vector<double> b) {
    vector<double> y            = resolver_sistema(l, true, b);         // Se resuelve Ly = b
    vector<double> solucion_lu  = resolver_sistema(matriz, false, y);   // Se resuelve Ux = y

    return solucion_lu; 
}