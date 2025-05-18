// Mail: nachum.amit@msmail.ariel.ac.il

#include "Judge.hpp"


void Judge::gather(){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){ 
      throw std::runtime_error("Not the Judge's turn");
    }
    if(this->is_blocked(Actions::Gather)){
            std::cout << "Gather is blocked to " + this->get_name()<<std::endl;
            this->clear_blocked();
            return;
    }   


    game.get_pool()--;
    this->add_coins(1);





}


void Judge::tax(){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

     if(game.current_player() != this){ 
      throw std::runtime_error("Not the Judge's turn");
    }
    if(this->is_blocked(Actions::Tax)){
            throw std::runtime_error("Tax is blocked to "+ this->get_name());
            this->clear_blocked();
    }   

    game.get_pool() -= 2;
    this->add_coins(2);

}


void Judge::bribe(){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

     if(game.current_player() != this){ 
       throw std::runtime_error("Not the Judge's turn");
    }

    if(this->get_coins()  < 4) 
        throw std::runtime_error("Not enough Money to bribe");


    this->game.get_pool() += 4;
    this->reduce_coins(4);


        if(this->is_blocked(Actions::Bribe)){
            std::cout <<"Bribe has been Blocked " + this->get_name()<<std::endl;
            this->clear_blocked();
            return;
        }


this->extra_turn = true;



}

void Judge::arrest(Player &player){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){ 
       throw std::runtime_error("Not the Judge's turn");
    }

    if(!player.get_active()){
        throw std::runtime_error(player.get_name() + " Has been Couped");
    }

    if(this->last_arrested_player == &player) {
        this->set_free_arrested(player);
        throw std::runtime_error(player.get_name() + " has already been arrested");
    }

    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");

    if (player.get_coins() < 1) 
        throw std::runtime_error(player.get_name() + " has no coins to steal");

    if(this->is_blocked(Actions::Arrest)){
            std::cout <<"Arrest has been Blocked to "+ this->get_name() <<std::endl;
            this->clear_blocked();
            return;
    }    

   

player.reduce_coins(1);
this->add_coins(1);
last_arrested_player = &player;


}


void Judge::sanction(Player &player){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){ 
      throw std::runtime_error("Not the Judge's turn");
    }
    if(player.get_active()){
        throw std::runtime_error(player.get_name() + " Has been couped");
    }  

    if(this->get_coins()  < 3) 
        throw std::runtime_error("Not enough Money to sanction");


    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");

      




this->reduce_coins(3);
game.get_pool() += 3;

player.block_action(Actions::Gather);
player.block_action(Actions::Tax);
player.set_action_indicator(Actions::Sanction,true,this);



}

void Judge::coup(Player& player){
    handle_sanctions();

    if(game.current_player() != this){ 
       throw std::runtime_error("Not the Judge's turn");
    }
    if(!player.get_active()) 
        throw std::runtime_error(player.get_name()+" is not active anymore");

    if(this == &player) 
        throw std::runtime_error("You cannot coup yourself");


    if(this->get_coins() < 7) 
        throw std::runtime_error("Not enough Money to coup "+player.get_name());




    this->reduce_coins(7);
    game.get_pool() += 7; 
    player.set_action_indicator(Actions::Coup, true, this);
    this->game.notify_general_coup(player,*this);
        
       
    
    if (player.get_action_indicator()[Actions::Coup].first) {    
        player.eliminate();                                      
    } else {
        std::cout << "🛡️ Coup against " << player.get_name() << " was prevented by a General." << std::endl;
    }


}


void Judge::undo(Player &player){};


std::string Judge::get_role_name() const{
    return "Judge";
}

Game& Judge::get_game(){
    return game;
}






