// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef MERCHANT_HPP
#define MERCHANT_HPP
#include "../../coup/Player/Player.hpp"
#include "../../coup/Game/Game.hpp"


class Merchant : public Player{

    private:
    Game &game;

    public:
    Merchant(const std::string &name,Game &game): Player(name),game(game){};
    ~Merchant(){};
    Merchant(const Merchant& other):Player(other),game(other.game){};
    Merchant& operator=(const Merchant& other) = delete;

    void gather() override;
    void tax() override;
    void bribe() override;
    void arrest(Player &player) override; 
    void sanction(Player &player) override;
    void coup(Player &player) override;
    void undo(Player &player) override;
    std::string get_role_name() const override;
    void handle_discount();
    void handle_arrest();
    void handle_passive_effects();
    




};





#endif