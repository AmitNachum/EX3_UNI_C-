// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "../../coup/Player/Player.hpp"
#include "../../coup/Game/Game.hpp"


class Judge : public Player {

    private:
        Game &game;


    public:
    
    Judge(const std::string& name,Game &game,bool is_ai) : Player(name,is_ai),game(game){};
    ~Judge(){};
    Judge(const Judge &other):Player(other),game(other.game){};
    Judge &operator=(const Judge& other) = delete;

    void gather() override;
    void tax() override;
    void bribe() override;
    void arrest(Player &player) override; 
    void sanction(Player &player) override;
    void coup(Player &player) override;
    void undo(Player &player) override;
    std::string get_role_name() const override;
    Game& get_game();
    void block_Bribe();


    void handle_sanctions(){
        if (!(this->get_action_indicator()[Sanction].first) || this->get_action_indicator()[Sanction].second == nullptr) return;
    
        this->get_action_indicator()[Sanction].second->set_has_extra_turn(false);
        std::cout << "Sanction has been canceled by the Judge" << std::endl;
        this->clear_blocked();
    
        this->get_action_indicator()[Sanction] = {false, nullptr}; 
    }
    









};












#endif