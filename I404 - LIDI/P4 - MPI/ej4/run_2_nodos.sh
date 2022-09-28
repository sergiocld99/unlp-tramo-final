#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=8
#SBATCH -o output_2_nodos.txt
#SBATCH -e errores.txt
mpirun matmul.out $1