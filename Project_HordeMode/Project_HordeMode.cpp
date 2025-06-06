#include <SFML/Graphics.hpp>
#include"Header.h"
using namespace sf;
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    Game game;
    while (game.running()) {
        game.update();
        game.render();
    }
    
    return 0;
}