#include "Game/Game.hpp"
#include "Role/Factory/Factory.hpp"
#include "Role/Strategy/Strategy.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>

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

    // 🏭 Factory-created players
    Player* arthur = FactoryPlayers::createPlayer(ROLE_GENERAL, "Arthur", game,true);
    Player* elaine = FactoryPlayers::createPlayer(ROLE_GENERAL, "Elaine", game,true);
    Player* moshe  = FactoryPlayers::createPlayer(ROLE_GOVERNOR, "Moshe", game,true);

    game.add_player(arthur);
    game.add_player(elaine);
    game.add_player(moshe);

    // 🤖 Strategy assignment (e.g., AIaggresive to all)
    unordered_map<Player*, AIaction*> ai_map;
    ai_map[arthur] = new AIaggresive();
    ai_map[elaine] = new AIaggresive();
    ai_map[moshe]  = new AIaggresive();

    int turn_count = 1;

    try {
        while (game.active_players().size() > 1 && turn_count <= 1000) {
            Player* current = game.current_player();
            cout << "\n--- Turn " << turn_count << ": " << current->get_name()
                 << " (" << current->get_role_name() << ") ---" << endl;

            try {
                Player* target = get_coup_target(current, game.get_players());

                if (ai_map.count(current)) {
                    // 🎯 Track coup target for later validation
                    bool alive_before = target ? target->get_active() : false;

                    // 🧠 Let the AI choose what to do
                    ai_map[current]->favorite_action(current, *target);

                    // ✅ Coup feedback
                    if (target && target->get_action_indicator()[Actions::Coup].first) {
                        if (target->get_active() && !alive_before)
                            cout << "✅ " << target->get_name() << " was saved by a General!\n";
                        else if (!target->get_active())
                            cout << "💀 " << target->get_name() << " has been eliminated.\n";
                    }
                }
            } catch (const exception& ex) {
                cout << "Exception: " << ex.what() << endl;
                game.next_turn();
            }

            print_balances(game);
            turn_count++;

            this_thread::sleep_for(chrono::milliseconds(1000));
        }

        if (turn_count > 1000) {
            cout << "\n=== GAME TIMEOUT: Too many turns ===" << endl;
        } else {
            cout << "\n=== GAME OVER ===" << endl;
            cout << "🏆 Winner: " << game.winner() << endl;
        }

    } catch (const exception& e) {
        cout << "Fatal Error: " << e.what() << endl;
    }

    // ✅ Clean up AI strategies
    for (auto& pair : ai_map) {
        delete pair.second;
    }

    return 0;
}
