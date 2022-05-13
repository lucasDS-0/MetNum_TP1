#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include "algoritmos.h"

using namespace std;

// Variables que se utilizaran para la lectura del input
double r_i = 0;
double r_e = 0;
int m_plus_one = 0;     
int n = 0;
double iso = 0;
int ninst = 0;

int main(int argc, char **argv){
    // Armamos el sistema
    map <string, string> algoritmos = {
        {"0", "Eliminación Gausiana"},
        {"1", "Factorización LU"}
    };

    // Se verifica que se pase la cantidad de parámetros adecuada y que el algoritmo exista
    if (argc < 4 || algoritmos.find(argv[3]) == algoritmos.end()) {
        cout << "USO: ./ejecutable input_file output_file < 0 | 1 > " << endl;
        return 1;
    }

    /////////////////////////////////////////
    //          Lectura del Input          //
    /////////////////////////////////////////

    ifstream input_file;
    input_file.open(argv[1], ifstream::in);

    // Se verifica que se pueda leer el input
    if (input_file.fail()) {cout << "Error al leer el archivo de instancias" << endl;}

    string algoritmo = argv[3];     // Algoritmo a uilizar 0 : EG, 1 : LU

    // Se toman los parámetros del experimento
    // radio interno > radio externo > m+1 > n > isoterma buscada > cantidad de instancias
    input_file >> r_i >> r_e >> m_plus_one >> n >> iso >> ninst;

    // Se crean las paredes internas y externas para cada instancia
    vector<vector<double>> pared_interna = vector<vector<double>>(ninst,vector<double>(n,0));
    vector<vector<double>> pared_externa = vector<vector<double>>(ninst,vector<double>(n,0));

    for (int i = 0; i < ninst; i++) {
        for (int j = 0; j < n; j++) input_file >> pared_interna[i][j];
        for (int h = 0; h < n; h++) input_file >> pared_externa[i][h];
    }

    // Una vez leido todo, se cierra el archivo de entrada
    input_file.close();

    /////////////////////////////////////////
    //       Resolución del problema       //
    /////////////////////////////////////////

    vector<vector<vector<double>>> hornos = {};       
    vector<double> tiempos = {};
    vector<vector<double>> l = {};
    vector<vector<double>> copia_lu = {};

    for (int i = 0; i < ninst; i++) {
        // Calculamos el sistema a resolver
        pair<vector<vector<double>>,vector<double>> sistema_extendido = 
                                            armar_sistema(pared_interna[i], pared_externa[i],r_i, r_e, m_plus_one-1);
        
        // Se toma el tiempo que tarda en encontrar la solución del sistema
        auto start = chrono::steady_clock::now();

        // Obtenemos su vector solución con el algoritmo especificado
        vector<double> vector_solucion = {};
        if (algoritmo == "1") {
            if (!i)  { // Si es la primer instancia
                l = vector<vector<double>>(get<0>(sistema_extendido).size(), 
                    vector<double>(get<0>(sistema_extendido)[0].size(), 0));
                copia_lu = get<0>(sistema_extendido); // Copia para que no modifique el sistema original
                LU(copia_lu,l);       // Factorización LU
            }
            vector_solucion = resolver_LU(copia_lu, l, get<1>(sistema_extendido));    // LU
        } else {
            vector_solucion = resolver_gauss(get<0>(sistema_extendido), get<1>(sistema_extendido));    // Gauss    
        }     
        
        auto end = chrono::steady_clock::now();

        // Se guarda el tiempo de ejecucións
        double total_time = chrono::duration<double, milli>(end - start).count();
        tiempos.push_back(total_time);

        // Obtenemos el horno con sus temperaturas
        vector<vector<double>> horno =  calcular_temperaturas(pared_interna[i], pared_externa[i], sistema_extendido, 
                                                             vector_solucion, n, m_plus_one-1);

        hornos.push_back(horno);

    }

    /////////////////////////////////////////
    //        Escritura del output         //
    /////////////////////////////////////////

    ofstream output_file;
    output_file.open(argv[2], ofstream::out | std::ofstream::trunc);

    // Se escribe el horno un columna (radio) a la vez
    for (int h = 0; h < ninst; h++) {
        for (unsigned int i = 0; i < hornos[h][0].size(); i++) {
            for (unsigned int j = 0; j < hornos[h].size(); j++)
                output_file << hornos[h][j][i] << endl;
        }
    }

    // Una vez escrito todo, se cierra el archivo de salida
    output_file.close();
    
    // Se escriben los tiempos de ejecución en los logs
    for (int i = 0; i < ninst; i++) {
        cerr << tiempos[i] << endl;
    }

    return 0;
}