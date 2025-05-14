#include "Game/Game.hpp"
#include "Role/Factory/Factory.hpp"
#include <iostream>

using namespace std;

void show_balances(Game& game) {
    cout << "\n--- Current Balances ---" << endl;
    for (auto* p : game.get_players()) {
        cout << p->get_name() << ": " << p->get_coins() << " coins" << endl;
    }
    cout << "Pool: " << game.get_pool() << " coins" << endl;
}

int main() {
    Game game;

    Player* g1 = FactoryPlayers::createPlayer(ROLE_GENERAL, "General1", game, false);
    Player* g2 = FactoryPlayers::createPlayer(ROLE_GENERAL, "General2", game, false);

    game.add_player(g1);
    game.add_player(g2);

    int turn_counter = 1;
    int max_turns = 8;

    while (turn_counter <= max_turns) {
        Player* current = game.current_player();
        cout << "\nTurn " << turn_counter << ": " << current->get_name() << endl;

        try {
            if (current == g1 && turn_counter == 1) {
                cout << "General1 performs BRIBE (sets extra turn)" << endl;
                dynamic_cast<General*>(g1)->bribe();
            } else {
                cout << current->get_name() << " performs GATHER" << endl;
                dynamic_cast<General*>(current)->gather();
            }

            show_balances(game);

            // Check if extra turn is scheduled (without clearing it yet)
            if (current->has_extra_turn()) {
                cout << "➡ " << current->get_name() << " will have an EXTRA TURN after their next normal turn.\n";
            }

        } catch (const exception& e) {
            cout << "Exception: " << e.what() << endl;
        }

        // Move to next player as usual
        game.next_turn();
        turn_counter++;

        // If after moving to current again and still has extra turn, give it now.
        Player* next = game.current_player();
        if (next->has_extra_turn()) {
            cout << "⬅ Extra turn NOW for " << next->get_name() << endl;
            next->clear_extra_turn();

            cout << next->get_name() << " performs GATHER (EXTRA TURN)" << endl;
            dynamic_cast<General*>(next)->gather();
            show_balances(game);
        }
    }

    cout << "\n=== Simulation End ===\n";
}
