#include "auxiliares.h"

    /////////////////////////////////////////
    //   Funciones de impresión y debug    //
    /////////////////////////////////////////

// Sobrecarga del operador para imprimir una matriz.
ostream& operator<<(ostream& os, const vector<vector<double>>& matrix) {
    for (unsigned int i = 0; i < matrix.size(); i++) {
        for (unsigned int j = 0; j < matrix[0].size(); j++)
            os << setw(5) << matrix[i][j] << ' ';
        os << endl;
    }

    return os;
}

// Sobrecarga del operador para imprimir una matriz de pares.
ostream& operator<<(ostream& os, const vector<vector<pair<double, pair<int,int>>>>& matrix) {
    for (unsigned int i = 0; i < matrix.size(); i++) {
        for (unsigned int j = 0; j < matrix[0].size(); j++)
            os << '<' << get<0>(matrix[i][j]) << ',' 
            << '<' << get<0>(get<1>(matrix[i][j])) << ',' 
            << get<1>(get<1>(matrix[i][j])) << '>' << '>' << ' ';
        os << endl;
    }
    
    return os;
}

// Sobrecarga del operador para imprimir un vector fila de pares.
ostream& operator<<(ostream& os, const vector<pair<double, pair<int,int>>>& vector) {
    os << '[';
    for (unsigned int i = 0; i < vector.size(); i++) {
            os << '<' << get<0>(vector[i]) << ',' 
            << '<' << get<0>(get<1>(vector[i])) << ',' 
            << get<1>(get<1>(vector[i])) << '>' << '>' << ' ';
    }
    os << ']' << endl;

    return os;
}

// Sobrecarga del operador para imprimir un vector fila.
ostream& operator<<(ostream& os, const vector<double>& vector) {
    os << '[';
    for (unsigned int i = 0; i < vector.size(); i++) 
        os << setw(5) << vector[i] << ' ';
    os << ']' << endl;
    
    return os;
}

    /////////////////////////////////////////
    //        Funciones utilitarias        //
    /////////////////////////////////////////

//Agrega un vector como última columna de la matriz.
void push_back_column(vector<vector<double>>& matriz, const vector<double>& vector_b) {
    for (unsigned int i = 0; i < vector_b.size(); i++) 
        matriz[i].push_back(vector_b[i]);
}

    /////////////////////////////////////////
    //        Sistema de ecuaciones        //
    /////////////////////////////////////////

