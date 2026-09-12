ifeq ($(OS),Windows_NT)
	EXE = arkanoid.exe
	LIBS = -lpdcurses

else
	EXE = arkanoid
	LIBS = -lncursesw -lm
endif

CFLAGS = -Wall -Wpedantic

#activate dev tools (only used for level up when typing 'l' or 'L')
ifeq ($(DEV),1)
	CFLAGS += -D DEV
endif

# Group all object files for easier understanding
OBJS = main.o game_data.o application_state.o initialization_structures.o physics.o render.o


$(EXE): $(OBJS)
	gcc $(OBJS) -o $(EXE) $(CFLAGS) $(LIBS)
main.o: main.c established_parameters.h game_data.h initialization_structures.h physics.h render.h
	gcc main.c -c $(CFLAGS)
game_data.o: game_data.c game_data.h initialization_structures.h physics.h established_parameters.h
	gcc game_data.c -c $(CFLAGS)
application_state.o: application_state.c initialization_structures.h physics.h established_parameters.h
	gcc application_state.c -c $(CFLAGS)
initialization_structures.o: initialization_structures.c established_parameters.h initialization_structures.h
	gcc initialization_structures.c -c $(CFLAGS)
physics.o: physics.c physics.h established_parameters.h initialization_structures.h
	gcc physics.c -c $(CFLAGS)
render.o: render.c render.h established_parameters.h 
	gcc render.c -c $(CFLAGS)

clear:
	$(RM) $(EXE) $(OBJS)
	# $(RM) should adapt to the OS being used, no need for if statement. This deletes the executable and the object files.
