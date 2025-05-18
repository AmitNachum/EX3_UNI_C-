// Mail: nachum.amit@msmail.ariel.ac.il

#include "Baron.hpp"


void Baron::gather(){
    this->handle_sanction_bonus();

    if(this->get_coins() >= 10) {
       throw std::runtime_error("You must coup when holding 10 or more coins.");
    }

      if(game.current_player() != this){
       throw std::runtime_error("Not the Barons's turn");
    }

    if(this->is_blocked(Actions::Gather)){
            std::cout << "Gather is blocked to " + this->get_name() <<std::endl;
            this->clear_blocked();
            return;
    }   
    

        game.get_pool()--;
        this->add_coins(1);
    
          
}


void Baron::tax(){

    this->handle_sanction_bonus();

    if(this->get_coins() >= 10) {
        throw std::runtime_error("You must coup when holding 10 or more coins.");
    }
      if(game.current_player() != this){
       throw  std::runtime_error("Not the Barons's turn");
    }

    if(this->is_blocked(Actions::Tax)){
            throw std::runtime_error("Tax is blocked to "+ this->get_name());
            this->clear_blocked();
    }   




    game.get_pool() -= 2;
    this->add_coins(2);




}


void Baron::bribe(){

    this->handle_sanction_bonus();

    if(this->get_coins() >= 10) {
       throw std::runtime_error("You must coup when holding 10 or more coins.");
    }
     if(game.current_player() != this){
       throw std::runtime_error("Not the Barons's turn");
    }

    if(this->get_coins() < 4){ 
       throw  std::runtime_error("Not enough Money to bribe");
    }

    this->game.get_pool() += 4;
    this->reduce_coins(4);


        if(this->is_blocked(Actions::Bribe)){
            std::cout <<"Bribe has been Blocked "+ this->get_name()<<std::endl;
            return;
        }




        this->extra_turn = true;
  

}


void Baron::arrest(Player &player){

    if(game.current_player() != this){
       throw  std::runtime_error("Not the Barons's turn");
    }

    if(!player.get_active()){
       throw  std::runtime_error(player.get_name() + " has been Couped");
    }

    if(this->last_arrested_player == &player) {
        this->set_free_arrested(player);
        throw std::runtime_error(player.get_name() + " has already been arrested");
    }

    
    if(this == &player){
        throw std::runtime_error("You cannot Arrest yourself");
    }

    if(this->get_coins() >= 10) {
        throw std::runtime_error("You must coup when holding 10 or more coins.");
    }

    if (player.get_coins() < 1) {
        throw std::runtime_error(player.get_name() + " has no coins to steal");
    }
    
    if(this->is_blocked(Actions::Arrest)){
            std::cout <<"Arrest has been Blocked "+ this->get_name()<<std::endl;
            this->clear_blocked();
            return;
    }    

    this->handle_sanction_bonus();

    player.reduce_coins(1);
    this->add_coins(1);
    last_arrested_player = &player;


}


void Baron::sanction(Player &player){

    if(this->get_coins() >= 10) {
       throw std::runtime_error("You must coup when holding 10 or more coins.");
    }

    if(game.current_player() != this){
      throw  std::runtime_error("Not the Barons's turn");
    }

    if(this->get_coins()  < 3) {
      throw  std::runtime_error("Not enough Money to sanction");
    }

    if(this == &player){
       throw  std::runtime_error("You cannot sanction yourself");
    }

    if(!player.get_active()){
        throw std::runtime_error(player.get_name() + " Has been Couped");
    }

    this->handle_sanction_bonus();



    this->reduce_coins(3);
    game.get_pool() += 3;

    player.block_action(Actions::Gather);
    player.block_action(Actions::Tax);
    player.set_action_indicator(Actions::Sanction,true,this);
    




}

void Baron::coup(Player &player){

    
    if(game.current_player() != this) {
      throw std::runtime_error("Not the Barons's turn");
    }

    if(!player.get_active()) {
      throw std::runtime_error(player.get_name()+" Has been Couped");
    }
    if(this == &player){ 
       throw std::runtime_error("You cannot coup yourself");
    }
    
    if(this->get_coins() < 7){ 
       throw std::runtime_error("Not enough Money to coup "+player.get_name());
  
    }


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
    



}


void Baron::undo(Player &player){
    if(game.current_player() != this) {
        std::cout <<"Not the Baron's turn\n";
        return;
    }
    this->handle_sanction_bonus();
        

    player.clear_blocked_action(Actions::Tax);

}


 std::string Baron::get_role_name() const{
    return "Baron";
}

void Baron::invest(){

    this->handle_sanction_bonus();

    if(game.current_player() != this) 
        std::cout <<"Not the Baron's turn\n";

    if (game.get_pool() < 3)
        throw std::runtime_error("Not enough coins in the pool to invest");


    this->add_coins(3);
    game.get_pool() -= 3;



}

void Baron::handle_sanction_bonus() {
    
    if (this->get_action_indicator()[Actions::Sanction].first) {
        this->add_coins(1);
        this->get_action_indicator()[Actions::Sanction].first = false;
        this->get_action_indicator()[Actions::Sanction].second = nullptr;
    }
}