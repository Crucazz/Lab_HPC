//para compilar:
//make
//para ejecutar
//./wave [-N numero entero] [-x numero entero] [-y numero entero] [-T numero entero] [-f cadena caracteres]
//./wave -N 30 -x 5 -y 5 -T 1 -f imagen.raw
extern "C" {
  

#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
}

__global__ void wave(float *H1, float *H2, float*HAUX, int contador, int N)
{
  float c=1.0 , dt=0.1 , dd=2.0;
  float iMenos1,iMas1,jMenos1,jMas1;
  int j, i;
  i = blockDim.x*blockIdx.x + threadIdx.x;  // global index x (horizontal)
  j = blockDim.y*blockIdx.y + threadIdx.y;  // global index y (vertical)
  
  //caso inicial
  if( contador ==1)
  {
    iMenos1=HAUX[(i-1)*N+j];
    jMenos1=HAUX[i*N+(j-1)];
    iMas1=HAUX[(i+1)*N+j];
    jMas1=HAUX[i*N+(j+1)];

    H1[i*N+j]= HAUX[i*N+j]+ (c*c)*((dt/dd)*(dt/dd))*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]); 
  }
  else //caso normal
  {     
    iMenos1=HAUX[(i-1)*N+j];
    jMenos1=HAUX[i*N+(j-1)];
    iMas1=HAUX[(i+1)*N+j];
    jMas1=HAUX[i*N+(j+1)];

    H1[i*N+j]= 2*HAUX[i*N+j]-H2[i*N+j] +(c*c)*((dt/dd)*(dt/dd))*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]);

  }

  return H1;
    
}



//Main

__host__ int main(int argc, char *argv[])
{
/////////////////////////////////////////////////////
//  INICIO:   Lectura de argumentos
/////////////////////////////////////////////////////


  int N=0,T=0, x=0, y=0;
  char * f = NULL;
  f=recibirArgumentos(argc, argv, &N, &T, f, &x, &y);

/////////////////////////////////////////////////////
//  FIN:    Lectura de argumentos
///////////////////////////////////////////////////// 


  
/////////////////////////////////////////////////////
//  INICIO:   LLenado inicial de la matriz de host
/////////////////////////////////////////////////////
  
  float *H1 = (float *) malloc(N*N*sizeof(float));
  float *H2 = (float *) malloc(N*N*sizeof(float));
  float *HAUX = (float *) malloc(N*N*sizeof(float));
  

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
  
/////////////////////////////////////////////////////
//  FIN:   LLenado inicial de la matriz host
///////////////////////////////////////////////////// 

/////////////////////////////////////////////////////
//  INICIO:   Asignacion de memoria Device
/////////////////////////////////////////////////////

  float *d_H1;
  float *d_H2;
  float *d_HAUX;

  //Se asigna memoria para cada vector del device
  cudaMalloc( &d_H1, N*N*sizeof(float));
  cudaMalloc( &d_H2, N*N*sizeof(float));
  cudaMalloc( &d_HAUX, N*N*sizeof(float));

  //Se copia el vector del host al vector del device
  cudaMemcpy(d_H1, H1, N*N*sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_H2, H2, N*N*sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_HAUX, HAUX, N*N*sizeof(float), cudaMemcpyHostToDevice);

  /////////////////////////////////////////////////////
//  FIN:   Asignacion de memoria Device
/////////////////////////////////////////////////////


/////////////////////////////////////////////////////
//  INICIO:   Calculo de ecuacion
///////////////////////////////////////////////////// 

  int contador = 1;

  dim3 blocksize;
  dim3 gridsize;

  //Numero de threads en cada bloque

  gridsize.x = N/x;
  gridsize.y = N/y;
  blocksize.x = x;
  blocksize.y = y;



  //Se ejecuta el kernel
  

  while( contador <= T)
  {
    swap(H1,HAUX,N);
    cudaMemcpy(d_H1, H1, N*N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_H2, H2, N*N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_HAUX, HAUX, N*N*sizeof(float), cudaMemcpyHostToDevice);
    wave<<<gridsize, blocksize>>>(d_H1, d_H2, d_HAUX, contador, N);    
    cudaMemcpy(d_H1, H1, N*N*sizeof(double), cudaMemcpyDeviceToHost);
    swap(HAUX,H2,N);    
    contador++;
    
    
  }      

/////////////////////////////////////////////////////
//  FIN:   Calculo de ecuacion
///////////////////////////////////////////////////// 

  

  FILE *f1 = fopen(f, "w");
  fwrite(H1, sizeof(float), N*N, f1);
  fclose(f1);

  cudaFree(d_H1);
  cudaFree(d_H2);
  cudaFree(d_HAUX);

  free(H1);
  free(H2);
  free(HAUX);

  return 0;
}