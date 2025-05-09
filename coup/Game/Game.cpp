// Mail: nachum.amit@msmail.ariel.ac.il

#include "Game.hpp"


using std::cout,std::string,std::vector;


Game::Game() : currentTurnIndex(0), coinPool(50){};
Game::~Game(){
    for(Player *player : players){
        delete player;
    }
    players.clear();
}


void Game::add_player(Player* player){

    if (players.size() >= 6) throw std::runtime_error("Maximum number of players reached");
    if (player == nullptr) throw std::runtime_error("Invalid Input");


    players.push_back(player);
}

string Game::turn()const{

    if (players.empty()) throw std::runtime_error("No Players in the Game");
    
    return players.at(currentTurnIndex)->get_name();

}


Player* Game::current_player(){

    if (players.empty()) throw std::runtime_error("No Players in the Game");

    return players.at(currentTurnIndex);

}


vector<string> Game::active_players()const{
    vector<string> current_actives;
    for(Player *player : players){ 

        if (player->get_active()) current_actives.push_back(player->get_name());
        
    }

    return current_actives;
}


string Game::winner() const{

int remaining = 0;
int winner_index = 0;
int running_index = 0;

    for(Player *player : players){
        if (player->get_active()) {
            remaining++;
            winner_index = running_index;
        }

        running_index++;
    }

    if (remaining == 1) {
        return players.at(winner_index)->get_name();
    }

    throw std::runtime_error("Game is still in progress — no winner yet.");

}

void Game::next_turn(){
    if (players.empty()) throw std::runtime_error("No players are Present");

    int total = players.size();


    int attempts = 0;

    while (attempts < total)
    {
        currentTurnIndex = ( currentTurnIndex + 1 ) % total;

        if(players.at(currentTurnIndex)->get_active()){ 
            return;
        }

        attempts++;
        
    }



    throw std::runtime_error("No active players left to take a turn.");
}


size_t& Game::get_pool(){
    return coinPool;
}

vector<Player*> Game::get_players(){
    return this->players;
}

void Game::notify_general_coup(Player &target,Player &executioner){
    for (Player* p : players) {
        if(!(p->get_active())) continue;
        if( p == &executioner && p != &target) continue;


        if (p->get_role_name() == "General") {
            General* general = dynamic_cast<General*>(p);
            
            if (general != nullptr) {
                std::cout << "General " << general->get_name()
                          << ", do you want to prevent a coup on " << target.get_name()
                          << "? (y/n): ";
                char choice;
                std::cin >> choice;
                choice = std::tolower(choice);
                if (choice == 'y') {
                general->prevent_coup(target);
                }
                          
            }
        }
    }
    
}
