// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef GAME_HPP
#define GAME_HPP
#include "../Player/Player.hpp"
#include "../../roles/General/General.hpp"
#include <vector>
#include <string>

using std::vector;


class Game{

    private:
    vector<Player*> players;
    size_t currentTurnIndex ;
    size_t coinPool;


    public:

    Game();
    ~Game();


    void add_player(Player* player);
    std::string turn()const;
    std::vector<std::string> active_players() const;
    std::string winner()const;
    vector<Player*> get_players();
    void notify_general_coup(Player &target,Player &executioner);
    Player *current_player();
    void next_turn();
    size_t& get_pool();
    bool is_human_turn();
    bool game_over();
    


};


#endif