#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=4
#SBATCH -o output.txt
#SBATCH -e errores.txt
mpirun mpi-simple.out