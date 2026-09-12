"""
Descripción: Script para automatizar la generación de gráficos de rendimiento 
(tiempo de ejecución y peak de memoria) de los algoritmos de ordenamiento.
Se separan los resultados según la distribución inicial de los arreglos 
(aleatoria, ascendente, descendente).
"""

import pandas as pd
import matplotlib.pyplot as plt
import os
import gc

ruta_csv = 'data/measurements/sorting_results.csv'
ruta_plots = 'data/plots'

os.makedirs(ruta_plots, exist_ok=True)

def generar_graficos():
    try:
        df = pd.read_csv(ruta_csv)
        
        df_promedio = df.groupby(['algoritmo', 't', 'n']).mean(numeric_only=True).reset_index()
        
        algoritmos = df_promedio['algoritmo'].unique()
        
        colores = {'ascendente': 'blue', 'descendente': 'red', 'aleatorio': 'green'}
        estilos_linea = {'ascendente': '-', 'descendente': '-', 'aleatorio': '-'}
        marcadores = {'ascendente': 'o', 'descendente': 's', 'aleatorio': '^'}
        
        for alg in algoritmos:
            df_alg = df_promedio[df_promedio['algoritmo'] == alg]
            tipos = df_alg['t'].unique()
            
            fig, ax = plt.subplots()
            
            for t in tipos:
                df_t = df_alg[df_alg['t'] == t].sort_values(by='n')
                
                ax.plot(df_t['n'], df_t['tiempo_us'], 
                         color=colores.get(t, 'black'), 
                         linestyle=estilos_linea.get(t, '-'), 
                         marker=marcadores.get(t, 'o'), 
                         label=t)
            
            ax.set_xscale('log')
            ax.set_yscale('log')
            ax.set_title(f'Tiempo Promedio vs N: {alg}')
            ax.set_xlabel('Tamaño (N)')
            ax.set_ylabel('Tiempo (us)')
            ax.legend()
            
            fig.savefig(f'{ruta_plots}/tiempo_{alg}.png')
            plt.close(fig)

            fig, ax = plt.subplots()
            
            for t in tipos:
                df_t = df_alg[df_alg['t'] == t].sort_values(by='n')
                
                ax.plot(df_t['n'], df_t['memoria_kb'], 
                         color=colores.get(t, 'black'), 
                         linestyle=estilos_linea.get(t, '-'), 
                         marker=marcadores.get(t, 'o'), 
                         label=t)
            
            ax.set_xscale('log')
            ax.set_yscale('log')
            ax.set_title(f'Memoria Promedio vs N: {alg}')
            ax.set_xlabel('Tamaño (N)')
            ax.set_ylabel('Memoria (KB)')
            ax.legend()
            
            fig.savefig(f'{ruta_plots}/memoria_{alg}.png')
            plt.close(fig)
            
            gc.collect()
            
        print("Gráficos generados exitosamente.")

    except FileNotFoundError:
        print(f"Error: No se encontró {ruta_csv}.")

if __name__ == "__main__":
    generar_graficos()
