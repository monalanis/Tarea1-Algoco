/*
 * Descripción: Programa principal que automatiza este entorno de para los algoritmos
 * de ordenamiento. Lee los arreglos generados, ejecuta Standard Sort, Merge Sort, 
 * Quick Sort y Patience Sort. Mide el tiempo de ejecución en microsegundos y el 
 * peak de consumo de memoria RAM (KB) de forma multiplataforma, exportando los 
 * resultados a un archivo CSV.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
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

vector<int> sortArray(vector<int>& arr);
void mergeSort(vector<int>& arr, int left, int right);
void quickSort(vector<int>& arr, int low, int high);
vector<int> patienceSort(vector<int>& arr);

vector<int> leerArreglo(const string& rutaArchivo){
    vector<int> arr;
    ifstream archivo(rutaArchivo);
    int numero;
    while (archivo >> numero){
        arr.push_back(numero);
    }
    return arr;
}

void guardarArreglo(const string& rutaArchivo, const vector<int>& arr){
    ofstream archivo(rutaArchivo);
    for(int num : arr){
        archivo << num << "\n";
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

    string rutaCSV = "data/measurements/sorting_results.csv";
    bool existeArchivo = fs::exists(rutaCSV);

    ofstream archivo(rutaCSV, ios::app);
    if(!existeArchivo){
       archivo << "algoritmo,n,t,d,m,tiempo_us,memoria_kb\n"; 
    }
    archivo << algoritmo << "," << n << "," << t << "," << d << ","
            << m << "," << microsegundos << "," << memoriaKB << "\n";
}

int main(int argc, char* argv[]){
    if(argc < 3){
        cout << "Uso: ./sorting <archivo_entrada.txt> <algoritmo>" << endl;
        cout << "Algoritmos disponibles: stdsort, mergesort, quicksort, patiencesort" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string algoritmo = argv[2];

    size_t lastSlash = inputFile.find_last_of('/');
    size_t dotPos = inputFile.find_last_of('.');
    string baseName = inputFile.substr(
        lastSlash == string::npos ? 0 : lastSlash + 1,
        dotPos == string::npos ? inputFile.length() : dotPos - (lastSlash == string::npos ? 0 : lastSlash + 1));

    vector<int> arr = leerArreglo(inputFile);
    if (arr.empty()) {
        cerr << "Error: no se pudo leer el arreglo desde " << inputFile << endl;
        return 1;
    }

    fs::create_directories("data/array_output");
    fs::create_directories("data/measurements");

    auto inicio = steady_clock::now();
    
    if (algoritmo == "stdsort") {
        arr = sortArray(arr);
    } else if (algoritmo == "mergesort") {
        mergeSort(arr, 0, static_cast<int>(arr.size()) - 1);
    } else if (algoritmo == "quicksort") {
        quickSort(arr, 0, static_cast<int>(arr.size()) - 1);
    } else if (algoritmo == "patiencesort") {
        arr = patienceSort(arr);
    } else {
        cerr << "Algoritmo no reconocido. Usa: stdsort, mergesort, quicksort, patiencesort" << endl;
        return 1;
    }

    auto fin = steady_clock::now();
    auto duracion = duration_cast<microseconds>(fin - inicio).count();
    long memoria = obtenerMemoriaKB();

    guardarArreglo("data/array_output/" + baseName + "_out.txt", arr);
    guardarMedicion(algoritmo, baseName, duracion, memoria);
    
    cout << algoritmo << " -> tiempo: " << duracion << " us, memoria: " << memoria << " KB" << endl;
    
    return 0;
}