#include <SFML/Graphics.hpp>
#include"Header.h"
using namespace sf;
int main()
{
    Game game;
    while (game.running()) {

        game.update();
        game.render();

    }
}