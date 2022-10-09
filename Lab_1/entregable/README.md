Laboratorio 1  SIMD- OpenMP


Cosas necesarias:

	-Sistema operativo Ubuntu 18.X o superiores


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


	./wave -N 256 -T 300 -H 5 -f wave.raw -t 100
