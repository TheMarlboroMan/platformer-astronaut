#Definiciones.

#-DLOG_BASE_SILENCIOSO=1
CFLAGS=-c -g -O0 -Wno-deprecated -Wall -ansi -pedantic -std=c++0x 
CFLAGS_TARGET_FINAL=-g -O0 -Wno-deprecated -Wall -ansi -pedantic -std=c++0x 
CC=g++
DIR_CLASS=./class/
DIR_CLASS_BASE=./class/base_proyecto/
DIR_O=./objects/
LIBRERIAS=-lDanSDL2 -lSDL2 -lSDL2_mixer -lSDL2_image
# -lcurl

################################################################################
#Definición de objetos necesarios para main.

OBJETOS_BASE=$(DIR_O)input_base.o \
	$(DIR_O)configuracion_base.o \
	$(DIR_O)audio_base.o \
	$(DIR_O)tabla_sprites.o \
	$(DIR_O)cargador_recursos_base.o \

OBJETOS=$(DIR_O)input.o \
	$(DIR_O)configuracion.o \
	$(DIR_O)localizador.o \
	$(DIR_O)audio.o \
	$(DIR_O)cargador_recursos.o \
	$(DIR_O)controlador.o \
	$(DIR_O)celda.o \
	$(DIR_O)nivel.o \
	$(DIR_O)movil.o \
	$(DIR_O)representable.o \
	$(DIR_O)espaciable.o \
	$(DIR_O)representador.o \
	$(DIR_O)actor.o \
	$(DIR_O)jugador.o \
	$(DIR_O)proyectil.o \
	$(DIR_O)obstaculo_generico.o \
	$(DIR_O)plataforma_movil.o \

################################################################################
#Definiciones de targets.

clean:
	./limpiar.sh;

preparar_subida:
	./preparar_subida.sh;

all: a.out 

a.out: $(OBJETOS) \
main.cpp \
libDanSDL2.a 
	$(CC) $(CFLAGS_TARGET_FINAL) main.cpp $(OBJETOS_BASE) $(OBJETOS) -o a.out -L. $(LIBRERIAS)

################################################################################
#Clases de la aplicacion...

$(DIR_O)controlador.o :\
$(DIR_O)tabla_sprites.o \
$(DIR_O)nivel.o \
$(DIR_O)celda.o \
$(DIR_O)input.o \
$(DIR_O)actor.o \
$(DIR_O)jugador.o \
$(DIR_O)proyectil.o \
$(DIR_O)configuracion.o \
$(DIR_O)localizador.o \
$(DIR_O)audio.o \
$(DIR_O)cargador_recursos.o \
$(DIR_O)representable.o \
$(DIR_O)espaciable.o \
$(DIR_O)representador.o \
$(DIR_O)obstaculo_generico.o \
$(DIR_O)plataforma_movil.o \
$(DIR_CLASS)origen_proyectil.h \
$(DIR_CLASS)controlador.h \
$(DIR_CLASS)controlador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador.cpp -o $(DIR_O)controlador.o

$(DIR_O)nivel.o :\
$(DIR_O)celda.o \
$(DIR_CLASS)nivel.h \
$(DIR_CLASS)nivel.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)nivel.cpp -o $(DIR_O)nivel.o

$(DIR_O)celda.o :\
$(DIR_O)representable.o \
$(DIR_O)espaciable.o \
$(DIR_CLASS)celda.h \
$(DIR_CLASS)celda.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)celda.cpp -o $(DIR_O)celda.o

$(DIR_O)jugador.o :\
$(DIR_O)actor.o \
$(DIR_O)movil.o \
$(DIR_CLASS)origen_proyectil.h \
$(DIR_CLASS)jugador.h \
$(DIR_CLASS)jugador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)jugador.cpp -o $(DIR_O)jugador.o

$(DIR_O)proyectil.o :\
$(DIR_O)actor.o \
$(DIR_O)movil.o \
$(DIR_CLASS)origen_proyectil.h \
$(DIR_CLASS)proyectil.h \
$(DIR_CLASS)proyectil.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)proyectil.cpp -o $(DIR_O)proyectil.o

