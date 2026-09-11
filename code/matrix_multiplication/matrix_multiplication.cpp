/*
* Descripción: Es el programa principal que automatiza este entorno. Lee
* las matrices generadas en formato .txt, ejecuta los algoritmos Naive y Strassen,
* mide el tiempo de ejecución en microsegundos y el peak de consumo de memoria RAM (KB),
* exportando los resultados a un archivo CSV.
* Referencias: Las referencias más que nada son para entender ciertos
* elementos utilizados. 
* URL: https://www.geeksforgeeks.org/cpp/chrono-in-c/
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <filesystem>

#if defined(_WIN32)
    #include <windows.h>
    #include <psapi.h>
#else
    #include <sys/resource.h>
#endif

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

using Matrix = vector<vector<int>>;

Matrix naiveMultiply(Matrix& A, Matrix& B);
Matrix strassenMultiply(Matrix& A, Matrix& B);

Matrix leerMatriz(const string& rutaArchivo) {
    Matrix matriz;
    ifstream archivo(rutaArchivo);
    string linea;
    while (getline(archivo, linea)) {
        vector<int> fila;
        stringstream ss(linea);
        int numero;
        while (ss >> numero) {
            fila.push_back(numero);
        }
        if (!fila.empty()) {
            matriz.push_back(fila);
        }
    }
    return matriz;
}

void guardarMatriz(const string& rutaArchivo, const Matrix& matriz) {
    ofstream archivo(rutaArchivo);
    for (const auto& fila : matriz) {
        for (size_t i = 0; i < fila.size(); ++i) {
            archivo << fila[i] << (i == fila.size() - 1 ? "" : " ");
        }
        archivo << "\n";
    }
}

long obtenerMemoriaKB(){
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return static_cast<long>(info.PeakWorkingSetSize / 1024);
#elif defined(__APPLE__)
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return static_cast<long>(uso.ru_maxrss / 1024);
#else
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return uso.ru_maxrss;
#endif
}

void guardarMedicion(const string& algoritmo, const string& nombreBase, long long microsegundos, long memoriaKB){
    stringstream ss(nombreBase);
    string n, t, d, m;
    getline(ss, n, '_');
    getline(ss, t, '_');
    getline(ss, d, '_');
    getline(ss, m, '_');

    string rutaCSV = "data/measurements/matrix_results.csv";
    bool existeArchivo = fs::exists(rutaCSV);

    ofstream archivo(rutaCSV, ios::app);
    if(!existeArchivo){
       archivo << "algoritmo,n,t,d,m,tiempo_us,memoria_kb\n"; 
    }
    archivo << algoritmo << "," << n << "," << t << "," << d << ","
            << m << "," << microsegundos << "," << memoriaKB << "\n";
}

vector<vector<int>> naive(const vector<vector<int>>& A, const vector<vector<int>>& B, int n);
vector<vector<int>> strassen(const vector<vector<int>>& A, const vector<vector<int>>& B, int n);

int main(int argc, char* argv[]) {
    if(argc < 3){
        cout << "Uso: ./matrix_multiplication <archivo_base> <algoritmo>" << endl;
        cout << "Algoritmos disponibles: naive, strassen" << endl;
        return 1;
    }

    string baseName = argv[1];
    string algoritmo = argv[2];

    string fileA = "data/matrix_input/" + baseName + "_1.txt";
    string fileB = "data/matrix_input/" + baseName + "_2.txt";

    vector<vector<int>> A = leerMatriz(fileA);
    vector<vector<int>> B = leerMatriz(fileB);

    if (A.empty() || B.empty()) {
        cerr << "Error: no se pudieron leer las matrices base " << baseName << endl;
        return 1;
    }

    fs::create_directories("data/matrix_output");
    fs::create_directories("data/measurements");

    int n = A.size();
    vector<vector<int>> C;

    auto inicio = steady_clock::now();
    
    if (algoritmo == "naive") {
        C = naiveMultiply(A, B);
    } else if (algoritmo == "strassen") {
        C = strassenMultiply(A, B);
    } else {
        cerr << "Algoritmo no reconocido. Usa: naive, strassen" << endl;
        return 1;
    }

    auto fin = steady_clock::now();
    auto duracion = duration_cast<microseconds>(fin - inicio).count();
    long memoria = obtenerMemoriaKB();

    guardarMatriz("data/matrix_output/" + baseName + "_out.txt", C);
    guardarMedicion(algoritmo, baseName, duracion, memoria);
    
    cout << algoritmo << " -> tiempo: " << duracion << " us, memoria: " << memoria << " KB" << endl;
    
    return 0;
}