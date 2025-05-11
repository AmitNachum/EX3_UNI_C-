// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP
#include "../../coup/Player/Player.hpp"
#include "../../coup/Game/Game.hpp"

class Governor:public Player{

    Game &game;


    public:

    Governor(const std::string &name,Game& game,bool is_ai) : Player(name,is_ai),game(game){};
    ~Governor(){};

    Governor(const Governor& other): Player(other),game(other.game){};

    Governor &operator=(const Governor &other) = delete;

     void gather() override;
     void tax() override;
     void bribe() override;
     void arrest(Player &player) override; 
     void sanction(Player &player) override;
     void coup(Player &player) override;
     void undo(Player &player) override;
     std::string get_role_name() const override;
     Game& get_game();





};





#endif