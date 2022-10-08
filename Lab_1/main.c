
//para compilar:
//make
//para ejecutar
//./wave [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "funciones.h"
#include "funciones2.h"
#include <time.h>


#define READ 0
#define WRITE 1
#define STDOUT 1

#define LARGO 1024

//Main

int main(int argc, char *argv[])
{
  
  clock_t start2, end2;
  double cpu_time_used;
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
      for (int j = 0; j < N; j++)
      {
        printf(" %2.f ",HAUX[i*N+j]);
      }
      printf("\n");
    }  
*/

/////////////////////////////////////////////////////
//  FIN:   LLenado inicial de la matriz
///////////////////////////////////////////////////// 


/////////////////////////////////////////////////////
//  INICIO:   Calculo de ecuacion
///////////////////////////////////////////////////// 


float c=1.0 , dt=0.1 , dd=2.0;
float iMenos1,iMas1,jMenos1,jMas1;
int contador = 1;

double start; 
double end; 
start2 = clock();
start = omp_get_wtime();

#pragma omp parallel shared(H1,HAUX,H2,contador,T) private(jMas1,jMenos1,iMenos1,iMas1) num_threads(H)
{
  
  //printf("Hilos %d", omp_get_num_threads());
  while( contador <= T)
  {
    //Copia lo que esta en H1 en HAUX
    #pragma omp single
    {
      swap(H1,HAUX,N);
    }
    //caso inicial
    if( contador ==1)
    {
    
      #pragma omp for schedule(static, H) collapse(2)
      for (int i = 1; i < N-1; i++)
      {
        for (int j = 1; j < N-1; j++)
        {
          //printf("Pos[%d, %d] \n",i,j);
          iMenos1=HAUX[(i-1)*N+j];
          jMenos1=HAUX[i*N+(j-1)];
          iMas1=HAUX[(i+1)*N+j];
          jMas1=HAUX[i*N+(j+1)];

          H1[i*N+j]= HAUX[i*N+j]+ (c*c)*((dt/dd)*(dt/dd))*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]);
        }
      }  
    }
    else //caso normal
    {
      #pragma omp for schedule(static, H) collapse(2)
      for (int i = 1; i < N-1; i++)
      {
        for (int j = 1; j < N-1; j++)
        {
          iMenos1=HAUX[(i-1)*N+j];
          jMenos1=HAUX[i*N+(j-1)];
          iMas1=HAUX[(i+1)*N+j];
          jMas1=HAUX[i*N+(j+1)];

          H1[i*N+j]= 2*HAUX[i*N+j]-H2[i*N+j] +(c*c)*((dt/dd)*(dt/dd))*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]);
        }
        
      }

    } 
    #pragma omp single
    {
      swap(HAUX,H2,N);
      contador++;
    }
    
    
  }      
}

end = omp_get_wtime();
end2 = clock();
cpu_time_used = ((double) (end2 - start2))/1000000;
printf("Work took %f seconds\n", end - start);
printf("Tiempo %f \n",cpu_time_used);
/////////////////////////////////////////////////////
//  FIN:   Calculo de ecuacion
///////////////////////////////////////////////////// 

  


  printf("\nEl tamaño de N es: %d",N);
  FILE *f1 = fopen("ejemplo.raw", "w");
  fwrite(H1, sizeof(float), N*N, f1);
  fclose(f1);

  free(H1);
  free(H2);
  free(HAUX);
  free(HImpresion);


  printf("\nValores de T=%d, t=%d\n",T,t);

  return 0;
}