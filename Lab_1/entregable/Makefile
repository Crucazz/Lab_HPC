salida_out = wave


salida_headers = funciones.h
salida_source  = $(salida_headers:.h=.c) wave.c
salida_objects = $(salida_source:.c=.o)


CC     = gcc
CFLAGS = -Wall -fopenmp

depends = .depends

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


