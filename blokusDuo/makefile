SRC=main.c # fichiers source
SRC+=piece.c game.c init.c decisionTree.c blokus.c# autre fichiers source
EXE=blokus.out # fichier exécutable

CC=gcc
CFLAGS:=-Wall -Wextra -MMD -Og -g $(sdl2-config --cflags) # options de compilation
LDFLAGS:=-lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2  # options de l'éditeur de liens

OBJ=$(addprefix build/,$(SRC:.c=.o)) # met les .o dans build
DEP=$(addprefix build/,$(SRC:.c=.d)) # met les .d dans build (fichiers de dépendance)

all: $(OBJ)
	$(CC) -o $(EXE) $^ $(LDFLAGS) 
    # lie tous les fichiers objets pour créer l'exécutable

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $< 
    # compile le fichier source en fichier objet

clean:
	rm -rf build core *.o # supprime le répertoire build, le fichier core, et tous les fichiers .o

-include $(DEP) # inclut les fichiers de dépendances générés par -MMD