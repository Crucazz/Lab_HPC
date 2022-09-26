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
#include <math.h>
#include <omp.h>
#include "funciones.h"
#include "funciones2.h"


#define READ 0
#define WRITE 1
#define STDOUT 1

#define LARGO 1024

//Main

int main(int argc, char *argv[])
{
  
  
/////////////////////////////////////////////////////
//	INICIO:		Lectura de argumentos
/////////////////////////////////////////////////////

  int N=0,T=0,H=0,t=0;
  char * f = NULL;
  f=recibirArgumentos(argc, argv, &N, &T, f, &H, &t);

/////////////////////////////////////////////////////
//	FIN:		Lectura de argumentos
/////////////////////////////////////////////////////	

  printf("Los argumentos son -N %d -T %d -H %d y t %d\n",N,T,H,t);
  printf("La cadena es %s\n",f );
  printf("Hola %f", pow(2,4));
  holaMundo(2);
  holaMundoFunc1(3);


  
/////////////////////////////////////////////////////
//  INICIO:   LLenado inicial de la matriz
/////////////////////////////////////////////////////
  float *H1 = malloc(N*N*sizeof(float));
  float *H2 = malloc(N*N*sizeof(float));
  float *H3 = malloc(N*N*sizeof(float));

  #pragma omp parallel shared(H1,H2,H3) num_threads(H)
  {
    #pragma omp for schedule(static, H) collapse(2)
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        if( 0.4*N < i && 0.6*N > i && 0.4*N < j && 0.6*N > j) 
        {     
          H1[i*N+j]=20;
          H2[i*N+j]=20;
          H3[i*N+j]=20;      
        }
        else
        {
          H1[i*N+j]=0;
          H2[i*N+j]=0;
          H3[i*N+j]=0;
        }
      }
      
    }          
  }

  for (int i = 0; i < N; i++)
  {
    printf("\n");
    for (int j = 0; j < N; j++)
      printf(" %.2f ",H1[i*N+j]);   
  }
  printf("\n");
/////////////////////////////////////////////////////
//  FIN:   LLenado inicial de la matriz
///////////////////////////////////////////////////// 



  free(H1);

  //Parte paralalea
  int *A = (int *) malloc(LARGO*sizeof(int));

  for (int i = 0; i < LARGO; i++)
  {
    A[i] = (rand() % (6666 - 0 +1))+0;
  }

  int max = -1;
  for (int i = 0; i < LARGO; i++)
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