#FOR A WIN .EXE PROGRAM (make on the terminal)not working btw
arkanoid_win: main.o game_data.o application_state.o  initialization_structures.o physics.o render.o
	gcc main.o game_data.o application_state.o initialization_structures.o physics.o render.o -o arkanoid_win -Wall -lpdcurses
main.o: main.c established_parameters.h game_data.h initialization_structures.h physics.h render.h
	gcc main.c -c
game_data.o: game_data.c game_data.h initialization_structures.h physics.h established_parameters.h
	gcc game_data.c -c
application_state.o: application_state.c initialization_structures.h physics.h established_parameters.h #app state  esta vacio y no existe su .h asì que es solo una cuastion de estructurar el codigo
	gcc application_state.c -c
initialization_structures.o: initialization_structures.c established_parameters.h initialization_structures.h
	gcc initialization_structures.c -c
physics.o: physics.c physics.h established_parameters.h initialization_structures.h
	gcc physics.c -c 
render.o: render.c render.h established_parameters.h 
	gcc render.c -c 
