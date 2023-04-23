# Programación en memoria distribuida
Para configurar MPI es necesario contar con un compilador de C/C++: ```sudo apt-get install g++```

Luego para mantener las variables de entorno (mpicc y mpirun), agregu /etc/profile:
```
export PATH="$PATH:/usr/local/openmpi/bin"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/openmpi/lib"¸
```
