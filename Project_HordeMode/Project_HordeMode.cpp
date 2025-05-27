#include <SFML/Graphics.hpp>
#include"Header.h"
int main()
{
    Game game;

    while (game.running()) {

        game.update();
        game.render();

    }
}