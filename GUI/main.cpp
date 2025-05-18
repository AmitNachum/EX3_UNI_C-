// Minimal Coup GUI - Cleaned and Refactored (Enhanced with Coin Display, Block Tax, Hover Highlight, AI and Human Turn Logic)

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "../coup/Game/Game.hpp"
#include "../coup/Role/Factory/Factory.hpp"
#include "../coup/Role/Strategy/Strategy.hpp"
#include <unordered_map>
#include <random>
#include <thread>
#include <chrono>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;

struct PlayerConfig {
    std::string name;
    Roles role;
    bool isAI;
};

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

std::string check_winner(Game &game) {
    int active_count = 0;
    Player* last_active = nullptr;

    for (Player* p : game.get_players()) {
        if (p->get_active()) {
            active_count++;
            last_active = p;
        }
    }

    if (active_count == 1 && last_active) {
        return "Winner: " + last_active->get_name();
    }

    return "";
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Coup - Minimal GUI");
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Could not load font." << std::endl;
        return 1;
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("GUI/Assets/background.png")) {
        std::cerr << "Failed to load background image." << std::endl;
        return 1;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(
        float(WINDOW_WIDTH) / backgroundTexture.getSize().x,
        float(WINDOW_HEIGHT) / backgroundTexture.getSize().y
    );

    Game game;
    std::vector<PlayerConfig> configs;
    std::vector<Player*> createdPlayers;
    std::unordered_map<Player*, AIaction*> aiMap;
    Player* humanPlayer = nullptr;
    Player* selectedTarget = nullptr;

    sf::Text msg("", font, 20);
    msg.setPosition(20, 660);

    std::vector<Button> menuButtons = {
        Button(50, 500, 200, 40, "Add AI", font),
        Button(50, 550, 200, 40, "Add Human", font),
        Button(50, 600, 200, 40, "Start Game", font)
    };

    std::vector<Button> actionButtons;

    bool gameStarted = false;
    bool typing = false;
    std::string humanName = "";
    sf::Text inputText("", font, 18);
    inputText.setPosition(50, 450);
    inputText.setFillColor(sf::Color::Black);
    sf::RectangleShape inputBox(sf::Vector2f(200, 30));
    inputBox.setPosition(45, 445);
    inputBox.setFillColor(sf::Color::White);

    std::vector<std::string> aiNames = {"Zeta", "Nova", "Orion", "Quasar", "Luna", "Astra"};
    std::vector<Roles> allRoles = {ROLE_GENERAL, ROLE_GOVERNOR, ROLE_BARON, ROLE_JUDGE, ROLE_MERCHANT, ROLE_SPY};

    bool leftMouseReleased = true;

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameStarted) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (menuButtons[0].isHovered(mousePos)) {
                        std::string name = aiNames[configs.size() % aiNames.size()];
                        configs.push_back({name, allRoles[rand() % allRoles.size()], true});
                        msg.setString("AI Player Added: " + name);
                    } else if (menuButtons[1].isHovered(mousePos)) {
                        if (std::none_of(configs.begin(), configs.end(), [](const PlayerConfig& cfg) { return !cfg.isAI; })) {
                            typing = true;
                            msg.setString("Enter human name:");
                        } else {
                            msg.setString("Only one human player allowed.");
                        }
                    } else if (menuButtons[2].isHovered(mousePos)) {
                        if (configs.size() >= 2) {
                            for (auto& cfg : configs) {
                                Player* p = FactoryPlayers::createPlayer(cfg.role, cfg.name, game, cfg.isAI);
                                game.add_player(p);
                                createdPlayers.push_back(p);
                                if (!cfg.isAI) humanPlayer = p;
                                if (cfg.isAI) aiMap[p] = new AIaggresive();
                            }
                            gameStarted = true;
                        } else {
                            msg.setString("Add at least 2 players.");
                        }
                    }
                } else if (typing && event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8 && !humanName.empty()) {
                        humanName.pop_back();
                    } else if ((event.text.unicode == 13 || event.text.unicode == 10) && !humanName.empty()) {
                        configs.push_back({humanName, ROLE_GOVERNOR, false});
                        typing = false;
                        msg.setString("Human player added: " + humanName);
                    } else if (event.text.unicode < 128 && humanName.length() < 20) {
                        humanName += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(humanName);
                }
            }
        }

        window.clear();
        window.draw(background);

        if (!gameStarted) {
            for (auto& b : menuButtons) {
                b.shape.setFillColor(b.isHovered(mousePos) ? sf::Color(130, 130, 255) : sf::Color(100, 100, 250));
                b.draw(window);
            }
            if (typing) {
                window.draw(inputBox);
                window.draw(inputText);
            }
            window.draw(msg);
            window.display();
            continue;
        }

        Player* current = game.current_player();

        // AI TURN HANDLER
        if (current->is_AI()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            Player* target = nullptr;
            for (Player* p : game.get_players()) {
                if (p != current && p->get_active()) {
                    target = p;
                    break;
                }
            }
            try {
                if (!target)
                    throw std::runtime_error("AI has no valid target");
                if (aiMap.find(current) == aiMap.end())
                    throw std::runtime_error("AI map missing for current player");
                aiMap[current]->favorite_action(current, *target);
            } catch (const std::exception& e) {
                msg.setString("AI error: " + std::string(e.what()));
            }
            std::string winner = check_winner(game);
            if (!winner.empty()) {
                msg.setString(winner + " - Game Over");
                window.draw(msg);
                window.display();
                continue;
            }
            game.next_turn();
            continue;
        }

        actionButtons.clear();
        int startY = 100;
        int spacing = 60;

        std::vector<std::string> labels = {
            "Tax", "Gather", "Sanction", "Bribe", "Coup", "Arrest"
        };

        for (const std::string& label : labels) {
            actionButtons.emplace_back(600, startY, 150, 40, label, font);
            startY += spacing;
        }

        if (current->get_role_name() == "Baron") {
            actionButtons.emplace_back(600, startY, 150, 40, "Invest", font);
            startY += spacing;
        }
        if (current->get_role_name() == "Governor") {
            actionButtons.emplace_back(600, startY, 150, 40, "Block Tax", font);
            startY += spacing;
        }
        if (current->get_role_name() == "Spy") {
            actionButtons.emplace_back(600, startY, 150, 40, "Block Arrest", font);
            startY += spacing;
        }

        // LEFT MOUSE CLICK DETECTION (only on release)
        static bool wasPressed = false;
        bool nowPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (wasPressed && !nowPressed) {
            int target_index = 0;
            for (Player* p : game.get_players()) {
                if (p != humanPlayer && p->get_active()) {
                    sf::FloatRect targetRect(50, 100 + 50 * target_index, 180, 40);
                    if (targetRect.contains(static_cast<sf::Vector2f>(mousePos))) {
                        selectedTarget = p;
                    }
                    target_index++;
                }
            }
            for (auto& btn : actionButtons) {
                if (btn.isHovered(mousePos)) {
                    try {
                        if (btn.label == "Tax") current->tax();
                        else if (btn.label == "Gather") current->gather();
                        else if (btn.label == "Sanction" && selectedTarget) current->sanction(*selectedTarget);
                        else if (btn.label == "Bribe") current->bribe();
                        else if (btn.label == "Coup" && selectedTarget) current->coup(*selectedTarget);
                        else if (btn.label == "Arrest" && selectedTarget) current->arrest(*selectedTarget);
                        else if (btn.label == "Invest") {
                            if (current->get_role_name() != "Baron") throw std::runtime_error("Only Barons can invest");
                            dynamic_cast<Baron*>(current)->invest();
                        }
                        else if (btn.label == "Block Tax") {
                            if (!selectedTarget) throw std::runtime_error("No target selected for block");
                            if (current->get_role_name() != "Governor") throw std::runtime_error("Only Governor can block tax");
                            dynamic_cast<Governor*>(current)->block_tax(*selectedTarget);
                        }
                        else if (btn.label == "Block Arrest") {
                            if (!selectedTarget) throw std::runtime_error("No target selected for block");
                            if (current->get_role_name() != "Spy") throw std::runtime_error("Only Spy can block arrest");
                            dynamic_cast<Spy*>(current)->block_arrest(*selectedTarget);
                        } else {
                            throw std::runtime_error("Action not valid or no target selected.");
                        }

                        std::string winner = check_winner(game);
                        if (!winner.empty()) {
                            msg.setString(winner + " - Game Over");
                            continue;
                        }

                        if (current->has_extra_turn()) current->clear_extra_turn();
                        else game.next_turn();
                    } catch (const std::exception& ex) {
                        msg.setString(ex.what());
                    }
                }
            }
        }
        wasPressed = nowPressed;

        sf::Text turnText("Turn: " + current->get_name(), font, 20);
        turnText.setPosition(20, 20);
        turnText.setFillColor(sf::Color::White);
        window.draw(turnText);

        if (humanPlayer) {
            sf::Text roleText("Your Role: " + humanPlayer->get_role_name(), font, 20);
            roleText.setPosition(WINDOW_WIDTH - 240, 20);
            roleText.setFillColor(sf::Color::Cyan);
            window.draw(roleText);

            sf::Text coinText("Coin Pile: " + std::to_string(humanPlayer->get_coins()), font, 20);
            coinText.setPosition(WINDOW_WIDTH - 240, 50);
            coinText.setFillColor(sf::Color::Yellow);
            window.draw(coinText);
        }

        int boxX = 50, boxY = 100, boxHeight = 40;
        for (Player* p : game.get_players()) {
            if (p != humanPlayer && p->get_active()) {
                sf::RectangleShape box(sf::Vector2f(180, boxHeight));
                box.setPosition(boxX, boxY);
                box.setFillColor(box.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ? sf::Color::Red : sf::Color(60, 60, 60));
                box.setOutlineThickness(2);
                box.setOutlineColor(sf::Color::White);
                sf::Text nameText(p->get_name(), font, 18);
                nameText.setPosition(boxX + 10, boxY + 8);
                nameText.setFillColor(sf::Color::White);
                window.draw(box);
                window.draw(nameText);
                boxY += boxHeight + 10;
            }
        }

        for (auto& btn : actionButtons) {
            btn.shape.setFillColor(btn.isHovered(mousePos) ? sf::Color::Green : sf::Color(100, 100, 250));
            btn.draw(window);
        }

        window.draw(msg);
        window.display();
    }

    for (auto& [p, ai] : aiMap) delete ai;
    return 0;
}
