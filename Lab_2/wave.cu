//para compilar:
//make
//para ejecutar
//./wave [-N numero entero] [-x numero entero] [-y numero entero] [-T numero entero] [-f cadena caracteres]
//./wave -N 30 -x 5 -y 5 -T 1 -f imagen.raw
extern "C" {
  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funciones.h"
}

__global__ void wave(float *H1, float *H2, float*HAUX, int N, int contador)
{
  float c=1.0 , dt=0.1 , dd=2.0;
  float iMenos1,iMas1,jMenos1,jMas1;
  int j, i;
  extern __shared__ float temp[];

  i = blockDim.x*blockIdx.x + threadIdx.x;  // global index x (horizontal)
  j = blockDim.y*blockIdx.y + threadIdx.y;  // global index y (vertical)
    //caso inicial
  if (i >1 && i<N-1 && j>1 && j<N-1 )
  {  
    if( contador ==1)
    {
      iMenos1=HAUX[(i-1)*N+j];
      jMenos1=HAUX[i*N+(j-1)];
      iMas1=HAUX[(i+1)*N+j];
      jMas1=HAUX[i*N+(j+1)];

      temp[(threadIdx.x*blockDim.x)+threadIdx.y]= HAUX[i*N+j]+ (c*c)*((dt/dd)*(dt/dd))*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]);
      
    }
    else //caso normal
    {
      iMenos1=HAUX[(i-1)*N+j];
      jMenos1=HAUX[i*N+(j-1)];
      iMas1=HAUX[(i+1)*N+j];
      jMas1=HAUX[i*N+(j+1)];
      temp[(threadIdx.x*blockDim.x)+threadIdx.y]= 2*HAUX[i*N+j]-H2[i*N+j] +(c*c)*((dt/dd)*(dt/dd))*(iMas1+iMenos1+jMenos1+jMas1-4*HAUX[i*N+j]);
     
    }
    
    H1[i*N+j] =temp[(threadIdx.x*blockDim.x)+threadIdx.y];
  }
    
  

}

__global__ void swap(float *origen,float *destino, int N)
{
  int j, i;
  i = blockDim.x*blockIdx.x + threadIdx.x;  // global index x (horizontal)
  j = blockDim.y*blockIdx.y + threadIdx.y;  // global index y (vertical)
  if (i >1 && i<N-1 && j>1 && j<N-1 )
    destino[i*N+j]=origen[i*N+j];
  
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

  dim3 blocksize;
  dim3 gridsize;

  //Numero de threads en cada bloque

  int BiasX = 0, BiasY = 0;

  if (N%x!=0)
    BiasX = 1;
  if (N%y!=0)
    BiasY = 1;
  gridsize.x = (N/x)+BiasX;
  gridsize.y = (N/y)+BiasY;
  blocksize.x = x;
  blocksize.y = y;

  //printf("El tamaño de N: %d, gridZise: %d * %d\n",N,gridsize.x,gridsize.y);
  int contador = 1;

  int numBlocks = gridsize.x*gridsize.y;
  int TamBloque = x*y;
  int device;
  cudaDeviceProp prop;
  int activeWarps;
  int maxWarps;

  cudaGetDevice(&device);
  cudaGetDeviceProperties(&prop, device);  
  cudaOccupancyMaxActiveBlocksPerMultiprocessor( &numBlocks,
        wave,
        TamBloque,
        0);

    activeWarps = numBlocks * TamBloque / prop.warpSize;
    maxWarps = prop.maxThreadsPerMultiProcessor / prop.warpSize;

    printf ("La ocupancia %lf  active/max*100 \n",(double)activeWarps / maxWarps * 100);
    printf ("MAx warp: %d   y  activos warp : %d \n",maxWarps,activeWarps);




  cudaEvent_t start2, stop2;
  float gpu_time = 0.0f;
  cudaEventCreate(&start2) ;
  cudaEventCreate(&stop2) ;
  cudaEventRecord(start2, 0);  

  time_t start = time(NULL);

  //Se ejecuta el kernel
  while( contador <= T)
  {
    swap<<<gridsize, blocksize>>>(d_H1,d_HAUX,N);
    cudaDeviceSynchronize();
    wave<<<gridsize, blocksize, x*y*sizeof(float)>>>(d_H1, d_H2, d_HAUX, N, contador);
    cudaError_t err = cudaGetLastError();
            if (err != cudaSuccess)
              printf("Error: %s\n", cudaGetErrorString(err));
    swap<<<gridsize, blocksize>>>(d_HAUX,d_H2,N);
    cudaDeviceSynchronize();
    contador++;

  }
  time_t finish = time(NULL);
  cudaEventRecord(stop2, 0);
  cudaEventSynchronize(stop2);
  cudaEventElapsedTime(&gpu_time, start2, stop2);
  printf("Time spent: %.5f\n", gpu_time);
  cudaEventDestroy(start2);
  cudaEventDestroy(stop2);
  printf("Wall-Clock: %.5f\n", (double)(finish - start));




/////////////////////////////////////////////////////
//  FIN:   Calculo de ecuacion
/////////////////////////////////////////////////////
  cudaMemcpy(H1, d_H1, N*N*sizeof(float), cudaMemcpyDeviceToHost);
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