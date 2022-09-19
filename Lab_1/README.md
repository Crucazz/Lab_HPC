Lab_1


Cosas necesarias:

	-Sistema operativo Ubuntu 18.X o superiores
	-ingresar el siguiente comando en la consola:

		$ sudo apt-get install libjpeg8-dev 

para compilar:
	En la carpeta de destino ingresar el comando:

		$ make

Para ejecutar:		

	$ ./pipeline [-c numero entero] [-u numero entero] [-n numero entero] [-m cadena caracteres] [-b]

	donde:
		-c: cantidad de imágenes
		-u: UMBRAL para binarizar la imagen.
		-n: UMBRAL para clasificación
		-m: NOMBRE del archivo que contiene la máscara a utilizar de la siguiente forma:
		-b: bandera que indica si se deben mostrar los resultados por pantalla, es decir, la conclusión obtenida al leer la imagen binarizada.

	Ademas sólo se podrán recibir máscaras de 3x3, con 3 filas y 3 columnas. Las columnas separadas por espacios
	y las filas separadas por saltos de lı́nea. Por ejemplo: laplace.txt


como ejemplo se puede ejecutar:

./pipeline -c 2 -u 70 -m laplace.txt -n 20 -b