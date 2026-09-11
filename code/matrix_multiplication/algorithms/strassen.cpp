/*
 * Descripción: Es la implementación del algoritmo dividir y conquistar
 * de Strassen para la multiplicación de matrices. Además incluye funciones
 * de soporte para rellenar matrices con ceros cuandos sus dimensiones no son
 * potencias exactas de 2, así nos garantiza el caso base recursivo.
 * Referencia: Implementación base adaptada de GeeksforGeeks.
 * URL: https://www.geeksforgeeks.org/dsa/strassens-matrix-multiplication/
 */

#include <vector>
using namespace std;

using Matrix = vector<vector<int>>;

Matrix sumarMatrices(const Matrix& a, const Matrix& b){
    int n = static_cast<int>(a.size());
    Matrix res(n, vector<int>(n));
    for (int i =0; i < n; ++i)
        for(int j=0; j< n; ++j)
            res[i][j] = a[i][j] + b[i][j];
    return res;
}

Matrix restarMatrices(const Matrix& a, const Matrix& b){
    int n = static_cast<int>(a.size());
    Matrix res(n, vector<int>(n));
    for(int i = 0; i<n; ++i)
        for(int j=0; j<n; ++j)
            res[i][j] = a[i][j] - b[i][j];
    return res;
}

void dividirMatriz(const Matrix& mat, Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22){
    int newSize = static_cast<int>(a11.size());
    for(int i = 0; i < newSize; ++i){
        for(int j = 0; j < newSize; ++j){
            a11[i][j] = mat[i][j];
            a12[i][j] = mat[i][j + newSize];
            a21[i][j] = mat[i + newSize][j];
            a22[i][j] = mat[i + newSize][j + newSize];
        }
    }
}

Matrix combinarMatriz(const Matrix& c11, const Matrix& c12, const Matrix& c21, const Matrix& c22){
    int newSize = static_cast<int>(c11.size());
    int n = newSize * 2;
    Matrix res(n, vector<int>(n));
    for(int i = 0; i < newSize; ++i){
        for(int j = 0; j < newSize; ++j){
            res[i][j] = c11[i][j];
            res[i][j + newSize] = c12[i][j];
            res[i + newSize][j] = c21[i][j];
            res[i + newSize][j + newSize] = c22[i][j];
        }
    }
    return res;
}

Matrix strassenRecursivo(const Matrix& mat1, const Matrix& mat2){
    int n = static_cast<int>(mat1.size());

    if( n <= 64){ 
        Matrix res(n, vector<int>(n, 0));
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                for(int k = 0; k < n; ++k){
                    res[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
        return res;
    }

    int newSize = n/2;
    Matrix a11(newSize, vector<int>(newSize)), a12(newSize, vector<int>(newSize)),
           a21(newSize, vector<int>(newSize)), a22(newSize, vector<int>(newSize));
    Matrix b11(newSize, vector<int>(newSize)), b12(newSize, vector<int>(newSize)),
           b21(newSize, vector<int>(newSize)), b22(newSize, vector<int>(newSize));

    dividirMatriz(mat1, a11, a12, a21, a22);
    dividirMatriz(mat2, b11, b12, b21, b22);

    Matrix m1 = strassenRecursivo(sumarMatrices(a11, a22), sumarMatrices(b11, b22));
    Matrix m2 = strassenRecursivo(sumarMatrices(a21, a22), b11);
    Matrix m3 = strassenRecursivo(a11, restarMatrices(b12, b22));
    Matrix m4 = strassenRecursivo(a22, restarMatrices(b21, b11));
    Matrix m5 = strassenRecursivo(sumarMatrices(a11, a12), b22);
    Matrix m6 = strassenRecursivo(restarMatrices(a21, a11), sumarMatrices(b11, b12));
    Matrix m7 = strassenRecursivo(restarMatrices(a12, a22), sumarMatrices(b21, b22));

    Matrix c11 = sumarMatrices(restarMatrices(sumarMatrices(m1, m4), m5), m7);
    Matrix c12 = sumarMatrices(m3, m5);
    Matrix c21 = sumarMatrices(m2, m4);
    Matrix c22 = sumarMatrices(restarMatrices(sumarMatrices(m1, m3), m2), m6);
 
    return combinarMatriz(c11, c12, c21, c22);
}

int siguientePotencia(int n){
    int potencia = 1;
    while (potencia < n) potencia *= 2;
    return potencia;
}

Matrix rellenarCeros(const Matrix& mat, int size){
    int n= static_cast<int>(mat.size());
    Matrix resized(size, vector<int>(size, 0));
    for(int i = 0; i<n; ++i)
        for(int j = 0; j<n; ++j)
            resized[i][j] = mat[i][j];
    return resized;
}

vector<vector<int>> strassenMultiply(vector<vector<int>>& mat1, vector<vector<int>>& mat2){
    int n = static_cast<int>(mat1.size());
    int size =  siguientePotencia(n);

    Matrix cPad;
    if(size == n){
        cPad =  strassenRecursivo(mat1, mat2);
    } else {
        Matrix aPad = rellenarCeros(mat1, size);
        Matrix bPad =  rellenarCeros(mat2, size);
        cPad =  strassenRecursivo(aPad, bPad);
    }

    if(size == n){
        return cPad;
    }

    Matrix C(n, vector<int>(n));
    for(int i = 0; i<n; ++i)
        for(int j = 0; j< n; ++j)
            C[i][j] = cPad[i][j];
    return C;
}