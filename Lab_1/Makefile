salida_out = pipeline


salida_headers = funciones.h funciones2.h
salida_source  = $(salida_headers:.h=.c) main.c
salida_objects = $(salida_source:.c=.o)


CC     = gcc
CFLAGS = -Wall -fopenmp

depends = .depends
depends1 = .depends1
depends2 = .depends2
depends3 = .depends3
depends4 = .depends4
depends5 = .depends5
depends6 = .depends6

build : $(salida_out) 

$(salida_out) : $(salida_objects)
	$(CC)  -o $@ $^  $(CFLAGS)



$(objects) :
	$(CC)  -c -o $@ $*.c $(CFLAGS)



$(depends) : $(salida_source) $(salida_headers)
	@$(CC) -MM $(salida_source) > $@



clean :
	$(RM) $(salida_out)  $(salida_objects) $(zipfile) $(depends) 

.PHONY : build zip clean

sinclude $(depends)


