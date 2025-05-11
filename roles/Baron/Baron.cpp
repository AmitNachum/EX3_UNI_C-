// Mail: nachum.amit@msmail.ariel.ac.il

#include "Baron.hpp"


void Baron::gather(){
    this->handle_sanction_bonus();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Baron's turn");

    if(this->is_blocked(Actions::Gather)){
            std::cout << "Gather is blocked" <<std::endl;
            this->clear_blocked();
            game.next_turn();
            return;
    }   
    

        game.get_pool()--;
        this->add_coins(1);
    
        if(this->has_extra_turn()){
            this->clear_extra_turn();
            return;
        }
        
        game.next_turn();
    
}


void Baron::tax(){

    this->handle_sanction_bonus();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Baron's turn");

    if(this->is_blocked(Actions::Tax)){
            std::cout << "Tax is blocked" <<std::endl;
            this->clear_blocked();
            game.next_turn();
            return;
    }   




    game.get_pool() -= 2;
    this->add_coins(2);

    if(this->has_extra_turn()){
        this->clear_extra_turn();
        return;
    }
    game.next_turn();



}


void Baron::bribe(){

    this->handle_sanction_bonus();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Baron's turn");

    if(this->get_coins()  < 4) 
        throw std::runtime_error("Not enough Money to bribe");


    this->game.get_pool() += 4;
    this->reduce_coins(4);


        if(this->is_blocked(Actions::Bribe)){
            std::cout <<"Bribe has been Blocked"<<std::endl;
            this->clear_blocked();
            game.next_turn();
            return;
        }




        this->extra_turn = true;


}


void Baron::arrest(Player &player){
    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Baron's turn");

    if(this->has_already_arrested(player)) {
        this->set_free_arrested(player);
        throw std::runtime_error(player.get_name() + " has already been arrested");
    }
    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");

    if (player.get_coins() < 1) 
        throw std::runtime_error(player.get_name() + " has no coins to steal");
    
    if(this->is_blocked(Actions::Arrest)){
            std::cout <<"Arrest has been Blocked"<<std::endl;
            game.next_turn();
            this->clear_blocked();
            return;
    }    

    this->handle_sanction_bonus();

    player.reduce_coins(1);
    this->add_coins(1);

    if(this->has_extra_turn()){
        this->clear_extra_turn();
        return;
    }

    game.next_turn();

}


void Baron::sanction(Player &player){

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Baron's turn");

    if(this->get_coins()  < 3) 
        throw std::runtime_error("Not enough Money to sanction");

    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");
        

    this->handle_sanction_bonus();



    this->reduce_coins(3);
    game.get_pool() += 3;

    player.block_action(Actions::Gather);
    player.block_action(Actions::Tax);
    player.set_action_indicator(Actions::Sanction,true,this);
    

    if(this->has_extra_turn()){
        this->clear_extra_turn();
        return;
    }

    game.next_turn();


}

void Baron::coup(Player &player){

    
    if(game.current_player() != this) 
        throw std::runtime_error("Not the Barons's turn");

    if(!player.get_active()) 
        throw std::runtime_error(player.get_name()+" is not active anymore");
    
    if(this == &player) 
        throw std::runtime_error("You cannot coup yourself");

    
    if(this->get_coins() < 7) 
        throw std::runtime_error("Not enough Money to coup "+player.get_name());
    


    this->handle_sanction_bonus();

    this->reduce_coins(7);
    game.get_pool() += 7; 
    player.set_action_indicator(Actions::Coup, true, this);
    this->game.notify_general_coup(player,*this);
    
   

    if (player.get_action_indicator()[Actions::Coup].first) {    // 3. No general prevented it
        player.eliminate();                                      //    Now we eliminate the target

    } else {
        std::cout << "🛡️ Coup against " << player.get_name() << " was prevented by a General." << std::endl;
    }
    


    if(this->has_extra_turn()){
        this->clear_extra_turn();
        return;
    }

    game.next_turn();
}


void Baron::undo(Player &player){
    if(game.current_player() != this) 
        throw std::runtime_error("Not the Baron's turn");

    this->handle_sanction_bonus();
        

    player.clear_blocked_action(Actions::Tax);

}


 std::string Baron::get_role_name() const{
    return "Baron";
}

void Baron::invest(){

    this->handle_sanction_bonus();

    if(game.current_player() != this) 
        throw std::runtime_error("Not the Baron's turn");

    if (game.get_pool() < 3)
        throw std::runtime_error("Not enough coins in the pool to invest");


    this->add_coins(3);
    game.get_pool() -= 3;

    if(this->has_extra_turn()){
        this->clear_extra_turn();
        return;
    }

    game.next_turn();

}

void Baron::handle_sanction_bonus() {
    if (this->get_action_indicator()[Actions::Sanction].first) {
        this->add_coins(1);
        this->get_action_indicator()[Actions::Sanction].first = false;
        this->get_action_indicator()[Actions::Sanction].second = nullptr;
    }
}