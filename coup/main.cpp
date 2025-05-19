#include "../coup/Game/Game.hpp"
#include "../roles/Governor/Governor.hpp"
#include "../roles/Judge/Judge.hpp"
#include <iostream>

int main() {
    Game& game = Game::get_instance();

    // Create players
    Governor* gov = new Governor("GovernorPlayer", game, false);
    Judge* judge = new Judge("JudgePlayer", game, true);  // AI judge (to auto decide)

    // Add players to the game
    game.add_player(gov);
    game.add_player(judge);

    // Give the governor some coins
    gov->add_coins(4);  // Bribe requires 4 coins

    // Print initial state
    std::cout << gov->get_name() << " coins before bribe: " << gov->get_coins() << std::endl;
    std::cout << "Extra turn flag before bribe: " << (gov->has_extra_turn() ? "true" : "false") << std::endl;

    try {
        gov->bribe();  // This will trigger notify_Judge_Bribe
    } catch (const std::exception& e) {
        std::cerr << "Bribe failed: " << e.what() << std::endl;
    }

    std::cout << gov->get_name() << " coins after bribe: " << gov->get_coins() << std::endl;
    std::cout << "Extra turn flag after bribe: " << (gov->has_extra_turn() ? "true" : "false") << std::endl;

    return 0;
}
