Lab_1


Cosas necesarias:

	-Sistema operativo Ubuntu 18.X o superiores
	-ingresar el siguiente comando en la consola:


para compilar:
	En la carpeta de destino ingresar el comando:

		$ make

Para ejecutar:		

	$ ./wave [-N numero entero] [-T numero entero] [-H numero entero] [-f cadena caracteres] [-t numero entero]

	donde:
		-N: Tamaño de grilla
        -T: Numero de pasos.
        -H: Numero de hebras
        -f: NOMBRE del archivo de salida:
        -t: Numero de iteraciones de salida


como ejemplo se puede ejecutar:

./wave -N 12 -T 32 -H 51 -f texto.txt -t 12