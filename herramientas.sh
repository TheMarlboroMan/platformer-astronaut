#!/bin/bash

clear

while [ "$opcion" != 0 ]
do
	#menu
	echo "[0] - Salir
[1] - Make
[r] - run
[k] - Limpiar compilacion
[l] - Limpiar copias de seguridad"

	read opcion;
	
	case "$opcion" in

		"0" ) 
			echo "Saliendo" 
		;;

		"1" )
			make
		;;

		"r" | "R" )
			./a.out
		;;

		"k" | "K" )
			rm ./objects/*
			rm ./a.out
			echo "Listo"
		;;

		"l" | "L" )
			find ./ -name '*~' -exec rm '{}' \; -print -or -name ".*~" -exec rm {} \; -print
		;;

		#Por defecto.
		* ) 
			echo "Error: opción incorrecta" 
		;;
	esac
done

exit 0
