"""
Descripción: Script para automatizar la generación de gráficos de rendimiento 
(tiempo de ejecución y peak de memoria) a partir de los datos en formato CSV.
Se utiliza escala logarítmica base 2 para el eje X y se promedian las muestras.
"""
import pandas as pd
import matplotlib.pyplot as plt
import os
import gc

ruta_csv = 'data/measurements/matrix_results.csv'
ruta_plots = 'data/plots'

os.makedirs(ruta_plots, exist_ok=True)

def generar_graficos():
    try:
        df = pd.read_csv(ruta_csv)
        
        df_promedio = df.groupby(['algoritmo', 't', 'n']).mean(numeric_only=True).reset_index()
        
        algoritmos = df_promedio['algoritmo'].unique()
        
        colores = {'dispersa': 'blue', 'diagonal': 'green', 'densa': 'red'}
        marcadores = {'dispersa': 'o', 'diagonal': 's', 'densa': '^'}
        
        for alg in algoritmos:
            df_alg = df_promedio[df_promedio['algoritmo'] == alg]
            tipos = df_alg['t'].unique()
            
            fig, ax = plt.subplots()
            
            for t in tipos:
                df_t = df_alg[df_alg['t'] == t].sort_values(by='n')
                
                ax.plot(df_t['n'], df_t['tiempo_us'], 
                         color=colores.get(t, 'black'), 
                         linestyle='-', 
                         marker=marcadores.get(t, 'o'), 
                         label=t)
            
            ax.set_xscale('log', base=2)
            ax.set_yscale('log')
            ax.set_title(f'Tiempo Promedio vs N: {alg}')
            ax.set_xlabel('Dimensión de la Matriz (N)')
            ax.set_ylabel('Tiempo (us)')
            ax.legend()
            
            fig.savefig(f'{ruta_plots}/tiempo_{alg}.png')
            plt.close(fig) 

            fig, ax = plt.subplots()
            
            for t in tipos:
                df_t = df_alg[df_alg['t'] == t].sort_values(by='n')
                
                ax.plot(df_t['n'], df_t['memoria_kb'], 
                         color=colores.get(t, 'black'), 
                         linestyle='-', 
                         marker=marcadores.get(t, 'o'), 
                         label=t)
            
            ax.set_xscale('log', base=2)
            ax.set_yscale('log')
            ax.set_title(f'Memoria Promedio vs N: {alg}')
            ax.set_xlabel('Dimensión de la Matriz (N)')
            ax.set_ylabel('Memoria (KB)')
            ax.legend()
            
            fig.savefig(f'{ruta_plots}/memoria_{alg}.png')
            plt.close(fig)
            
            gc.collect()
            
        print("Gráficos generados exitosamente.")

    except FileNotFoundError:
        print(f"Error: No se encontró el archivo {ruta_csv}.")

if __name__ == "__main__":
    generar_graficos()