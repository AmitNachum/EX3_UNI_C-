// Mail: nachum.amit@msmail.ariel.ac.il

#include "Judge.hpp"


void Judge::gather(){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){ 
       std::cout << "Not the Judge's turn\n";
    }
    if(this->is_blocked(Actions::Gather)){
            std::cout << "Gather is blocked to " + this->get_name()<<std::endl;
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


void Judge::tax(){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

     if(game.current_player() != this){ 
       std::cout << "Not the Judge's turn\n";
    }
    if(this->is_blocked(Actions::Tax)){
            std::cout << "Tax is blocked " + this->get_name()<<std::endl;
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


void Judge::bribe(){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

     if(game.current_player() != this){ 
       std::cout << "Not the Judge's turn\n";
    }

    if(this->get_coins()  < 4) 
        throw std::runtime_error("Not enough Money to bribe");


    this->game.get_pool() += 4;
    this->reduce_coins(4);


        if(this->is_blocked(Actions::Bribe)){
            std::cout <<"Bribe has been Blocked " + this->get_name()<<std::endl;
            this->clear_blocked();
            game.next_turn();
            return;
        }




            if(this->has_extra_turn()){
            this->clear_extra_turn();
            return;
        }



        this->extra_turn = true;

        game.next_turn();

}

void Judge::arrest(Player &player){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){ 
       std::cout << "Not the Judge's turn\n";
    }

    if(this->has_already_arrested(player)) {
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
            game.next_turn();
            return;
    }    

   

player.reduce_coins(1);
this->add_coins(1);

if(this->has_extra_turn()){
    this->clear_extra_turn();
    return;
}

game.next_turn();

}


void Judge::sanction(Player &player){
    handle_sanctions();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(game.current_player() != this){ 
       std::cout << "Not the Judge's turn\n";
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

if(this->has_extra_turn()){
    this->clear_extra_turn();
    return;
}

game.next_turn();
}

void Judge::coup(Player& player){
    handle_sanctions();

    if(game.current_player() != this){ 
       std::cout << "Not the Judge's turn\n";
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


void Judge::undo(Player &player){};


std::string Judge::get_role_name() const{
    return "Judge";
}

Game& Judge::get_game(){
    return game;
}






