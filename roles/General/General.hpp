// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef GENERAL_HPP
#define GENERAL_HPP
#include "../../coup/Player/Player.hpp"  
#include "../../coup/Game/Game.hpp"      

using std::string, std::vector, std::pair;


class General : public Player{
    private:
    Game& game;

    public:

    General(const string& name, Game& game,bool is_ai): Player(name,is_ai),game(game){};
    ~General(){};
    General(const General& other) : Player(other),game(other.game){};
    General& operator=(const General &other) = delete;


    void gather();
    void tax(); 
    void bribe();
    void arrest(Player &player);
    void sanction(Player &player);
    void coup(Player &player);
    void undo(Player &player);
    std::string get_role_name() const;

    void prevent_coup(Player &Player);
    void handle_arrests();
    void revive_player(Player &Player);
};







#endif