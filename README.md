Repository for SkySquare sandbox game. Or a very bad attempt at a game.
Using C++ and SDL2 

A garbage of a game based of the Sky Block mini game in Minecraft and made look similar to terraria.

The main idea is that a player(a slime) is spawned into a world of floating islands which he can travel.

Players can destroy and place blocks. The block can be destroyed or placed by clicking the right mouse button within a 2 block radius from the player head.

Because the player is a Slime, he can somewhat stick to the walls and ceiling. This adds difficulty to traverse the underground. (this was totaly a feature, not a bug)

Player can jump by pressing space bar and move around by using the WASD key-set. Movement can lead to a situation when the player can fall to his doom from a high point on map, or fall off the island into the void. Both result in game over if the player health drops down to zero.

Most block have collision (solid blocks) so that the player could walk on them. Others are not solid and players go through them.

The program generates a new random map each time the player creates it and starts a new game. With the exeption of start island

Map can be saved, along with the player progress by pressing save. Progress can be loaded from file, which can be chosen in game, by pressing load.


Requires: -lSDL2 -lSDL2_ttf -lSDL2_image
