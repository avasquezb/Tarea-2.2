#include <iostream>
#include <fstream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]){
	int rank,procesadores,contador=0;
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &procesadores);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int contadorxproc [procesadores];
	int lin = 0;
	int cant_tot = 0;
	for(int j=0;j<procesadores;j++)
	{
		contadorxproc[j]=0;
	}
	ifstream file(argv[1]);
	string linea;
	MPI_Status status;
	while (getline(file,linea))
	{
		lin++;
		for (int i=0;i<procesadores;i++)
		{
			contadorxproc[i]++;
			MPI_Send(&lin, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		    MPI_Recv(&lin, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	cout<<"Cantidad total de lineas = "<<lin-1<<endl;
	for (int z=0;z<procesadores;z++)
	{
		cout<<"Procesador "<<z<<" = "<<contadorxproc[z]-1<<endl;
	}
	MPI_Finalize();
}