// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef GAME_HPP
#define GAME_HPP
#include "../Player/Player.hpp"
#include "../../roles/General/General.hpp"
#include "../../roles/Judge/Judge.hpp"
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using std::vector;


class Game{

    private:
    vector<Player*> players;
    size_t currentTurnIndex ;
    size_t coinPool;
    Game();
    Game(const Game& game) = delete;
    Game operator=(const Game& game) = delete;


    public:

    
    ~Game();
    static Game& get_instance(){
        static Game instance;
        return instance;
    }


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
    void notify_Judge_Bribe(Player &executioner);
    void reset();
    void set_index_turn(Player *player);
    


};


#endif