$(DIR_O)plataforma_movil.o :\
$(DIR_O)actor.o \
$(DIR_O)movil.o \
$(DIR_CLASS)plataforma_movil.h \
$(DIR_CLASS)plataforma_movil.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)plataforma_movil.cpp -o $(DIR_O)plataforma_movil.o

$(DIR_O)obstaculo_generico.o :\
$(DIR_O)actor.o \
$(DIR_CLASS)obstaculo_generico.h \
$(DIR_CLASS)obstaculo_generico.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)obstaculo_generico.cpp -o $(DIR_O)obstaculo_generico.o

$(DIR_O)actor.o :\
$(DIR_O)representable.o \
$(DIR_O)espaciable.o \
$(DIR_CLASS)actor.h \
$(DIR_CLASS)actor.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor.cpp -o $(DIR_O)actor.o

$(DIR_O)representable.o :\
$(DIR_CLASS)representable.h \
$(DIR_CLASS)representable.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)representable.cpp -o $(DIR_O)representable.o

$(DIR_O)movil.o :\
$(DIR_CLASS)movil.h \
$(DIR_CLASS)movil.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)movil.cpp -o $(DIR_O)movil.o

$(DIR_O)espaciable.o :\
$(DIR_CLASS)espaciable.h \
$(DIR_CLASS)espaciable.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)espaciable.cpp -o $(DIR_O)espaciable.o

$(DIR_O)representador.o :\
$(DIR_O)representable.o \
$(DIR_CLASS)representador.h \
$(DIR_CLASS)representador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)representador.cpp -o $(DIR_O)representador.o

################################################################################
#Clases por defecto...

$(DIR_O)input.o :\
$(DIR_O)input_base.o \
$(DIR_CLASS)input.h \
$(DIR_CLASS)input.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)input.cpp -o $(DIR_O)input.o

$(DIR_O)configuracion.o :\
$(DIR_O)configuracion_base.o \
$(DIR_CLASS)configuracion.h \
$(DIR_CLASS)configuracion.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)configuracion.cpp -o $(DIR_O)configuracion.o

$(DIR_O)localizador.o :\
$(DIR_CLASS)localizador.h \
$(DIR_CLASS)localizador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)localizador.cpp -o $(DIR_O)localizador.o

$(DIR_O)audio.o:\
$(DIR_O)audio_base.o \
$(DIR_CLASS)audio.h\
$(DIR_CLASS)audio.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)audio.cpp -o $(DIR_O)audio.o

$(DIR_O)cargador_recursos.o:\
$(DIR_O)cargador_recursos_base.o \
$(DIR_CLASS)cargador_recursos.h\
$(DIR_CLASS)cargador_recursos.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)cargador_recursos.cpp -o $(DIR_O)cargador_recursos.o

################################################################################
#Clases base

$(DIR_O)configuracion_base.o :\
$(DIR_CLASS_BASE)configuracion_base.h \
$(DIR_CLASS_BASE)configuracion_base.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS_BASE)configuracion_base.cpp -o $(DIR_O)configuracion_base.o

$(DIR_O)audio_base.o :\
$(DIR_CLASS_BASE)audio_base.h \
$(DIR_CLASS_BASE)audio_base.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS_BASE)audio_base.cpp -o $(DIR_O)audio_base.o

$(DIR_O)cargador_recursos_base.o :\
$(DIR_CLASS_BASE)cargador_recursos_base.h \
$(DIR_CLASS_BASE)cargador_recursos_base.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS_BASE)cargador_recursos_base.cpp -o $(DIR_O)cargador_recursos_base.o

$(DIR_O)input_base.o :\
$(DIR_CLASS_BASE)input_base.h \
$(DIR_CLASS_BASE)input_base.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS_BASE)input_base.cpp -o $(DIR_O)input_base.o

$(DIR_O)tabla_sprites.o :\
$(DIR_CLASS_BASE)tabla_sprites.h \
$(DIR_CLASS_BASE)tabla_sprites.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS_BASE)tabla_sprites.cpp -o $(DIR_O)tabla_sprites.o

