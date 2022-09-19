//para instalar la libreria en linux:
//sudo apt-get install libjpeg8-dev 
//
//para compilar:
//make
//para ejecutar
//./pipeline [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]


#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>
#include "funciones.h"
#include "funciones2.h"


#define READ 0
#define WRITE 1
#define STDOUT 1

#define N 1024

//Main

int main(int argc, char *argv[])
{
  
  
/////////////////////////////////////////////////////
//	INICIO:		Lectura de argumentos
/////////////////////////////////////////////////////

  int c=0,u=0,n=0,flag=0;
  char * m = NULL;
  m=recibirArgumentos(argc, argv, &c, &u, m, &n, &flag);

/////////////////////////////////////////////////////
//	FIN:		Lectura de argumentos
/////////////////////////////////////////////////////	

  printf("Los argumentos son -c %d -u %d -n %d y flag %d\n",c,u,n,flag);
  printf("La cadena es %s\n",m );
  holaMundo(2);
  holaMundoFunc1(3);

  //Parte paralalea
  int *A = (int *) malloc(N*sizeof(int));

  for (int i = 0; i < N; i++)
  {
    A[i] = (rand() % (6666 - 0 +1))+0;
  }

  int max = -1;
  for (int i = 0; i < N; i++)
  {
    if(A[i]>max){
      max = A[i];
      printf("El maximo cambio a %d\n",max);
    }

  }

  printf("El mazimo paarlelo es: %d \n",max);
  int gmax = -1;
  #pragma omp parallel shared(gmax) num_threads(2)
  {
    int maxi = -1;

    #pragma omp for schedule(static, 2)
      for (int i = 0; i < N; i++)
      {
        if(A[i] >maxi)
        {
          printf("El maximo cambio de %d a %d\n",maxi,A[i]);
          maxi = A[i];
          
        }
      }

    #pragma omp critical
      if (maxi > gmax)
        gmax=max;

  }

  printf("El maximo con omp es: %d\n",gmax);

  return 0;
}