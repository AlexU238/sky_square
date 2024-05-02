CC := g++
LFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image

a.out: main.o game.o map_generator.o game_object.o entity.o input_box.o player.o mob.o text.o block.o drawable.o 
	$(CC) *.o $(LFLAGS)
	rm *.o
	
main.o: main.cpp
	$(CC) -c main.cpp

game.o: Game.cpp
	$(CC) -c Game.cpp
	
map_generator.o: MapGenerator.cpp
	$(CC) -c MapGenerator.cpp
	
game_object.o: GameObject.cpp
	$(CC) -c GameObject.cpp
	
entity.o: Entity.cpp
	$(CC) -c Entity.cpp
	
input_box.o: InputBox.cpp
	$(CC) -c InputBox.cpp
	
player.o: Player.cpp
	$(CC) -c Player.cpp
	
mob.o: Mob.cpp
	$(CC) -c Mob.cpp
	
text.o: Text.cpp
	$(CC) -c Text.cpp
	
block.o: Block.cpp
	$(CC) -c Block.cpp
	
drawable.o: Drawable.cpp
	$(CC) -c Drawable.cpp
	
clean:
	rm a.out $(OBJECTS)
	

