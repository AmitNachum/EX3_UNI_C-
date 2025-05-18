#include "Merchant.hpp"
// Mail: nachum.amit@msmail.ariel.ac.il
void Merchant::handle_discount(){
    if(this->get_coins() >= 3){
        this->add_coins(1);
    }
}


void Merchant::handle_arrest(){
    std::vector<std::pair<bool,Player*>>& actionID= this->get_action_indicator();
    if(!(actionID[Actions::Arrest].first) || actionID[Actions::Arrest].second == nullptr) return;
    
    actionID[Actions::Arrest].second->reduce_coins(1);
    this->add_coins(1);
    this->reduce_coins(2);
    this->game.get_pool() += 2;

    actionID[Actions::Arrest] = {false,nullptr};

}

void Merchant::handle_passive_effects() {
    handle_discount();
    handle_arrest();
}

void Merchant::gather(){
    
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){
        std::cout<<"Not the Merchant's turn\n";
        return;
    }
    if(this->is_blocked(Actions::Gather)){
            std::cout << "Gather is blocked to " + this->get_name() <<std::endl;
            this->clear_blocked();
            return;
    }   

    handle_passive_effects();

    game.get_pool()--;
    this->add_coins(1);

 
    
}

void Merchant::tax(){
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){
        std::cout<<"Not the Merchant's turn\n";
        return;
    }
    if(this->is_blocked(Actions::Tax)){
            throw std::runtime_error("Tax is blocked to "+ this->get_name());
            this->clear_blocked();
    }   

    handle_passive_effects();





    game.get_pool() -= 2;
    this->add_coins(2);

}


void Merchant::bribe(){
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){
        throw std::runtime_error("Not the Merchant's turn");
    }
    if(this->get_coins()  < 4) 
        throw std::runtime_error("Not enough Money to bribe");

    if(this->is_blocked(Actions::Bribe)){
            std::cout <<"Bribe has been Blocked to " + this->get_name()<<std::endl;
            this->clear_blocked();
            return;
        }
        
    handle_passive_effects();

    this->game.get_pool() += 4;
    this->reduce_coins(4);



    this->extra_turn = true;


  

}



void Merchant::arrest(Player &player){


    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){
       throw std::runtime_error("Not the Merchant's turn");
    }

    if(this->last_arrested_player == &player) {
        this->set_free_arrested(player);
        throw std::runtime_error(player.get_name() + " has already been arrested");
    }

    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself"); 

    if (player.get_coins() < 1) 
        throw std::runtime_error(player.get_name() + " has no coins to steal");

    if(!player.get_active()){
        throw std::runtime_error(player.get_name() + " Has been Couped");
    }    

    handle_passive_effects();  
   

player.reduce_coins(1);
this->add_coins(1);
last_arrested_player = &player;



}


void Merchant::sanction(Player &player){

    if(game.current_player() != this){
        throw std::runtime_error("Not the Merchant's turn");
        return;
    }
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");
    
    if(!player.get_active()){
        throw std::runtime_error(player.get_name() + " Has been Couped");
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



void Merchant::coup(Player& player){
    
     if(game.current_player() != this){
        std::cout<<"Not the Merchant's turn\n";
        return;
    }

    if(!player.get_active()) 
        throw std::runtime_error(player.get_name()+" Has been Couped");

    if(this == &player) 
        throw std::runtime_error("You cannot coup yourself");


    if(this->get_coins() < 7) 
        throw std::runtime_error("Not enough Money to coup "+player.get_name());

    handle_passive_effects();



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


void Merchant::undo(Player &player){
    
    if(game.current_player() != this){
        std::cout<<"Not the Merchant's turn\n";
        return;
    }


    handle_passive_effects();
    player.clear_blocked_action(Actions::Tax);


}


std::string Merchant::get_role_name() const{
    return "Merchant";
}

