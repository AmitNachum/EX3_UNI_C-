
// Mail: nachum.amit@msmail.ariel.ac.il
#include "Spy.hpp"
string Spy::view_pile(Player &player){
    int other_coin_amount = player.get_coins();
    return player.get_name() + " has: " + std::to_string(other_coin_amount) + " coins";

}

void Spy::block_arrest(Player &player){
    if (!player.is_blocked(Actions::Arrest)) {
        player.block_action(Actions::Arrest);
        std::cout << player.get_name() << "'s arrest has been blocked" << std::endl;
    }
    
}


void Spy::gather(){
   if(game.current_player() != this) {
        throw std::runtime_error("Not the Spy's turn\n");
    }
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");


    if(this->is_blocked(Actions::Gather)){
        this->clear_blocked();
        throw std::runtime_error("Gather is blocked to " + this->get_name());
        
    }   


    game.get_pool()--;
    this->add_coins(1);


}



void Spy::tax(){
    if(game.current_player() != this) {
      throw std::runtime_error("Not the Spy's turn");
    }
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(this->is_blocked(Actions::Tax)){
            this->clear_blocked();
            throw std::runtime_error("Tax is blocked to "+ this->get_name());
          
    }   




    game.get_pool() -= 2;
    this->add_coins(2);
}



void Spy::bribe(){

    if(game.current_player() != this) {
      throw std::runtime_error("Not the Spy's turn");
        return;
    }
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(this->get_coins()  < 4) 
        throw std::runtime_error("Not enough Money to bribe");


    this->game.get_pool() += 4;
    this->reduce_coins(4);


        if(this->is_blocked(Actions::Bribe)){
            this->clear_blocked();
            throw std::runtime_error("Bribe has been Blocked to " + this->get_name());
        }




        this->extra_turn = true;
        game.notify_Judge_Bribe(*this);


}



void Spy::arrest(Player &player){

    if(game.current_player() != this) {
        std::cout << "Not the Spy's turn\n";
        return;
    }
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(this->last_arrested_player == &player) {
        this->set_free_arrested(player);
        throw std::runtime_error(player.get_name() + " has already been arrested");
    }

    if(this == &player)
        throw std::runtime_error("You cannot Arrest yourself");
    
    if(!player.get_active())
        throw std::runtime_error(player.get_name() + " Has been Couped");      

    if (player.get_coins() < 1) 
        throw std::runtime_error(player.get_name() + " has no coins to steal");

    if(this->is_blocked(Actions::Arrest)){
            this->clear_blocked();
            throw std::runtime_error("Arrest has been Blocked to " + this->get_name());
    }    

   

player.reduce_coins(1);
this->add_coins(1);
last_arrested_player = &player;


}


void Spy::sanction(Player &player){
    if(game.current_player() != this) {
        std::cout << "Not the Spy's turn\n";
        return;
    }
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(this->get_coins()  < 3) 
        throw std::runtime_error("Not enough Money to sanction");

    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");
    
    if(!player.get_active())
        throw std::runtime_error(player.get_name() + " Has been Couped");    
    





this->reduce_coins(3);
game.get_pool() += 3;

player.block_action(Actions::Gather);
player.block_action(Actions::Tax);
player.set_action_indicator(Actions::Sanction,true,this);


}



void Spy::coup(Player& player){
    
    if(game.current_player() != this) {
       throw std::runtime_error("Not the Spy's turn");
    }
    
    if(!player.get_active()) 
        throw std::runtime_error(player.get_name()+" Has been Couped");

    if(this == &player) 
        throw std::runtime_error("You cannot coup yourself");


    if(this->get_coins() < 7) 
        throw std::runtime_error("Not enough Money to coup "+player.get_name());


    this->reduce_coins(7);
    game.get_pool() += 7; 
    player.set_action_indicator(Actions::Coup, true, this);
    this->game.notify_general_coup(player,*this);
        
       
    
    if (player.get_action_indicator()[Actions::Coup].first) {    // 3. No general prevented it
        player.eliminate();                                      //    Now we eliminate the target
    } else {
        std::cout << "🛡️ Coup against " << player.get_name() << " was prevented by a General." << std::endl;
    }

        
}


void Spy::undo(Player &player){
    
    if(game.current_player() != this) {
        std::cout << "Not the Spy's turn\n";
        return;
    }
    player.clear_blocked_action(Actions::Tax);


}


std::string Spy::get_role_name() const{
    return "Spy";
}

