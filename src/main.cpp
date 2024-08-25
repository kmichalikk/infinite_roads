#include "game.h"

int main() {
    Game game(800, 600);

    game.initialize();

    if (game.ready()) {
        game.startMainLoop();
    }
}
