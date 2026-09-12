# Documentación y Guía de Ejecución

## Entrega
La entrega se realiza vía **aula.usm.cl** en formato `.zip`.

---

## 1. Multiplicación de Matrices

Todos los archivos relacionados con este problema se encuentran dentro de la carpeta `code/matrix_multiplication/`. Para probar el código, primero debe navegar a ese directorio desde la terminal:

```bash
cd code/matrix_multiplication/

### Generación de Datos de Prueba
Se deben generar los archivos ejecutando el script de python estrcitamente desde dentro de la carpeta scripts/

cd scripts/
python3 matrix_generator.py
cd ..

### Programa Principal(Compilación)
Regresando a code/matrix_multiplication/ utiliza el makefile

    make clean
    make
```

## Ejecución de Pruebas
Esta implementación aísla el proceso en el sistema operativo mediante un bucle bash. Si los algoritmos se ejecutaran secuencialmente en un mismo flujo, la alta carga de memoria dinámica y recursividad del algoritmo de Strassen establecería un peak histórico de RAM que oculta el bajo consumo del algoritmo Naive.

for archivo in data/matrix_input/*_1.txt; do
    base=$(basename "$archivo" _1.txt)
    for alg in naive strassen; do
        ./matrix_multiplication "$base" "$alg"
    done
done

## Generación de Gráficos

python3 scripts/plot_generator.py


### Ordenamiento de arreglo unidimensional
Todos los archivos relacionados con este problema se encuentran dentro de la carpeta `code/sorting/`. Para probar el código, primero debe navegar a ese directorio desde la terminal:

```bash
cd code/sorting/


### Generación de Datos de Prueba (SORTING)
Los arreglos con distintas distribuciones deben ser generados antes de ejecutar las pruebas. Ejecuta el script estrictamente desde la carpeta scripts/

cd scripts/
python3 array_generator.py
cd ..

### Programa principal (Compilación)
Regresando a code/sorting/ utilice el makefile

make clean
make
```

### Ejecución de Pruebas
Al igual que las matrices, esta implementación aísla los procesos para que la API nativa del sistema operativo mida correctamente el peak histórico de memoria RAM sin que el consumo estructural de un algoritmo (como MergeSort) enmascare a los algortimos in-place  (como QuickSort)

for archivo in data/array_input/*.txt; do
    for alg in stdsort mergesort quicksort patiencesort; do
        ./sorting "$archivo" "$alg"
    done
done

### Generación de Gráficos
Genere los graficos desde el directorio raíz del problema

python3 scripts/plot_generator.py
