// Place this in your GUI main file

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "../coup/Game/Game.hpp"
#include "../coup/Role/Factory/Factory.hpp"
#include "../coup/Role/Strategy/Strategy.hpp"
#include <unordered_map>
#include <chrono>
#include <thread>
#include <random>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;


void checkForWinner(Game& game, sf::Text& msg) {
    int activeCount = 0;
    Player* lastActive = nullptr;
    for (Player* p : game.get_players()) {
        if (p->get_active()) {
            activeCount++;
            lastActive = p;
        }
    }

    if (activeCount == 1 && lastActive) {
        msg.setString("Winner: " + lastActive->get_name());
    }
}


struct PlayerConfig {
    std::string name;
    Roles role;
    bool isAI;
};

Player *player = nullptr;

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::string label;

    Button(float x, float y, float width, float height, const std::string& label, const sf::Font& font)
        : label(label)
    {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(100, 100, 250));

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(x + width / 2.0f, y + height / 2.0f);
    }

    bool isHovered(const sf::Vector2i& mousePos) const {
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    void draw(sf::RenderWindow& win) const {
        win.draw(shape);
        win.draw(text);
    }
};

struct PlayerBox {
    sf::RectangleShape box;
    sf::Text nameText;
    Player* player;

    PlayerBox(float x, float y, const std::string& name, const sf::Font& font, Player* p)
        : player(p)
    {
        box.setPosition(x, y);
        box.setSize(sf::Vector2f(140, 50));
        box.setFillColor(sf::Color(60, 60, 60));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2);

        nameText.setFont(font);
        nameText.setString(name);
        nameText.setCharacterSize(16);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(x + 10, y + 15);
    }

    bool isHovered(const sf::Vector2i& mousePos) const {
        return box.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    void draw(sf::RenderWindow& win) const {
        win.draw(box);
        win.draw(nameText);
    }
};

std::vector<std::string> aiNames = {"Zeta", "Nova", "Orion", "Quasar", "Luna", "Astra"};
std::vector<Roles> allRoles = { ROLE_GENERAL, ROLE_GOVERNOR, ROLE_BARON, ROLE_JUDGE, ROLE_MERCHANT, ROLE_SPY };
Player* spyTarget = nullptr;

