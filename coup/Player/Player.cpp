// Mail: nachum.amit@msmail.ariel.ac.il

#include "Player.hpp"
#include <stdexcept>

Player::Player(const std::string& name,bool is_ai = false)
    : active(true), extra_turn(false), coins(5), name(name),actions_indicator(Actions::NumsAction,{false,nullptr}),is_ai(is_ai) {}

Player::Player(const Player& other) {
    this->active = other.active;
    this->name = other.name;
    this->coins = other.coins;
    this->is_ai = other.is_ai;
}

Player& Player::operator=(const Player &other) {
    if (this == &other) {
        return *this;
    }
    this->active = other.active;
    this->coins = other.coins;
    this->name = other.name;
    return *this;
}

Player::~Player() {}

std::string Player::get_name() const {
    return this->name;
}
bool Player::is_AI() const {
    return is_ai;
}

void Player::set_is_AI(bool value)
{
    this->is_ai = value;
}

int Player::get_coins() const {
    return this->coins;
}


bool Player::get_active() const {
    return this->active;
}

void Player::add_coins(int amount) {
    if (amount < 0) throw std::invalid_argument("Invalid amount value");
    this->coins += amount;
}

void Player::reduce_coins(int amount) {
    if (amount < 0) throw std::invalid_argument("Invalid amount value");
    this->coins -= amount;
}

void Player::remove_coins(int amount) {
    if (amount < 0) throw std::invalid_argument("Invalid amount value");
    if (amount > coins) throw std::runtime_error("Not enough coins");
    coins -= amount;
}

void Player::clear_extra_turn() {
    this->extra_turn = false;
}

void Player::eliminate() {
    this->active = false;
}

bool Player::has_extra_turn() const {
    return this->extra_turn;
}

bool Player::has_already_arrested(Player &player) {

    for(const std::string& name : arrested_names) {
        if (player.name == name) {
            return true;
        }
    }

    arrested_names.push_back(player.name);
    return false;
}


std::vector<Actions>& Player::get_blocked() {
    return blocked;
}

void Player::block_action(Actions action) {
    if (!is_blocked(action)) {
        blocked.push_back(action);
    }
}

bool Player::is_blocked(Actions to_block) const {
    if (blocked.empty()) {
        return false;
    }
    for (Actions action : blocked) {
        if (action == to_block) return true;
    }
    return false;
}

void Player::clear_blocked() {
    this->blocked.clear();
}

void Player::set_free_arrested(Player &player){
    for (const std::string& name : this->arrested_names){
        if(player.get_name() == name){
            arrested_names.erase(std::remove(arrested_names.begin(),arrested_names.end(),name),arrested_names.end());
            break;
        }
    }
    return;
}


vector<std::string>& Player::get_arrested(){
    return this->arrested_names;
}

void Player::clear_blocked_action(Actions act){

    for(const Actions &action : blocked){
        if (action == act)
        {
            blocked.erase(std::remove(blocked.begin(),blocked.end(),act),blocked.end());
        }
        
    }
}


void Player::set_action_indicator(Actions action,bool value,Player *player){
    this->actions_indicator[action] = {value,player};
}

vector<pair<bool,Player*>>& Player::get_action_indicator(){
    return this->actions_indicator;
}

void Player::set_active(bool val){
    this->active = val;
}


void Player::set_has_extra_turn(bool val){
    this->extra_turn = val;
}
