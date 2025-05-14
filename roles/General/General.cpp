#include "General.hpp"
// Mail: nachum.amit@msmail.ariel.ac.il
void General::handle_arrests(){
    if(!(actions_indicator[Actions::Arrest].first) || actions_indicator[Actions::Arrest].second == nullptr) return;

    actions_indicator[Actions::Arrest].second->reduce_coins(1);
    this->add_coins(1);

    actions_indicator[Actions::Arrest] = {false,nullptr};

}

void General::revive_player(Player &player){
    player.set_active(true);
}

void General::prevent_coup(Player &player){
    if(this->get_coins() < 5){ 
       throw std::runtime_error("General cannot prevent the coup — not enough coins.");
    }
    
    if(!(player.get_action_indicator()[Actions::Coup].first) || 
    player.get_action_indicator()[Actions::Coup].second == nullptr) return;

    
    this->reduce_coins(5);
    revive_player(player);

    std::cout << "General prevented the coup against " << player.get_name() << "!" << std::endl;

    player.set_action_indicator(Actions::Coup,false,nullptr);



}





void General::gather(){

    if(game.current_player() != this){
        throw std::runtime_error("Not the General's turn");
    }

    handle_arrests();    

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");


    if(this->is_blocked(Actions::Gather)){
            std::cout << "Gather is blocked to "+ this->get_name() <<std::endl;
            this->clear_blocked();
            return;
    }   


    game.get_pool()--;
    this->add_coins(1);




}



void General::tax(){

     if(game.current_player() != this){
        throw std::runtime_error("Not the General's turn");
    }

    handle_arrests();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(this->is_blocked(Actions::Tax)){
            std::cout << "Tax is blocked to "+ this->get_name() <<std::endl;
            this->clear_blocked();
            return;
    }   

    game.get_pool() -= 2;
    this->add_coins(2);



}



void General::bribe(){

    if(game.current_player() != this){
        throw std::runtime_error("Not the General's turn");
        return;
    }

    handle_arrests();    

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(this->get_coins()  < 4) 
        throw std::runtime_error("Not enough Money to bribe");

    
    this->game.get_pool() += 4;
    this->reduce_coins(4);


        if(this->is_blocked(Actions::Bribe)){
            std::cout <<"Bribe has been Blocked to " + this->get_name()<<std::endl;
            this->clear_blocked();
            return;
        }
        

        this->extra_turn = true;


}




void General::arrest(Player &player){

     if(game.current_player() != this){
        throw std::runtime_error("Not the General's turn\n");
    }

    handle_arrests();

    if(this->get_coins() >= 10){ 
        throw std::runtime_error("You must coup when holding 10 or more coins.");
    }

    if(this->has_already_arrested(player)) {
        this->set_free_arrested(player);
        throw std::runtime_error(player.get_name() + " has already been arrested");
    }

    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");
    if(!player.get_active()){
        throw std::runtime_error(player.get_name() + " Has been Couped");
    }    

    if (player.get_coins() < 1) 
        throw std::runtime_error(player.get_name() + " has no coins to steal");

    if(this->is_blocked(Actions::Arrest)){
            std::cout <<"Arrest has been Blocked to " + this->get_name()<<std::endl;
            this->clear_blocked();
            return;
    }    

   

player.reduce_coins(1);
this->add_coins(1);




}





void General::sanction(Player &player){
     if(game.current_player() != this){
        std::cout <<"Not the General's turn\n";
        return;
    }
    handle_arrests();

    if(this->get_coins() >= 10) 
        throw std::runtime_error("You must coup when holding 10 or more coins.");

    if(this->get_coins()  < 3) 
        throw std::runtime_error("Not enough Money to sanction");

    if(this == &player)
        throw std::runtime_error("You cannot sanction yourself");

    if(!player.get_active()){
        throw std::runtime_error(player.get_name() + " Has been Couped");
    }    
    





this->reduce_coins(3);
game.get_pool() += 3;

player.block_action(Actions::Gather);
player.block_action(Actions::Tax);
player.set_action_indicator(Actions::Sanction,true,this);





}




void General::coup(Player& player){
    
    if(game.current_player() != this){
        throw std::runtime_error("Not the General's turn");
    }

    handle_arrests();   

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





void General::undo(Player &player){
    
     if(game.current_player() != this){
        std::cout <<"Not the General's turn\n";
        return;
    }
    player.clear_blocked_action(Actions::Tax);


}

std::string General::get_role_name() const{
    return "General";
}