int main() {
    srand(static_cast<unsigned>(time(nullptr))); 
    

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Coup GUI");
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Could not load font.\n";
        return 1;
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("GUI/Assets/background.png")) {
        std::cerr << "Failed to load background image.\n";
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // Scale to fit window size
    float scaleX = static_cast<float>(WINDOW_WIDTH) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);

    Game game;
    std::vector<Player*> createdPlayers;
    std::unordered_map<Player*, AIaction*> ai_map;
    std::vector<Button> fixedButtons, targetButtons;
    std::vector<PlayerBox> playerBoxes;
    std::vector<PlayerConfig> playerConfigs;

    Button addAI(100, 500, 250, 50, "Add AI Player", font);
    Button addHuman(100, 570, 250, 50, "Add Human Player", font);
    Button startGame(100, 640, 250, 50, "Start Game", font);
    sf::Text msg("", font, 20); msg.setPosition(100, 450);
    sf::RectangleShape inputBox(sf::Vector2f(200, 30));
    inputBox.setPosition(400, 185);
    sf::Text inputText("", font, 18); inputText.setPosition(405, 190); inputText.setFillColor(sf::Color::Black);

    std::string humanName = "";
    bool typingName = false, gameStarted = false;
    Player* selectedTarget = nullptr;

    //fixedButtons.emplace_back(600, 100, 150, 40, "Tax", font);
    //fixedButtons.emplace_back(600, 160, 150, 40, "Gather", font);


    targetButtons.emplace_back(600, 250, 150, 40, "Coup", font);
    targetButtons.emplace_back(600, 310, 150, 40, "Arrest", font);
    targetButtons.emplace_back(600, 370, 150, 40, "Sanction", font);
    targetButtons.emplace_back(600, 430, 150, 40, "Bribe", font);

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (!gameStarted) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (addAI.isHovered(mousePos) && playerConfigs.size() < 6) {
                        std::string name = aiNames[playerConfigs.size() % aiNames.size()];
                        playerConfigs.push_back({name, allRoles[rand() % allRoles.size()], true});
                        msg.setString("AI Player Added: " + name);
                    } else if (addHuman.isHovered(mousePos)) {
                        if (playerConfigs.size() < 6 && std::none_of(playerConfigs.begin(), playerConfigs.end(), [](const PlayerConfig& cfg) { return !cfg.isAI; })) {
                            typingName = true;
                            msg.setString("Enter your name:");
                        } else {
                            msg.setString("Only one human player allowed.");
                        }
                    } else if (startGame.isHovered(mousePos)) {
                        if (playerConfigs.size() >= 2) {
                            for (const auto& cfg : playerConfigs) {
                                Player* p = FactoryPlayers::createPlayer(cfg.role, cfg.name, game, cfg.isAI);
                                game.add_player(p);
                                createdPlayers.push_back(p);
                                if(!cfg.isAI) player = p;
                                if (cfg.isAI) ai_map[p] = new AIaggresive();
                            }
                            for (size_t i = 0; i < createdPlayers.size(); ++i) {
                                playerBoxes.emplace_back(100 + (i % 3) * 160, 500 + (i / 3) * 60,
                                    createdPlayers[i]->get_name(), font, createdPlayers[i]);
                            }
                            gameStarted = true;
                        } else {
                            msg.setString("Need at least 2 players.");
                        }
                    }
                }

                if (typingName && event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8 && !humanName.empty()) humanName.pop_back();
                    else if ((event.text.unicode == 13 || event.text.unicode == 10) && !humanName.empty()) {
                        playerConfigs.push_back({humanName, allRoles[rand() % allRoles.size()], false});
                        typingName = false;  
                    } else if (event.text.unicode < 128 && humanName.length() < 20) {
                        humanName += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(humanName);
                }
            }
        }
        

        if (!gameStarted || game.get_players().empty()) {
            window.clear();
            window.draw(backgroundSprite);
            for (auto* b : {&addAI, &addHuman, &startGame}) {
                b->shape.setFillColor(b->isHovered(sf::Mouse::getPosition(window)) ? sf::Color(130, 130, 255) : sf::Color(100, 100, 250));
                b->draw(window);
            }
            window.draw(msg);
            if (typingName) {
                window.draw(inputBox);
                window.draw(inputText);
            }
            window.display();
            continue;
        }


        
            if (event.type == sf::Event::Closed) window.close();
                        if (!gameStarted) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (addAI.isHovered(mousePos) && playerConfigs.size() < 6) {
                        std::string name = aiNames[playerConfigs.size() % aiNames.size()];
                        playerConfigs.push_back({name, allRoles[playerConfigs.size() % allRoles.size()], true});
                        msg.setString("AI Player Added: " + name);
                    } else if (addHuman.isHovered(mousePos)) {
                        if (playerConfigs.size() < 6 && std::none_of(playerConfigs.begin(), playerConfigs.end(), [](const PlayerConfig& cfg) { return !cfg.isAI; })) {
                            typingName = true;
                            msg.setString("Enter your name:");
                        } else {
                            msg.setString("Only one human player allowed.");
                        }
                    }
                    } else if (startGame.isHovered(mousePos)) {
                        if (playerConfigs.size() >= 2) {
                            for (const auto& cfg : playerConfigs) {
                                Player* p = FactoryPlayers::createPlayer(cfg.role, cfg.name, game, cfg.isAI);
                                game.add_player(p);
                                createdPlayers.push_back(p);
                                if (cfg.isAI) ai_map[p] = new AIaggresive();
                            }
                            for (size_t i = 0; i < createdPlayers.size(); ++i) {
                                playerBoxes.emplace_back(100 + (i % 3) * 160, 500 + (i / 3) * 60,
                                    createdPlayers[i]->get_name(), font, createdPlayers[i]);
                            }
                            gameStarted = true;
                        } else {
                            msg.setString("Need at least 2 players.");
                        }
                    }
                }

                if (typingName && event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8 && !humanName.empty()) humanName.pop_back();
                    else if ((event.text.unicode == 13 || event.text.unicode == 10) && !humanName.empty()) {
                        playerConfigs.push_back({humanName, allRoles[playerConfigs.size() % allRoles.size()], false});
                        typingName = false;
                        msg.setString("Human player added: " + humanName);
                    } else if (event.text.unicode < 128 && humanName.length() < 20) {
                        humanName += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(humanName);
                    
                }
             else {
                if (!game.is_human_turn()) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    Player* current = game.current_player();
                    fixedButtons.clear();
                    fixedButtons.emplace_back(600, 100, 150, 40, "Tax", font);
                    fixedButtons.emplace_back(600, 160, 150, 40, "Gather", font);

                    Player* target = nullptr;
                    for (Player* p : game.get_players()) {
                        if (p != current && p->get_active()) {
                            target = p;
                            break;
                        }
                    }
                    if (target) ai_map[current]->favorite_action(current, *target);
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    Player* current = game.current_player();
                                 // After drawing turn text
                    fixedButtons.clear();
                    fixedButtons.emplace_back(600, 100, 150, 40, "Tax", font);
                    fixedButtons.emplace_back(600, 160, 150, 40, "Gather", font);
                    if (current->get_role_name() == "Baron" && !current->is_AI()) {
                        fixedButtons.emplace_back(600, 220, 150, 40, "Invest", font);
                    }
      
                    for (auto& box : playerBoxes) {
                        if (box.isHovered(mousePos)) selectedTarget = box.player;
                        if (current->get_role_name() == "Spy" && !current->is_AI()) {
                            spyTarget = selectedTarget;
                        }
                        
                    }
                    for (auto& btn : fixedButtons) {
                        if (btn.isHovered(mousePos)) {
                            try {
                                if (btn.label == "Tax") {
                                    current->tax();
                                    checkForWinner(game, msg);
                                }
                                else if (btn.label == "Gather"){
                                    current->gather();
                                    checkForWinner(game, msg);
                                }
                                else if (btn.label == "Invest") {
                                    Baron* baron = dynamic_cast<Baron*>(current);
                                    if (baron) baron->invest();
                                    else msg.setString("Only Barons can invest.");
                                }
                                
                            } catch (const std::exception& ex) {
                                msg.setString("Action failed: " + std::string(ex.what()));
                            }
                        }
                    }
                    for (auto& btn : targetButtons) {
                        if (btn.isHovered(mousePos) && selectedTarget) {
                            try {
                                if (btn.label == "Coup"){ 
                                    current->coup(*selectedTarget);
                                    checkForWinner(game, msg);
                                       if (!selectedTarget->get_active()) {
                                        selectedTarget = nullptr;
                                        }
                                }
                                else if (btn.label == "Arrest") {
                                    current->arrest(*selectedTarget);
                                    checkForWinner(game, msg);
                                }
                                else if (btn.label == "Sanction") {
                                    current->sanction(*selectedTarget);
                                    checkForWinner(game, msg);
                                }
                                else if (btn.label == "Bribe"){
                                     current->bribe();
                                     checkForWinner(game, msg);
                                    }
                            
                            } catch (const std::exception& ex) {
                                msg.setString("Action failed: " + std::string(ex.what()));
                            }
                        }
                    }
                }
            }
        

        window.clear();
        window.draw(backgroundSprite);

        if (!gameStarted) {
            for (auto* b : {&addAI, &addHuman, &startGame}) {
                b->shape.setFillColor(b->isHovered(mousePos) ? sf::Color(130, 130, 255) : sf::Color(100, 100, 250));
                b->draw(window);
            }
            window.draw(msg);
            if (typingName) {
                window.draw(inputBox);
                window.draw(inputText);
            }
        } else {
            Player* current = game.current_player();
            sf::Text turnText("Turn: " + current->get_name(), font, 20);
            turnText.setPosition(20, 20);
            fixedButtons.clear();
            fixedButtons.emplace_back(600, 100, 150, 40, "Tax", font);
            fixedButtons.emplace_back(600, 160, 150, 40, "Gather", font);
            if (current->get_role_name() == "Baron" && !current->is_AI()) {
                fixedButtons.emplace_back(600, 220, 150, 40, "Invest", font);
            }


            
                if(player){
                sf::Text roleText("Role: " + player->get_role_name(), font, 20);
                roleText.setFillColor(sf::Color::Cyan);
                roleText.setPosition(WINDOW_WIDTH - 200, 20); // Top-right
                window.draw(roleText);
                sf::Text coinText("Coins: " + std::to_string(player->get_coins()), font, 20);
                coinText.setFillColor(sf::Color::Yellow);
                coinText.setPosition(WINDOW_WIDTH - 200, 50);
                window.draw(coinText);
                sf::Text coinCache("Coins Cache:", font, 20);
                }
            

                
            window.draw(turnText);

            if (!current->is_AI()) {
                sf::Text coinText("Coins: " + std::to_string(current->get_coins()), font, 20);
                coinText.setFillColor(sf::Color::Yellow);
                coinText.setPosition(650, 100);
                window.draw(coinText);
            }

            for (auto& b : fixedButtons) {
                b.shape.setFillColor(b.isHovered(mousePos) ? sf::Color::Green : sf::Color(100, 100, 250));
                b.draw(window);
            }

            for (auto& b : targetButtons) {
                b.shape.setFillColor((selectedTarget && b.isHovered(mousePos)) ? sf::Color::Red : sf::Color(80, 80, 180));
                b.draw(window);
            }

            for (auto& pb : playerBoxes) {
                pb.box.setFillColor(pb.isHovered(mousePos) ? sf::Color(180, 180, 60) : sf::Color(60, 60, 60));
                pb.draw(window);
            }

            if (current->get_role_name() == "Spy" && !current->is_AI() && spyTarget && spyTarget->get_active()) {
                sf::Text spyInfo("Spy Target: " + spyTarget->get_name() + " | Coins: " 
                + std::to_string(spyTarget->get_coins()), font, 16);
                spyInfo.setFillColor(sf::Color::Red);
                spyInfo.setPosition(20, 650);
                window.draw(spyInfo);
            }
            

            window.draw(msg);
        }

        window.display();
    }

    for (auto& [k, v] : ai_map) delete v;
    return 0;
}
