#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "funciones.h"
#include "funciones2.h"




/*  
  -Entradas:
      -argc: que consiste en el contador de argumentos ingresados en la linea de comandos. El nombre del programa que se ejecuta se cuenta como un argumento, su formato es entero
      -*argv: arreglo con las entradas ingresadas por linea de comandos, su formato es erreglo de caracteres
      -*c: punteros a variable c que se utilizara para pasar su valor por referencia, su formato es entero
      -*u: punteros a variable u que se utilizara para pasar su valor por referencia, su formato es entero
      -*m: cadena de caracteres utilizada como referencia, su formato es cadena de caracteres
      -*n: punteros a variable n que se utilizara para pasar su valor por referencia, su formato es entero
      -*flag: punteros a variable flag que se utilizara para pasar su valor por referencia, su formato es entero
  Funcionamiento:
      Funcion que analizara los argumetos recibidos como parametros a la hora de ejecutar
      Estos argumetnos vienen en el dato argv, y siguen el formato getopt que es el siguiente:
        -c: cantidad de imagenes
        -u: UMBRAL para binarizar la imagen.
        -n: UMBRAL para clasificacion
        -m: NOMBRE del archivo que contiene la mascara a utilizar de la siguiente forma:
        -b: bandera que indica si se deben mostrar los resultados por pantalla, es decir, la conclusiÃ³n obtenida
            al leer la imagen binarizada.
      Estos argumentos son obligatorios con excepcion de -b
      Tambien se comprobara que cumplan el formato deseado, en caso de que alguno de estos no se cumpla, se saldra de la ejecucion              

  -Salida: 
      argumento recibido de -m, su formato es cadena de caracteres
*/

char * recibirArgumentos(int argc, char *argv[], int *c, int *u, char * m, int *n, int *flag)
{
  int flags, opt;
  char *aux3;
  aux3 = malloc(10*sizeof(char));
  if(argc <9){//si se ingresa un numero de argumentos menor a 9, se finaliza la ejecucion del programa
    //Debe ser 8, porque el nombre del programa se considera como un argumento, siendo -c,-u,-m,-n 
    //y sus respectivos valores que acompaÃ±en. No se considera -b (que seria el decimo argumento) porque es un flag que puede ser especificado por el usuario o no
    printf("Se ingreso un numero incorrecto de argumentos\n");
    fprintf(stderr, "Uso correcto: %s [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]\n",argv[0]);
       exit(EXIT_FAILURE);
    }
  int N1 = -1,N2 = -1,N4 = -1;

  flags = 0;
  //Se inicia un ciclo while hasta que se verifiquen todos los argumentos ingresados como entradas, los cuales se reciben con getopt()
  //int getopt (int argc, char *const *argv, const char *options): Siempre se le debe entregar argc y argv. El tercer argumento consiste en el formato de las entradas, ya sea
  //el caracter correspondiente a la opcion/entrada que se quiere recibir y ademÃ¡s se debe indicar si dicha entrada estarÃ¡ acompaÃ±ada de algÃºn valor
  //Esto se indica con ":". Por lo tanto, "c:" quiere decir que se espera recibir la opcion -c y esta si o si debe estar acompaÃ±ada de un valor
  //En cambio, la opcion "-b" no necesita estar acompaÃ±ada de un valor
  while((opt = getopt(argc, argv, "c:u:m:n:b")) != -1) { 
     //opt recibe el argumento leido (se hace de forma secuencial) y se ingresa a un switch
     //En caso de que opt sea -1, se dejaran de buscar entradas y por lo tanto se podrÃ¡ salir del while
     switch(opt) {
     case 'b'://se busca la entrada -b, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -b
       flags = 1;
       break;

     case 'c': //se busca la entrada -c
    	N1 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -c a entero
       if(optarg!=0 && N1==0){//si no se ingresa un argumento junto a -c o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
      case 'u': //se busca la entrada -u
       N2 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -u a entero
       if(optarg!=0 && N2==0){//si no se ingresa un argumento junto a -u o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
      case 'm': //se busca la entrada -m
      m=optarg ;
       if(optarg!=0 && strcmp(optarg,"-b") == 0){//si no se ingresa un argumento junto a -m o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
       case 'n': //se busca la entrada -n
       N4 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -n a entero
       if(optarg!=0 && N4==0){//si no se ingresa un argumento junto a -n o si no se logra parsear el argumento ingresado, se considera como invalido
        fprintf(stderr, "Uso correcto: %s [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]\n",argv[0]);
        exit(EXIT_FAILURE);
         }
       break;
     default: /* '?' */
       fprintf(stderr, "Uso correcto: %s [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]\n",argv[0]);
       exit(EXIT_FAILURE);
     }
  }

  if(flags==1){//si se encontro un flag -m, se setea la variable global flag = 1, respecto al scope del proceso principal
    (*flag) = 1;
    }
  (*c) = N1; //se iguala la variable c a N, para poder acceder al valor en el main
  (*u) = N2; //se iguala la variable u a N, para poder acceder al valor en el main
  (*n) = N4; //se iguala la variable n a N, para poder acceder al valor en el main

  if(N1<=0)
  {
    printf("El valor que acompaÃ±a a -c debe ser mayor a 0\n");
    exit(EXIT_FAILURE);
  }

  if(N4<0 || N4>100)
  {
    printf("El valor que acompaÃ±a a -n debe estar entre 0(cero) y 100\n");
    exit(EXIT_FAILURE);
  }

  return m;
}



void holaMundoFunc1( int parametro1 )
{
  printf("hola mundo del funciones 1 %d \n",parametro1);

}

