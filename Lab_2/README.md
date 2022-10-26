Lab_1


Cosas necesarias:

	-Sistema operativo Ubuntu 18.X o superiores
	-ingresar el siguiente comando en la consola:


para compilar:
	En la carpeta de destino ingresar el comando:

		$ make

Para ejecutar:		

	$ ./wave [-N numero entero] [-x numero entero] [-y numero entero] [-T numero entero] [-f cadena caracteres]

	donde:
		-N: Tamaño de grilla        
        -x: Tamaño del bloque en X
        -y: Tamaño del bloque en Y
        -T: Numero de pasos.
        -f: NOMBRE del archivo de salida:
        


como ejemplo se puede ejecutar:


./wave -N 30 -x 5 -y 5 -T 1 -f imagen.raw
