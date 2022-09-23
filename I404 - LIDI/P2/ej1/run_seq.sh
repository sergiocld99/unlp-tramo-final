#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o output_seq.txt
#SBATCH -e errores.txt
./seq.out 512 $1
./seq.out 1024 $1
./seq.out 2048 $1
./seq.out 4096 $1
