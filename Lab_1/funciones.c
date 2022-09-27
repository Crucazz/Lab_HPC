#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "funciones.h"
#include "funciones2.h"
#include <math.h>




/*  
  -Entradas:
      -argc: que consiste en el contador de argumentos ingresados en la linea de comandos. El nombre del programa que se ejecuta se cuenta como un argumento, su formato es entero
      -*argv: arreglo con las entradas ingresadas por linea de comandos, su formato es erreglo de caracteres
      -*N: punteros a variable N que se utilizara para pasar su valor por referencia, su formato es entero
      -*T: punteros a variable T que se utilizara para pasar su valor por referencia, su formato es entero
      -*f: cadena de caracteres utilizada como referencia, su formato es cadena de caracteres
      -*H: punteros a variable H que se utilizara para pasar su valor por referencia, su formato es entero
      -*t: punteros a variable t que se utilizara para pasar su valor por referencia, su formato es entero
  Funcionamiento:
      Funcion que analizara los argumetos recibidos como parametros a la hora de ejecutar
      Estos argumetnos vienen en el dato argv, y siguen el formato getopt que es el siguiente:
        -N: Tamaño de grilla
        -T: Numero de pasos.
        -H: Numero de hebras
        -f: NOMBRE del archivo de salida:
        -t: Numero de iteraciones de salida.
      Estos argumentos son obligatorios
      Tambien se comprobara que cumplan el formato deseado, en caso de que alguno de estos no se cumpla, se saldra de la ejecucion              

  -Salida: 
      argumento recibido de -f, su formato es cadena de caracteres
*/

char * recibirArgumentos(int argc, char *argv[], int *N, int *T, char * f, int *H, int *t)
{
  int  opt;
  char *aux3;
  aux3 = malloc(10*sizeof(char));
  if(argc <11){//si se ingresa un numero de argumentos menor a 9, se finaliza la ejecucion del programa
    //Debe ser 10, porque el nombre del programa se considera como un argumento, siendo -N,-T,-H,-f,-t 
    //y sus respectivos valores que acompaÃ±en. No se considera -b (que seria el decimo argumento) porque es un flag que puede ser especificado por el usuario o no
    printf("Se ingreso un numero incorrecto de argumentos\n");
    fprintf(stderr, "Uso correcto: %s [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]\n",argv[0]);
       exit(EXIT_FAILURE);
    }
  int N1 = -1,N2 = -1,N4 = -1, Nt= -1;

  //Se inicia un ciclo while hasta que se verifiquen todos los argumentos ingresados como entradas, los cuales se reciben con getopt()
  //int getopt (int argc, char *const *argv, const char *options): Siempre se le debe entregar argc y argv. El tercer argumento consiste en el formato de las entradas, ya sea
  //el caracter correspondiente a la opcion/entrada que se quiere recibir y ademÃ¡s se debe indicar si dicha entrada estarÃ¡ acompaÃ±ada de algÃºn valor
  //Esto se indica con ":". Por lo tanto, "c:" quiere decir que se espera recibir la opcion -c y esta si o si debe estar acompaÃ±ada de un valor
  //En cambio, la opcion "-b" no necesita estar acompaÃ±ada de un valor
  while((opt = getopt(argc, argv, "N:T:f:H:t:")) != -1) { 
     //opt recibe el argumento leido (se hace de forma secuencial) y se ingresa a un switch
     //En caso de que opt sea -1, se dejaran de buscar entradas y por lo tanto se podrÃ¡ salir del while
     switch(opt) {
     case 't'://se busca la entrada -t
       Nt = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -t a entero
       if(optarg!=0 && Nt==0){//si no se ingresa un argumento junto a -t o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;

     case 'N': //se busca la entrada -N
    	N1 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -N a entero
       if(optarg!=0 && N1==0){//si no se ingresa un argumento junto a -N o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
      case 'T': //se busca la entrada -u
       N2 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -T a entero
       if(optarg!=0 && N2==0){//si no se ingresa un argumento junto a -T o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
      case 'f': //se busca la entrada -f
        f=optarg ;
       if(optarg!=0 && strcmp(optarg,"-f") == 0){//si no se ingresa un argumento junto a -f o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
       case 'H': //se busca la entrada -H
       N4 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -H a entero
       if(optarg!=0 && N4==0){//si no se ingresa un argumento junto a -H o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
     default: /* '?' */
       fprintf(stderr, "Uso correcto: %s [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]\n",argv[0]);
       exit(EXIT_FAILURE);
     }
  }

  (*t) = Nt;
  (*N) = N1;
  (*T) = N2;
  (*H) = N4;

  if(N1<=0)
  {
    printf("El valor que acompaña a -N debe ser mayor a 0\n");
    exit(EXIT_FAILURE);
  }
  if(N2<=0)
  {
    printf("El valor que acompaña a -T debe ser mayor a 0\n");
    exit(EXIT_FAILURE);
  }
  if(N4<=0)
  {
    printf("El valor que acompaña a -t debe ser mayor a 0\n");
    exit(EXIT_FAILURE);
  }


  return f;
}



void swap(float *origen,float *destino, int N)
{
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++)
      destino[i*N+j]=origen[i*N+j];   
  }
  
}

void saveImg(float *imagen, float *imagenAmpliada, int N)
{
  printf("El N es: %d",N);
  N=N-1;
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
        //printf("Nuevo [%d %d] = viejo [%d %d] \n",i,j,i+1,j+1);
        imagen[i*N+j] = imagenAmpliada[(i+1)*N+(j+1)];
    }
  }
}



void holaMundoFunc1( int parametro1 )
{
  printf("hola mundo del funciones 1 %d \n",parametro1);

}