pair<vector<vector<double>>,vector<double>> armar_sistema(vector<double> p_interna, vector<double> p_externa, 
                                                         double r_in, double r_ex, int m) {

    // Seteamos parámetros que vamos a necesitar                              
    double pi = 2.0 * asin(1.0);
    int n = p_interna.size();
    double delta_r = (r_ex - r_in)/m;
    double delta_theta = (2.0*pi)/n;
    vector<double> radios = {};
    radios.push_back(r_in);
    for (double i = 1; i < m; i++) radios.push_back(r_in + delta_r*i);
    radios.push_back(r_ex);

    // Calculamos los coeficientes del sistema 
    double         valor_a  = 1/(pow(delta_r,2));
    vector<double> valor_b  = {};
    vector<double> valor_c  = {};
    for (int i = 0; i <= m; i++) {
        valor_b.push_back(1/(radios[i]*delta_r));
        valor_c.push_back(1/(pow(radios[i], 2) * pow(delta_theta, 2)));
    } 

    int incognitas = n*(m-1);   // Este cálculo surge de que las paredes del horno son datos, no incógnitas
    vector<vector<pair<double, pair<int,int>>>> sistema_previo = {};

    // Armamos el sistema de ecuaciones que cumplen con la ecuación de calor,
    // donde cada ecuación sería de la forma:
    // (a-b)*t(j-1,k) + (-2a+b-2c)*t(j,k) + a*t(j+1,k) + c*t(j,k-1) + c*t(j,k+1) = 0
    // Lo armamos de tal manera que luego resulte más facil armar la matriz banda para el sistema a resolver

    vector<pair<double, pair<int,int>>> fila = {};
    for (int j = 1; j < m; j++) {                                                                   // m: radios
        for (int k = 1; k <= n; k++) {                                                              // n: angulos    
            fila.push_back(make_pair(valor_a-valor_b[j], make_pair(j-1, k)));                       // (a-b)*t(j-1,k)
            fila.push_back(make_pair(-2.0*valor_a + valor_b[j] -2.0*valor_c[j], make_pair(j, k)));  // (-2a+b-2c)*t(j,k)
            fila.push_back(make_pair(valor_a, make_pair(j+1, k)));                                  // a*t(j+1,k)
            (k-1 == 0) ?
                fila.push_back(make_pair(valor_c[j], make_pair(j, n)))  :                           // c*t(j,k-1)
                fila.push_back(make_pair(valor_c[j], make_pair(j, k-1)));                           // c*t(j,k-1)
            (k+1 > n) ?
                fila.push_back(make_pair(valor_c[j], make_pair(j, 1)))  :                           // c*t(j,k+1)
                fila.push_back(make_pair(valor_c[j], make_pair(j, k+1)));                           // c*t(j,k+1)
            sistema_previo.push_back(fila);
            fila.clear();
        }
    }    

    // Luego armamos el sistema de ecuaciones
    // Este sistema es la matriz cuadrada que adicionalmente es banda, y será el que nos interesa resolver
    vector<vector<double>> sistema = vector<vector<double>>(incognitas,vector<double>(incognitas, 0));
    vector<double> vector_b = vector<double>(incognitas,0);

    // (cantidad_de_angulos) * (indice_del_radio - 1) + indice_del_angulo - 1 = posicion_en_sistema
    
    for (int i = 0; i < incognitas; i++) {              // incognitas: n*(m-1)
        for (int j = 0; j < 5; j++) {
            int radio_aux       = get<0>(get<1>(sistema_previo[i][j])); 
            int angulo_aux      = get<1>(get<1>(sistema_previo[i][j]));
            double coeficiente  = get<0>(sistema_previo[i][j]);
            if (radio_aux == 0) {
                vector_b[i] = coeficiente * p_interna[angulo_aux-1];
            } else if (radio_aux == m) {
                vector_b[i] = coeficiente * p_externa[angulo_aux-1];
            } else {
                sistema[i][n*(radio_aux-1) + angulo_aux - 1] = coeficiente; 
            }
        }
    }

    return make_pair(sistema,vector_b);
}

    /////////////////////////////////////////
    //           Armado del horno          //
    /////////////////////////////////////////

// Con el sistema resuelto, se calculan las temperaturas y se llena el horno con ellas
vector<vector<double>> calcular_temperaturas(vector<double> pared_interna, 
                                            vector<double> pared_externa,
                                            pair<vector<vector<double>>, vector<double>> sistema, 
                                            vector<double> vec_sol, 
                                            int n, int m) {
                                                
    int incognitas = get<0>(sistema).size();            //incognitas = n*(m-1)
    vector<double> temperaturas_aux = vector<double>(incognitas, 0);

    // Calculamos las temperaturas internas(sin pared interna/externa)
    for (int i = 0; i < incognitas; i++) {      
        for (int j = 0; j < incognitas; j++) {      
            // Dado que en la diagonal está la incognita de interés, la omitimos
            if (i != j) temperaturas_aux[i] += get<0>(sistema)[i][j]*vec_sol[j];
        }
        temperaturas_aux[i] -= get<1>(sistema)[i];      // Restamos el termino independiente
        temperaturas_aux[i] /= get<0>(sistema)[i][i];   // Dividimos por el coeficiente de la incógnita de interes
    }

    // Armamos las paredes del horno
    vector<vector<double>> horno = vector<vector<double>>(n, vector<double>(m+1, 0));
    for (int i = 0; i < n; i++) {
        horno[i][0]  = pared_interna[i]; 
        horno[i][m]  = pared_externa[i];
    }  

    // Armamos el interior del horno
    int aux = 0;
    for (int i = 1; i < m; i++) {                       // m -> radios
        for (int j = 0; j < n; j++) {                   // n -> angulos
            horno[j][i] = temperaturas_aux[aux];
            aux++;
        }
    }

    return horno;
}