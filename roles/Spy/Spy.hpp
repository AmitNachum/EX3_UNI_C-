// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef SPY_HPP
#define SPY_HPP
#include "../../coup/Player/Player.hpp"
#include "../../coup/Game/Game.hpp"
using std::string, std::vector, std::pair;


class Spy : public Player{
    private:
    Game &game;
    
    public:
        Spy(const string& name, Game& game,bool is_ai):Player(name,is_ai),game(game){};
        Spy(Spy &other):Player(other),game(other.game){};
        ~Spy(){};
        Spy &operator=(Spy& other) = delete;

        void gather() override;
        void tax() override;
        void bribe() override;
        void arrest(Player &player) override; 
        void sanction(Player &player) override;
        void coup(Player &player) override;
        void undo(Player &player) override;
        string view_pile(Player &player);
        void block_arrest(Player &player);
        std::string get_role_name() const;
};










#endif