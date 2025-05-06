// Mail: nachum.amit@msmail.ariel.ac.il

#include "Governor.hpp"



void Governor::gather(){
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Governer's turn");

    if (game.get_pool() < 1) 
        throw std::runtime_error("No money to gather");

    if(this->is_blocked(Actions::Gather)){
        std::cout << "Gather is blocked" <<std::endl;
        this->clear_blocked();
    }    

    game.get_pool()--;
    this->add_coins(1);

    if(this->has_extra_turn()){
        this->clear_extra_turn();
        return;
    }
    
    game.next_turn();

    
}

void Governor::tax(){
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Governer's turn");
    
    if(this->is_blocked(Actions::Tax)){
            std::cout << "Tax is blocked" <<std::endl;
            this->clear_blocked();
    }      

    game.get_pool() -= 3;
    this->add_coins(3);

    if(this->has_extra_turn()){
        this->clear_extra_turn();
        return;
    }
    game.next_turn();

}

void Governor::bribe(){
    
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this)
        throw std::runtime_error("Not the Governer's turn");

    if(this->coins < 4) 
        throw std::runtime_error("Not enough Money to bribe");

    this->game.get_pool() += 4;
    this->reduce_coins(4);

        if(this->is_blocked(Actions::Bribe)){
            std::cout <<"Bribe has been Blocked"<<std::endl;
            game.next_turn();
            this->clear_blocked();
            return;
        }

        this->extra_turn = true;


}

void Governor::arrest(Player &player){
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Governer's turn");

    if(this->has_already_arrested(player)) {
        this->set_free_arrested(player);
        throw std::runtime_error(player.get_name() + " has already been arrested");
    }

    if(this == &player)
        throw std::runtime_error("You cannot arrest yourself");

    if (player.get_coins() < 1) 
        throw std::runtime_error(player.get_name() + " has no coins to steal");
    
    player.reduce_coins(1);
    this->add_coins(1);

    game.next_turn();



}   


void Governor::sanction(Player &player){
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Governer's turn");

    if(this->get_coins()  < 3) 
        throw std::runtime_error("Not enough Money to sanction");

    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");        

    this->reduce_coins(3);
    game.get_pool() += 3;
    
    player.block_action(Actions::Gather);
    player.block_action(Actions::Tax);
    player.set_has_sanctioned(true,this);

    game.next_turn();


}

void Governor::coup(Player &player){

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Governer's turn");

    if(!player.get_active()) 
        throw std::runtime_error(player.get_name()+" is not active anymore");
    
    if(this == &player) 
        throw std::runtime_error("You cannot coup yourself");

    
    if(this->get_coins() < 7) 
        throw std::runtime_error("Not enough Money to coup "+player.get_name());

    if(this->is_blocked(Actions::Coup)){
            std::cout << "Coup is blocked" <<std::endl;
            this->clear_blocked();
    }  
    

    player.eliminate();
    
    this->reduce_coins(7);
    game.get_pool() +=7;


    game.next_turn();


}

void Governor::undo(Player &player){
    if(game.current_player() != this) 
        throw std::runtime_error("Not the Governer's turn");


    player.clear_blocked_action(Actions::Tax);


    
}


 std::string Governor::get_role_name()const{
    return "Governer";
}

Game& Governor::get_game(){
    return game;
}
