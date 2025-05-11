#ifndef STRATEGY_HPP
#define STRATEGY_HPP
#include "../../Player/Player.hpp"
#include "../../Game/Game.hpp"
#include <random>



class AIaction {
    public:
        virtual void favorite_action(Player* player, Player& target) = 0;
        virtual ~AIaction() = default;
    };
    


class AIaggresive: public AIaction{
    
     void favorite_action(Player *player,Player &target) override{
        if(player == nullptr) return;

        
        int coins = player->get_coins();

        if(coins >= 10) player->coup(target);


        std::vector<Actions> random;
        
        if(coins >= 4 ) random.push_back(Actions::Bribe);
        if(coins >= 3) random.push_back(Actions::Sanction);
        random.push_back(Actions::Tax);
        random.push_back(Actions::Gather);

        srand(static_cast<unsigned int>(time(nullptr)));
        int position = (rand()) % random.size();

        Actions act = random[position];
        
        switch (act) {
            case Sanction:
                player->sanction(target);
                break;
            case Bribe:
                player->bribe();
                break;
            case Tax:
                player->tax();
                break;
            case Gather:
                player->gather();
                break;
            default:
                break;
        }
        
        

    }

    ~AIaggresive()override = default;
};


class AIRandom : public AIaction {
    public:
        void favorite_action(Player* player, Player& target) override {
            if (!player) return;
    
            std::vector<Actions> available;
    
            
            if (player->get_coins() >= 10) {
                player->coup(target);
                return;
            }
    

            available.push_back(Actions::Gather);
            available.push_back(Actions::Tax);
            available.push_back(Actions::Bribe);
            available.push_back(Actions::Sanction);
    
         
            srand(static_cast<unsigned int>(time(nullptr)));
            int index = rand() % available.size();
            Actions act = available[index];
    
            switch (act) {
                case Gather:
                    player->gather();
                    break;
                case Tax:
                    player->tax();
                    break;
                case Bribe:
                    player->bribe();
                    break;
                case Sanction:
                    player->sanction(target);
                    break;
                default:
                    break;
            }
        }
        ~AIRandom() override = default;
    };
    









#endif