// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef BRAON_HPP
#define BARON_HPP

#include "../../coup/Player/Player.hpp"
#include "../../coup/Game/Game.hpp"

class Baron: public Player {

    private:
    Game &game;



    public:

    Baron(const std::string& name, Game &game):Player(name),game(game){};
    ~Baron(){};
    Baron(const Baron &other):Player(other),game(other.game){};
    Baron& operator=(Baron &other) = delete;



    void gather() override;
    void tax() override;
    void bribe() override;
    void arrest(Player &player) override; 
    void sanction(Player &player) override;
    void coup(Player &player) override;
    void undo(Player &player) override;
    std::string get_role_name() const override;

    
    void handle_sanction_bonus();
    void invest();







};
#endif