#include "Game/Game.hpp"
#include "../roles/Governor/Governor.hpp"
#include "../roles/Judge/Judge.hpp"
#include "../roles/Baron/Baron.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace std;

void print_balances(Game& game) {
    cout << "\n--- Coin Balances ---" << endl;
    for (auto* p : game.get_players()) {
        cout << p->get_role_name() << " (" << p->get_name() << "): " << p->get_coins() << " coins" << endl;
    }
    cout << "Pool: " << game.get_pool() << " coins\n" << endl;
}

Player* get_coup_target(Player* current, const vector<Player*>& players) {
    for (Player* p : players) {
        if (p != current && p->get_active()) return p;
    }
    return nullptr;
}

int main() {
    Game game;

    auto* gov = new Governor("Moshe", game);
    auto* judge = new Judge("Sarah", game);
    auto* baron = new Baron("Elie", game);

    game.add_player(gov);
    game.add_player(judge);
    game.add_player(baron);

    int turn_count = 1;

    try {
        while (game.active_players().size() > 1 && turn_count <= 1000) {
            Player* current = game.current_player();
            cout << "--- Turn " << turn_count << ": " << current->get_name() << " (" << current->get_role_name() << ") ---" << endl;

            try {
                Player* target = get_coup_target(current, game.get_players());

                if (current->get_coins() >= 10 && target) {
                    current->coup(*target);
                }
                else if (auto* g = dynamic_cast<Governor*>(current)) {
                    if (g->get_coins() >= 7 && target) {
                        g->coup(*target);
                    } else if (g->get_coins() >= 4) {
                        g->bribe();
                    } else {
                        g->gather();
                    }
                } else if (auto* j = dynamic_cast<Judge*>(current)) {
                    if (j->get_coins() >= 3) {
                        j->tax();
                    } else {
                        j->gather();
                    }
                } else if (auto* b = dynamic_cast<Baron*>(current)) {
                    if (b->get_coins() >= 7 && target) {
                        b->coup(*target);
                    } else if (b->get_coins() >= 3) {
                        b->tax();
                    } else {
                        b->gather();
                    }
                }
            } catch (const std::exception& ex) {
                cout << "Exception: " << ex.what() << endl;
                game.next_turn(); // prevent freeze
            }

            print_balances(game);
            turn_count++;

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        if (turn_count > 1000) {
            cout << "\n=== GAME TIMEOUT: Too many turns ===" << endl;
        } else {
            cout << "\n=== GAME OVER ===" << endl;
            cout << "Winner: " << game.winner() << endl;
        }

    } catch (const std::exception& e) {
        cout << "Fatal Error: " << e.what() << endl;
    }


    return 0;
}
