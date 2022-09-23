#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o output_par.txt
#SBATCH -e errores.txt
./salida.out 512 2
./salida.out 512 4
./salida.out 512 8
./salida.out 1024 2
./salida.out 1024 4
./salida.out 1024 8
./salida.out 2048 2
./salida.out 2048 4
./salida.out 2048 8
./salida.out 4096 2
./salida.out 4096 4
./salida.out 4096 8