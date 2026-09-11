/*
 * Descripción: Es la implementación del algoritmo estándar (Naive) para la
 * multiplicación de matrices cuadradas con complejidad O(n^3). Se encuentra 
 * optimizada, se alteró el orden de los cilcos a i-k-j para que sea más amigable con la 
 * memoria caché .
 * Referencia: Estas referencias son más o menos teoricas para entender el orden 
 * que utilicé para la matriz, también utilicé el libro "Algorithms" entregado por las referencias.
 * URL: https://courses.grainger.illinois.edu/cs232/sp2009/lectures/X18.pdf
*/

#include <vector>

using namespace std;

vector<vector<int>> naiveMultiply(vector<vector<int>>& A, vector<vector<int>>& B){
    int n = static_cast<int>(A.size());
    vector<vector<int>> C(n, vector<int>(n, 0));

    for(int i = 0; i<n; ++i){
        for(int k = 0; k<n; ++k){
            int valorA= A[i][k];
            for(int j=0; j<n; ++j){
                C[i][j] += valorA * B[k][j];
            }
        }
    }

    return C;
}