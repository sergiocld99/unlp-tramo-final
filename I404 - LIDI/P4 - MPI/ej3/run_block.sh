#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --tasks-per-node=4
#SBATCH -o output4_block.txt
#SBATCH -e errores.txt
mpirun blocking-ring.out $1