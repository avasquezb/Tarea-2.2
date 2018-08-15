#include<stdio.h>
#include<mpi.h>
#include<fstream>
#include <iostream>
using namespace std;


int LenArchivo(char *text)
{
	int cont=-1;
	int lim[2];
	string lin;
	ifstream archivo(text);
	MPI_Recv(lim, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    archivo.seekg(lim[0]);
	while(lim[1]>=archivo.tellg() && archivo.tellg()!=-1)
		{
			getline(archivo,lin);
			cont++;
		}
	archivo.close();
	return cont;
}
int main(int argc, char* argv[]){

  	int procesadores, rank;
  	int Tot=-1;
  	int fin,cant_proc,aux,lim[2];
  	MPI_Init(&argc,&argv);
  	MPI_Comm_size(MPI_COMM_WORLD, &procesadores);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank==0)
	{
		int Tot=-1;
		int lim[2], cant_proc,fin,aux;
		string lin;
		ifstream archivo(argv[1]);
		archivo.seekg(0, archivo.end); 
		fin = archivo.tellg();
		cant_proc=fin/procesadores;
		if(fin%procesadores==0){
			Tot--;
		} 
		archivo.seekg(0);
		for(int i=1;i<procesadores;i++) 
		{
	    	lim[0]=cant_proc*i;
	    	lim[1]=cant_proc*(i+1);
	    	MPI_Send(lim, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
    	while(cant_proc>=archivo.tellg()) 
		{
			getline(archivo,lin);
			Tot++;
		}
		cout<<"Procesador 0 = "<<Tot<<endl;
		for(int i=1;i<procesadores;i++)
		{
			MPI_Recv(&aux, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		 	cout<<"Procesador "<<i<<" = "<<aux<<endl;
		 	Tot=aux+Tot;
		}
		cout<<"Cantidad Total = "<<Tot<<endl;
    	archivo.close();
	}
	else
	{
		aux=LenArchivo(argv[1]);
		MPI_Send(&aux, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
  MPI_Finalize();
}