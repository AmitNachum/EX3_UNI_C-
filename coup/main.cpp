// Mail: nachum.amit@msmail.ariel.ac.il

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../coup/Game/Game.hpp"
#include "../coup/Player/Player.hpp"
#include "../roles/Baron/Baron.hpp"
#include "../roles/Governor/Governor.hpp"

using namespace std;
using namespace chrono;

void simulate_turn(Player* player, Game& game) {
    vector<string> actions = {"gather", "tax", "coup", "sanction", "bribe", "arrest"};
    srand(time(nullptr) + reinterpret_cast<uintptr_t>(player)); // unique seed per player

    // Find a valid target for actions like coup/arrest/sanction
    Player* target = nullptr;
    for (Player* p : game.get_players()) {
        if (p != player && p->get_active()) {
            target = p;
            break;
        }
    }

    // Retry random action selection until something valid is chosen
    for (int attempts = 0; attempts < 10; ++attempts) {
        string action = actions[rand() % actions.size()];

        try {
            if (action == "gather") {
                player->gather();
                cout << "[ACTION] " << player->get_name() << " gathered." << endl;
                break;
            }
            else if (action == "tax") {
                player->tax();
                cout << "[ACTION] " << player->get_name() << " taxed." << endl;
                break;
            }
            else if (action == "coup" && target) {
                player->coup(*target);
                cout << "[ACTION] " << player->get_name() << " couped " << target->get_name() << "." << endl;
                break;
            }
            else if (action == "sanction" && target) {
                player->sanction(*target);
                cout << "[ACTION] " << player->get_name() << " sanctioned " << target->get_name() << "." << endl;
                break;
            }
            else if (action == "bribe") {
                player->bribe();
                cout << "[ACTION] " << player->get_name() << " bribed " << "." << endl;
                break;
            }
            else if (action == "arrest" && target) {
                player->arrest(*target);
                cout << "[ACTION] " << player->get_name() << " arrested " << target->get_name() << "." << endl;
                break;
            }
        } catch (const exception& e) {
            cout << "[EXCEPTION] " << player->get_name() << ": " << e.what() << endl;
        }
    }
}

int main() {
    Game& game = Game::get_instance();

    // Create players
    Player* baron1 = new Baron("Baron_1", game,false);
    Player* governor1 = new Governor("Governor_1",game,false);
    Player* baron2 = new Baron("Baron_2", game,false);
    Player* governor2 = new Governor("Governor_2", game,false);

    // Add to game
    game.add_player(baron1);
    game.add_player(governor1);
    game.add_player(baron2);
    game.add_player(governor2);

    cout << "🔁 Starting full simulation of Coup turns 🔁" << endl;

    const int rounds = 5;
    for (int r = 1; r <= rounds; ++r) {
        cout << "\n====== ROUND " << r << " ======" << endl;

        for (size_t i = 0; i < game.get_players().size(); ++i) {
            Player* current = game.current_player();

            if (!current->get_active()) {
                cout << "[SKIP] " << current->get_name() << " is inactive." << endl;
                game.next_turn();
                continue;
            }

            cout << "\n[TURN] " << current->get_name() << "'s turn:" << endl;
            this_thread::sleep_for(seconds(1));

            simulate_turn(current, game);

            this_thread::sleep_for(milliseconds(500));
            game.next_turn();
        }

        this_thread::sleep_for(seconds(1));
    }

    cout << "\n✅ Simulation ended.\n" << endl;
    return 0;
}
