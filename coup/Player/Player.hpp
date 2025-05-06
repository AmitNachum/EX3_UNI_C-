// Mail: nachum.amit@msmail.ariel.ac.il

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector,std::pair;
 

enum Actions {
    Gather,
    Tax,
    Bribe,
    Arrest,
    Sanction,
    Coup
};

class Player{

    protected:
    vector<Actions> blocked;
    bool active;
    bool extra_turn;
    int coins;
    vector<std::string> arrested_names;
    std::string name;
    pair<bool,Player*> sanction_indicator;

    public:

    Player(const std::string& name);
    Player(const Player& other);
    Player& operator=(const Player &other);
    virtual ~Player();

    std::string get_name() const;
    void set_has_extra_turn(bool val);
    int get_coins()const;
    bool get_active()const;
    void add_coins(int amount);
    void reduce_coins(int amount);
    void remove_coins(int amount);
    void clear_extra_turn();
    void eliminate();
    bool has_extra_turn() const;
    bool has_already_arrested(Player &player);
    vector<Actions>& get_blocked();
    void block_action(Actions action);
    bool is_blocked(Actions to_block) const;
    void clear_blocked();
    void set_free_arrested(Player &Player);
    vector<std::string>& get_arrested();
    void clear_blocked_action(Actions act);
    void set_has_sanctioned(bool value,Player *player);
    pair<bool,Player *>& get_has_sanctioned();
    
    

    virtual void gather() = 0;
    virtual void tax() = 0;
    virtual void bribe() = 0;
    virtual void arrest(Player &player) = 0;
    virtual void sanction(Player &player) = 0;
    virtual void coup(Player &player) = 0;
    virtual void undo(Player &player) = 0;
    virtual std::string get_role_name() const = 0;
   

};

#endif