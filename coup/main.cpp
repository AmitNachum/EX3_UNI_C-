#include "../coup/Game/Game.hpp"
#include "../roles/Governor/Governor.hpp"
#include "../roles/Merchant/Merchant.hpp"
#include <iostream>
#include <memory>

int main() {
    Game game;

    // Create Governor and Merchant
    Governor* gov = new Governor("GovernorPlayer", game, false);
    Merchant* merchant = new Merchant("MerchantPlayer", game, false);

    // Add to game
    game.add_player(gov);
    game.add_player(merchant);

    // Set Governor as current player (if needed)
    // game.force_set_current_player(gov);  // Only if you have a setter

    std::cout << "Merchant coins before tax: " << merchant->get_coins() << std::endl;

    // Governor blocks Merchant
    gov->block_tax(*merchant);

    // Merchant tries to tax
    merchant->tax();

    std::cout << "Merchant coins after tax: " << merchant->get_coins() << std::endl;

    // Expectation: coins should NOT have increased if block_tax worked


    return 0;
}
