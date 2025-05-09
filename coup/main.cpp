#include "Game/Game.hpp"
#include "../roles/Governor/Governor.hpp"
#include "../roles/General/General.hpp"
#include <iostream>
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

    auto* general1 = new General("Arthur", game);
    auto* general2 = new General("Elaine", game);
    auto* gov = new Governor("Moshe", game);

    game.add_player(general1);
    game.add_player(general2);
    game.add_player(gov);

    int turn_count = 1;

    try {
        while (game.active_players().size() > 1 && turn_count <= 1000) {
            Player* current = game.current_player();
            cout << "\n--- Turn " << turn_count << ": " << current->get_name()
                 << " (" << current->get_role_name() << ") ---" << endl;

            try {
                Player* target = get_coup_target(current, game.get_players());

                if (current->get_coins() >= 10 && target) {
                    cout << current->get_name() << " used COUP on " << target->get_name() << endl;
                    bool alive_before = target->get_active();
                    current->coup(*target);
                    if (target->get_active() && !alive_before) {
                        cout << "✅ " << target->get_name() << " was saved by a General!\n";
                    } else if (!target->get_active()) {
                        cout << "💀 " << target->get_name() << " has been eliminated.\n";
                    }
                }
                else if (auto* g = dynamic_cast<Governor*>(current)) {
                    if (g->get_coins() >= 7 && target) {
                        cout << g->get_name() << " used COUP on " << target->get_name() << endl;
                        bool alive_before = target->get_active();
                        g->coup(*target);
                        if (target->get_active() && !alive_before) {
                            cout << "✅ " << target->get_name() << " was saved by a General!\n";
                        } else if (!target->get_active()) {
                            cout << "💀 " << target->get_name() << " has been eliminated.\n";
                        }
                    } else if (g->get_coins() >= 4) {
                        cout << g->get_name() << " used BRIBE" << endl;
                        g->bribe();
                    } else {
                        cout << g->get_name() << " used GATHER" << endl;
                        g->gather();
                    }
                }
                else if (auto* gen = dynamic_cast<General*>(current)) {
                    if (gen->get_coins() >= 7 && target) {
                        cout << gen->get_name() << " used COUP on " << target->get_name() << endl;
                        bool alive_before = target->get_active();
                        gen->coup(*target);
                        if (target->get_active() && !alive_before) {
                            cout << "✅ " << target->get_name() << " was saved by a General!\n";
                        } else if (!target->get_active()) {
                            cout << "💀 " << target->get_name() << " has been eliminated.\n";
                        }
                    } else if (gen->get_coins() >= 3) {
                        cout << gen->get_name() << " used TAX" << endl;
                        gen->tax();
                    } else {
                        cout << gen->get_name() << " used GATHER" << endl;
                        gen->gather();
                    }
                }

            } catch (const std::exception& ex) {
                cout << "Exception: " << ex.what() << endl;
                game.next_turn(); // Skip on error
            }

            print_balances(game);
            turn_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }

        if (turn_count > 1000) {
            cout << "\n=== GAME TIMEOUT: Too many turns ===" << endl;
        } else {
            cout << "\n=== GAME OVER ===" << endl;
            cout << "🏆 Winner: " << game.winner() << endl;
        }

    } catch (const std::exception& e) {
        cout << "Fatal Error: " << e.what() << endl;
    }

    return 0;
}
