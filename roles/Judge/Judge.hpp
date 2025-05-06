// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "../../coup/Player/Player.hpp"
#include "../../coup/Game/Game.hpp"


class Judge : public Player {

    private:
        Game &game;


    public:
    
    Judge(const std::string& name,Game &game) : Player(name),game(game){};
    ~Judge(){};
    Judge(const Judge &other):Player(other),game(other.game){};
    Judge &operator=(const Judge& other){
        
        if(this == &other) return *this;

        Player::operator=(other);

        this->name = other.name;

        return *this;
    }

    void gather() override;
    void tax() override;
    void bribe() override;
    void arrest(Player &player) override; 
    void sanction(Player &player) override;
    void coup(Player &player) override;
    void undo(Player &player) override;
    std::string get_role_name() const override;
    Game& get_game();


    void handle_sanctions(std::pair<bool, Player*>& pair){
        if (!pair.first || pair.second == nullptr) return;
    
        pair.second->set_has_extra_turn(false);
        std::cout << "Sanction has been canceled by the Judge" << std::endl;
    
        pair = {false, nullptr}; 
    }
    









};












#endif