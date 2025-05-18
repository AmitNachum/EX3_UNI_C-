// Mail: nachum.amit@msmail.ariel.ac.il

#include "Game.hpp"
#include <random>
#include <SFML/Graphics.hpp>

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
        Player *curr = players.at(currentTurnIndex);

        if(curr->get_active()){ 
            break;
        }

        attempts++;
        
    }


}


size_t& Game::get_pool(){
    return coinPool;
}

vector<Player*> Game::get_players(){
    return this->players;
}


void Game::notify_general_coup(Player &target, Player &executioner) {

    for (Player* p : players) {
        if (!p->get_active()) continue;
        if (p == &executioner && p != &target) continue;
        if (p->get_role_name() != "General") continue;

        General* general = dynamic_cast<General*>(p);
        if (!general) continue;

     
       if (general->is_AI()) {

        if(general == &target){
            try {
                if(p->get_coins() < 5){ 
                std::cout << "General cannot prevent the coup — not enough coins." << std::endl;
                continue;
               }
            general->prevent_coup(target);
            std::cout << general->get_name() << " (AI) saved itself from a coup!" << std::endl;

            } catch (const std::exception& e) {
            std::cout << general->get_name() << " (AI) failed to save itself: " << e.what() << std::endl;
        }
            continue;
        }


        int decision = rand() % 2;
        if (decision == 1) {
            try {
                general->prevent_coup(target);
                std::cout << general->get_name() << " (AI) prevented the coup against " << target.get_name() << "\n";
                return; 
            }  catch (const std::exception& e) {
                if (std::string(e.what()).find("Not the") != std::string::npos &&
                    std::string(e.what()).find("turn") != std::string::npos) {
                    std::cout << "🛡️ General tried to prevent a coup out of turn. Skipping...\n";
            } else {
                throw;
            }
        }
    } else {
        std::cout << general->get_name() << " (AI) chose NOT to save " << target.get_name() << std::endl;
    }
}


        
               else {
            sf::RenderWindow window(sf::VideoMode(400, 220), "Prevent Coup?");
            window.setVisible(true);
            window.requestFocus(); 
            sf::Font font;
            if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
                std::cerr << "Error: could not load font (DejaVuSans)\n";
                return;
            }

            sf::Text info("General " + p->get_name() + ",\n" + executioner.get_name() + " is about to coup " + target.get_name(), font, 16);
            info.setFillColor(sf::Color::White);
            info.setPosition(20, 10);

            sf::Text question("Prevent coup on " + target.get_name() + "?", font, 18);
            question.setFillColor(sf::Color::White);
            question.setPosition(40, 50);

            sf::Text errorText("", font, 16);
            errorText.setFillColor(sf::Color::Red);
            errorText.setPosition(40, 90);  // Message will show here if error

            sf::Text yesText("Yes", font, 18);
            sf::Text noText("No", font, 18);
            yesText.setFillColor(sf::Color::Black);
            noText.setFillColor(sf::Color::Black);
            yesText.setPosition(95, 160);
            noText.setPosition(295, 160);

            sf::RectangleShape yesButton(sf::Vector2f(100, 40));
            yesButton.setFillColor(sf::Color::Green);
            yesButton.setPosition(70, 150);

            sf::RectangleShape noButton(sf::Vector2f(100, 40));
            noButton.setFillColor(sf::Color::Red);
            noButton.setPosition(270, 150);

            bool decisionMade = false;
            while (window.isOpen() && !decisionMade) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2i mouse = sf::Mouse::getPosition(window);
                        if (yesButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                            try {
                                general->prevent_coup(target);
                                decisionMade = true;
                                window.close();
                            } catch (const std::exception& ex) {
                                errorText.setString(ex.what());  // Show error in popup
                            }
                        } else if (noButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                            decisionMade = true;
                            window.close();
                        }
                    }
                }

                window.clear(sf::Color::Black);
                window.draw(info);
                window.draw(question);
                window.draw(errorText);
                window.draw(yesButton);
                window.draw(noButton);
                window.draw(yesText);
                window.draw(noText);
                window.display();
            }
        }
    }
}


bool Game::is_human_turn(){
    return !(this->current_player()->is_AI());
}

bool Game::game_over()
{
    int alive = 0;

    for(auto *player : players){
        if(player->get_active()){
            alive++;
        }
    }

    return alive <= 1;
}
