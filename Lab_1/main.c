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


  
/////////////////////////////////////////////////////
//  INICIO:   LLenado inicial de la matriz
/////////////////////////////////////////////////////
  float *HImpresion = malloc(N*N*sizeof(float));
  
  float *H1 = malloc(N*N*sizeof(float));
  float *H2 = malloc(N*N*sizeof(float));
  float *HAUX = malloc(N*N*sizeof(float));

  #pragma omp parallel shared(H1,H2) num_threads(H)
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
          HAUX[i*N+j]=20;
        }
        else
        {
          H1[i*N+j]=0;
          H2[i*N+j]=0;
          HAUX[i*N+j]=0;
        }
      }
      
    }          
  }
/*
  for (int i = 0; i < N; i++)
  {
    printf("\n");
    for (int j = 0; j < N; j++)
      printf(" %.2f ",H1[i*N+j]);   
  }
  printf("\n");
  */
/////////////////////////////////////////////////////
//  FIN:   LLenado inicial de la matriz
///////////////////////////////////////////////////// 


/*
H1 = t
H2 = t-2
HAUX = t-1
*/
/////////////////////////////////////////////////////
//  INICIO:   Calculo de ecuacion
///////////////////////////////////////////////////// 


float c=1.0 , dt=0.1 , dd=0.2;
float iMenos1,iMas1,jMenos1,jMas1;
int contador = 1;
int id_imagen = 1;

while( contador <= T)
{

  //Copia lo que esta en H1 en HAUX
  swap(H1,HAUX,N);
  //caso inicial
  if( contador ==1)
  {
    //printf("Se calcula el caso base: %d \n",contador);

    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        iMenos1=0;
        iMas1=0;
        jMenos1=0;
        jMas1=0;
        if(i-1>=0)
          iMenos1=HAUX[(i-1)*N+j];
        if(j-1>=0)
          jMenos1=HAUX[i*N+(j-1)];
        if(i+1<N)
          iMas1=HAUX[(i+1)*N+j];
        if(j+1<N)
          jMas1=HAUX[i*N+(j+1)];

        H1[i*N+j]= HAUX[i*N+j]+ (pow(c,2)/2)*pow(dt/dd,2)*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]);
      }
      
    }  
  }
  else //caso normal
  {

    //printf("Se calcula el caso normal: %d \n",contador);

    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        iMenos1=0;
        iMas1=0;
        jMenos1=0;
        jMas1=0;
        if(i-1>=0)
          iMenos1=HAUX[(i-1)*N+j];
        if(j-1>=0)
          jMenos1=HAUX[i*N+(j-1)];
        if(i+1<=N)
          iMas1=HAUX[(i+1)*N+j];
        if(j+1<=N)
          jMas1=HAUX[i*N+(j+1)];
        H1[i*N+j]= 2*HAUX[i*N+j]-H2[i*N+j] +(pow(c,2))*(pow(dt/dd,2))*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]);
      }
      
    }

  }
  //Copia lo que esta en HAUX en H2
  swap(HAUX,H2,N);
  //Se genera un documento (SECCION CRITICA)
  if(contador%t==0)
  {
    printf(" Se imprime la numero %d \n",contador);
    /*for (int i = 0; i < N; i++)
      {
        printf("\n");
        for (int j = 0; j < N; j++)
          printf(" %.2f ",H1[i*N+j]);   
      }
      printf("\n\n\n\n");
    */
    //printf("\nSE GENERA NUEVA IMAGEN");
    id_imagen++;
  }
  contador++;

}

/////////////////////////////////////////////////////
//  FIN:   Calculo de ecuacion
///////////////////////////////////////////////////// 

  

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
      //printf("El maximo cambio a %d\n",max);
    }

  }

  //printf("El mazimo paarlelo es: %d \n",max);
  int gmax = -1;
  #pragma omp parallel shared(gmax) num_threads(2)
  {
    int maxi = -1;

    #pragma omp for schedule(static, 2)
      for (int i = 0; i < N; i++)
      {
        if(A[i] >maxi)
        {
          //printf("El maximo cambio de %d a %d\n",maxi,A[i]);
          maxi = A[i];
          
        }
      }

    #pragma omp critical
      if (maxi > gmax)
        gmax=max;

  }







  //Creacion de prueba de archivo

/*
  printf("El tamaño de N es: %d",N);
  saveImg(HImpresion,H1,N);


  for (int i = 0; i < N; i++)
      {
        printf("\n");
        for (int j = 0; j < N; j++)
          printf(" %.2f ",H1[i*N+j]);   
      }
      printf("\n\n\n\n");

  printf("El tamaño de N es: %d",N);
  N=N-2;
  for (int i = 0; i < N; i++)
  {
    printf("\n");
    for (int j = 0; j < N; j++)
      printf(" %.2f ",HImpresion[i*N+j]);   
  }
  printf("\n");

  */
  printf("\nEl tamaño de N es: %d",N);
  FILE *f1 = fopen("ejemplo.raw", "w");
  fwrite(H1, sizeof(float), N*N, f1);
  fclose(f1);


  free(H1);
  free(H2);
  free(HAUX);
  free(HImpresion);


  printf("\nEl maximo con omp es: %d\n",gmax);

  printf("\nValores de T=%d, t=%d\n",T,t);

  return 0;
}