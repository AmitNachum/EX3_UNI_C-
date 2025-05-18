// Mail: nachum.amit@msmail.ariel.ac.il
#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "../../Player/Player.hpp"
#include "../../../roles/Baron/Baron.hpp"
#include "../../../roles/Governor/Governor.hpp"
#include "../../../roles/Judge/Judge.hpp"
#include "../../../roles/Merchant/Merchant.hpp"
#include "../../../roles/Spy/Spy.hpp"
#include "../../../roles/General/General.hpp"
#include "../../Game/Game.hpp"

enum Roles {
    ROLE_BARON,
    ROLE_GOVERNOR,
    ROLE_JUDGE,
    ROLE_MERCHANT,
    ROLE_SPY,
    ROLE_GENERAL
};

class FactoryPlayers {
private:
    FactoryPlayers() = delete; // prevent instantiation
    FactoryPlayers(const FactoryPlayers&) = delete;
    FactoryPlayers& operator=(const FactoryPlayers&) = delete;

public:
    static Player* createPlayer(Roles role, const std::string& name, Game& game,bool is_ai) {
        switch (role) {
            case ROLE_BARON:
                return new Baron(name, game,is_ai);
            case ROLE_GOVERNOR:
                return new Governor(name, game,is_ai);
            case ROLE_JUDGE:
                return new Judge(name, game,is_ai);
            case ROLE_MERCHANT:
                return new Merchant(name, game,is_ai);
            case ROLE_SPY:
                return new Spy(name, game,is_ai);
            case ROLE_GENERAL:
                return new General(name, game,is_ai);
            default:
                throw std::invalid_argument("Unknown role.");
        }
    }
};

#endif
