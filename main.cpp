#include "GameEngine.hpp"
#include "SFML/Graphics.hpp"

int main()
{
    GameEngine game("config.txt");

	  game.run();
    return 0;
}