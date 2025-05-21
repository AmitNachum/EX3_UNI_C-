/// Mail: nachum.amit@msmail.ariel.ac.il
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../coup/Game/Game.hpp"
#include "../coup/Player/Player.hpp"
#include "../roles/Governor/Governor.hpp"
#include "../roles/Baron/Baron.hpp"
#include "../roles/General/General.hpp"
#include "../roles/Judge/Judge.hpp"
#include "../roles/Merchant/Merchant.hpp"
#include "../roles/Spy/Spy.hpp"

TEST_CASE("Governor Test") {
    Game &game = Game::get_instance();
    game.reset();
    game.get_pool() = 50;
    Governor *gov = new Governor("Gov1", game, false);
    Governor *target = new Governor("Target", game, false);
    game.add_player(gov);
    game.add_player(target);
    game.set_index_turn(gov);

    SUBCASE("gather") {
        
        int c1 = gov->get_coins(); size_t p1 = game.get_pool();
        gov->gather();
        CHECK(gov->get_coins() == c1 + 1);
        CHECK(game.get_pool() == p1 - 1);

       
        game.get_pool() = 1;
        gov->gather();
        CHECK(game.get_pool() == 0);

        
        gov->block_action(Actions::Gather);
        CHECK_THROWS(gov->gather());

       
        gov->add_coins(10);
        CHECK_THROWS(gov->gather());

        
        game.set_index_turn(target);
        CHECK_THROWS(gov->gather());
    }

    SUBCASE("tax") {
        game.set_index_turn(gov);
        game.get_pool() = 50;
        gov->tax();
        CHECK(gov->get_coins() >= 3);

        gov->block_action(Actions::Tax);
        CHECK_THROWS(gov->tax());

        gov->add_coins(10);
        CHECK_THROWS(gov->tax());

        game.set_index_turn(target);
        CHECK_THROWS(gov->tax());

        game.set_index_turn(gov);
        game.get_pool() = 1;
        CHECK_THROWS_WITH( gov->tax(), "You must coup when holding 10 or more coins." );
    }

    SUBCASE("bribe") {
        game.set_index_turn(gov);
        gov->add_coins(4);
        CHECK_NOTHROW(gov->bribe());
        CHECK(gov->has_extra_turn());

        gov->reduce_coins(gov->get_coins());
        CHECK_THROWS(gov->bribe());

        gov->add_coins(10);
        CHECK_THROWS(gov->bribe());

        gov->reduce_coins(6);
        gov->block_action(Actions::Bribe);
        CHECK_THROWS(gov->bribe());

        game.set_index_turn(target);
        CHECK_THROWS(gov->bribe());
    }

    SUBCASE("arrest") {
        target->add_coins(2);
        int g1 = gov->get_coins(), t1 = target->get_coins();
        game.set_index_turn(gov);
        CHECK_NOTHROW(gov->arrest(*target));
        CHECK(gov->get_coins() == g1 + 1);
        CHECK(target->get_coins() == t1 - 1);

        CHECK_THROWS(gov->arrest(*gov));

        target->reduce_coins(target->get_coins());
        CHECK_THROWS(gov->arrest(*target));

        target->add_coins(2);
        gov->block_action(Actions::Arrest);
        CHECK_THROWS(gov->arrest(*target));

        gov->set_free_arrested(*target);
        CHECK_THROWS(gov->arrest(*target));
    }

    SUBCASE("block_tax") {
        Governor* other = new Governor("Other", game, false);
        game.add_player(other);
        gov->block_tax(*other);
        CHECK(other->is_blocked(Actions::Tax));

    }

    game.reset();
}  



TEST_CASE("Baron Test") {
    Game& game = Game::get_instance();
    game.reset();
    game.get_pool() = 50;
    Baron* baron = new Baron("Baron", game, false);
    Governor* target = new Governor("Target", game, false);
    game.add_player(baron);
    game.add_player(target);
    game.set_index_turn(baron);

    SUBCASE("gather") {
        int coins = baron->get_coins();
        size_t pool = game.get_pool();
        CHECK_NOTHROW(baron->gather());
        CHECK(baron->get_coins() == coins + 1);
        CHECK(game.get_pool() == pool - 1);

        baron->block_action(Actions::Gather);
        CHECK_THROWS(baron->gather());

        baron->add_coins(10);
        CHECK_THROWS(baron->gather());

        game.get_pool() = 0;
        CHECK_THROWS(baron->gather());

        game.set_index_turn(target);
        CHECK_THROWS(baron->gather());
    }

    SUBCASE("tax") {
        game.set_index_turn(baron);
        game.get_pool() = 50;
        int coins = baron->get_coins();
        CHECK_NOTHROW(baron->tax());
        CHECK(baron->get_coins() == coins + 2);

        baron->block_action(Actions::Tax);
        CHECK_THROWS(baron->tax());

        baron->add_coins(10);
        CHECK_THROWS(baron->tax());

        game.set_index_turn(target);
        CHECK_THROWS(baron->tax());

        game.set_index_turn(baron);
        game.get_pool() = 1;
        baron->reduce_coins(10);
        CHECK_NOTHROW(baron->tax());  // Should still allow action even with low pool
    }

    SUBCASE("bribe") {
        game.set_index_turn(baron);
        baron->add_coins(4);
        CHECK_NOTHROW(baron->bribe());
        CHECK(baron->has_extra_turn());

        baron->reduce_coins(baron->get_coins());
        CHECK_THROWS(baron->bribe());

        baron->add_coins(10);
        CHECK_THROWS(baron->bribe());

        baron->reduce_coins(6);
        baron->block_action(Actions::Bribe);
        CHECK_THROWS(baron->bribe());

        game.set_index_turn(target);
        CHECK_THROWS(baron->bribe());
    }

    SUBCASE("arrest") {
        target->add_coins(2);
        int b1 = baron->get_coins(), t1 = target->get_coins();
        game.set_index_turn(baron);
        CHECK_NOTHROW(baron->arrest(*target));
        CHECK(baron->get_coins() == b1 + 1);
        CHECK(target->get_coins() == t1 - 1);

        CHECK_THROWS(baron->arrest(*baron));

        target->reduce_coins(target->get_coins());
        CHECK_THROWS(baron->arrest(*target));

        target->add_coins(2);
        baron->block_action(Actions::Arrest);
        CHECK_THROWS(baron->arrest(*target));

        baron->set_free_arrested(*target);
        CHECK_THROWS(baron->arrest(*target));
    }

    SUBCASE("sanction") {
        game.set_index_turn(baron);
        target->set_active(true);
        baron->add_coins(3);
        CHECK_NOTHROW(baron->sanction(*target));
        CHECK(target->is_blocked(Actions::Tax));
        CHECK(target->is_blocked(Actions::Gather));

        CHECK_THROWS(baron->sanction(*baron));

        baron->reduce_coins(baron->get_coins());
        CHECK_THROWS(baron->sanction(*target));

        target->set_active(false);
        baron->add_coins(3);
        CHECK_THROWS(baron->sanction(*target));

        game.set_index_turn(target);
        CHECK_THROWS(baron->sanction(*target));
    }

    SUBCASE("coup") {
        game.set_index_turn(baron);
        target->set_active(true);
        baron->add_coins(7);
        CHECK_NOTHROW(baron->coup(*target));
        CHECK(!target->get_active());

        target->set_active(true);
        CHECK_THROWS(baron->coup(*baron));

        baron->reduce_coins(baron->get_coins());
        CHECK_THROWS(baron->coup(*target));

        target->set_active(false);
        baron->add_coins(7);
        CHECK_THROWS(baron->coup(*target));

        target->set_active(true);
        game.set_index_turn(target);
        CHECK_THROWS(baron->coup(*target));
    }

    SUBCASE("invest") {
        game.set_index_turn(baron);
        game.get_pool() = 5;
        int c1 = baron->get_coins();
        CHECK_NOTHROW(baron->invest());
        CHECK(baron->get_coins() == c1 + 3);

        game.get_pool() = 20;
        CHECK_NOTHROW(baron->invest());

        baron->add_coins(1);  
        CHECK_NOTHROW(baron->invest());

        game.set_index_turn(target);
        CHECK_THROWS(baron->invest());  

        game.set_index_turn(baron);
        game.get_pool() = 2;
        CHECK_THROWS(baron->invest());
    }

    game.reset();
}


TEST_CASE("General") {
    Game& game = Game::get_instance();
    game.reset();
    game.get_pool() = 50;
    General* gen = new General("Gen", game, false);
    Governor* target = new Governor("Gov", game, false);
    game.add_player(gen);
    game.add_player(target);
    game.set_index_turn(gen);

    SUBCASE("gather") {
        int c = gen->get_coins();
        size_t p = game.get_pool();
        CHECK_NOTHROW(gen->gather());
        CHECK(gen->get_coins() == c + 1);
        CHECK(game.get_pool() == p - 1);

        gen->block_action(Actions::Gather);
        CHECK_THROWS(gen->gather());

        gen->add_coins(10);
        CHECK_THROWS(gen->gather());

        game.get_pool() = 0;
        CHECK_THROWS(gen->gather());

        game.set_index_turn(target);
        CHECK_THROWS(gen->gather());
    }

    SUBCASE("tax") {
        game.set_index_turn(gen);
        game.get_pool() = 50;
        int coins = gen->get_coins();
        CHECK_NOTHROW(gen->tax());
        CHECK(gen->get_coins() == coins + 2);

        gen->block_action(Actions::Tax);
        CHECK_THROWS(gen->tax());

        gen->add_coins(10);
        CHECK_THROWS(gen->tax());

        game.set_index_turn(target);
        CHECK_THROWS(gen->tax());

        game.set_index_turn(gen);
        game.get_pool() = 1;
        gen->reduce_coins(10);
        CHECK_NOTHROW(gen->tax());
    }

    SUBCASE("bribe") {
        game.set_index_turn(gen);
        gen->add_coins(4);
        CHECK_NOTHROW(gen->bribe());
        CHECK(gen->has_extra_turn());

        gen->reduce_coins(gen->get_coins());
        CHECK_THROWS(gen->bribe());

        gen->add_coins(10);
        CHECK_THROWS(gen->bribe());

        gen->reduce_coins(6);
        gen->block_action(Actions::Bribe);
        CHECK_THROWS(gen->bribe());

        game.set_index_turn(target);
        CHECK_THROWS(gen->bribe());
    }

    SUBCASE("arrest") {
        target->add_coins(2);
        int c1 = gen->get_coins(), c2 = target->get_coins();
        game.set_index_turn(gen);
        CHECK_NOTHROW(gen->arrest(*target));
        CHECK(gen->get_coins() == c1 + 1);
        CHECK(target->get_coins() == c2 - 1);

        CHECK_THROWS(gen->arrest(*gen));

        target->reduce_coins(target->get_coins());
        CHECK_THROWS(gen->arrest(*target));

        target->add_coins(2);
        gen->block_action(Actions::Arrest);
        CHECK_THROWS(gen->arrest(*target));

    }

    SUBCASE("sanction") {
        game.set_index_turn(gen);
        target->set_active(true);
        gen->add_coins(3);
        CHECK_NOTHROW(gen->sanction(*target));
        CHECK(target->is_blocked(Actions::Tax));
        CHECK(target->is_blocked(Actions::Gather));

        CHECK_THROWS(gen->sanction(*gen));

        gen->reduce_coins(gen->get_coins());
        CHECK_THROWS(gen->sanction(*target));

        target->set_active(false);
        gen->add_coins(3);
        CHECK_THROWS(gen->sanction(*target));

        target->set_active(true);
        game.set_index_turn(target);
        CHECK_THROWS(gen->sanction(*target));
    }

    SUBCASE("coup") {
        game.set_index_turn(gen);
        gen->add_coins(7);
        target->set_active(true);
        CHECK_NOTHROW(gen->coup(*target));
        CHECK(!target->get_active());

        target->set_active(true);
        CHECK_THROWS(gen->coup(*gen));

        gen->reduce_coins(gen->get_coins());
        CHECK_THROWS(gen->coup(*target));

        target->set_active(false);
        gen->add_coins(7);
        CHECK_THROWS(gen->coup(*target));

        target->set_active(true);
        game.set_index_turn(target);
        CHECK_THROWS(gen->coup(*target));
    }

    SUBCASE("prevent_coup") {
        game.set_index_turn(gen);
        target->set_active(true);
        gen->add_coins(5);
        target->set_action_indicator(Actions::Coup, true, gen);
        CHECK_NOTHROW(gen->prevent_coup(*target));
        CHECK(target->get_active());

        gen->reduce_coins(gen->get_coins());
        target->set_action_indicator(Actions::Coup, true, gen);
        CHECK_THROWS(gen->prevent_coup(*target));

        gen->add_coins(5);
        target->set_action_indicator(Actions::Coup, false, nullptr);
        CHECK_NOTHROW(gen->prevent_coup(*target));  // does nothing

        target->set_action_indicator(Actions::Coup, true, nullptr);
        CHECK_NOTHROW(gen->prevent_coup(*target));  // still safe

        target->set_active(false);
        CHECK(target->get_active() == false);
    }

    game.reset();
}

TEST_CASE("Merchant") {
    Game& game = Game::get_instance();
    game.reset();
    game.get_pool() = 50;
    Merchant* m = new Merchant("Merchant", game, false);
    Spy* target = new Spy("Spy", game, false);
    game.add_player(m);
    game.add_player(target);
    game.set_index_turn(m);

    SUBCASE("gather") {
        int before = m->get_coins();
        size_t pool = game.get_pool();
        game.set_index_turn(m);
        CHECK_NOTHROW(m->gather());
        CHECK(m->get_coins() == before + 2); 
        CHECK(game.get_pool() == pool - 1);

        m->block_action(Actions::Gather);
        CHECK_THROWS(m->gather());

        m->add_coins(10);
        CHECK_THROWS(m->gather());

        game.get_pool() = 0;
        CHECK_THROWS(m->gather());

        game.set_index_turn(target);
        CHECK_THROWS(m->gather());
    }

    SUBCASE("tax") {
        game.set_index_turn(m);
        m->block_action(Actions::Tax);
        CHECK_THROWS(m->tax());

        m->clear_blocked();
        m->add_coins(10);
        CHECK_THROWS(m->tax());

        m->reduce_coins(10);
        game.get_pool() = 1;
        CHECK_NOTHROW(m->tax());

        game.set_index_turn(target);
        CHECK_THROWS(m->tax());

        game.set_index_turn(m);
        game.get_pool() = 10;
        CHECK_NOTHROW(m->tax());
    }

    SUBCASE("bribe") {
        game.set_index_turn(m);
        m->add_coins(4);
        CHECK_NOTHROW(m->bribe());

        m->reduce_coins(m->get_coins());
        CHECK_THROWS(m->bribe());

        m->add_coins(10);
        CHECK_THROWS(m->bribe());

        m->reduce_coins(6);
        m->block_action(Actions::Bribe);
        CHECK_THROWS(m->bribe());

        game.set_index_turn(target);
        CHECK_THROWS(m->bribe());
    }

    SUBCASE("arrest") {
        target->add_coins(3);
        int m_before = m->get_coins();
        game.set_index_turn(m);
        CHECK_NOTHROW(m->arrest(*target));
        CHECK(m->get_coins() > m_before);
        CHECK(target->get_coins() == 7); 

        target->reduce_coins(target->get_coins());
        CHECK_THROWS(m->arrest(*target));

        target->add_coins(2);
        m->block_action(Actions::Arrest);
        CHECK_THROWS(m->arrest(*target));

        m->set_free_arrested(*target);
        CHECK_THROWS(m->arrest(*target));

        CHECK_THROWS(m->arrest(*m));
    }

    SUBCASE("sanction") {
        game.set_index_turn(m);
        target->set_active(true);
        m->add_coins(3);
        CHECK_NOTHROW(m->sanction(*target));
        CHECK(target->is_blocked(Actions::Gather));
        CHECK(target->is_blocked(Actions::Tax));

        CHECK_THROWS(m->sanction(*m));

        m->reduce_coins(m->get_coins());
        CHECK_THROWS(m->sanction(*target));

        target->set_active(false);
        m->add_coins(3);
        CHECK_THROWS(m->sanction(*target));

        game.set_index_turn(target);
        CHECK_THROWS(m->sanction(*target));
    }

    SUBCASE("coup") {
        game.set_index_turn(m);
        target->set_active(true);
        m->add_coins(7);
        CHECK_NOTHROW(m->coup(*target));
        CHECK(!target->get_active());

        target->set_active(true);
        CHECK_THROWS(m->coup(*m));

        m->reduce_coins(m->get_coins());
        CHECK_THROWS(m->coup(*target));

        m->add_coins(7);
        target->set_active(false);
        CHECK_THROWS(m->coup(*target));

        target->set_active(true);
        game.set_index_turn(target);
        CHECK_THROWS(m->coup(*target));
    }

    game.reset();
}

TEST_CASE("Spy Functionality") {
    Game& game = Game::get_instance();
    game.reset();
    game.get_pool() = 50;
    Spy* spy = new Spy("Spy", game, false);
    Governor* g = new Governor("Gov", game, false);
    game.add_player(spy);
    game.add_player(g);
    game.set_index_turn(spy);

    SUBCASE("gather") {
        int before = spy->get_coins();
        size_t pool = game.get_pool();
        CHECK_NOTHROW(spy->gather());
        CHECK(spy->get_coins() == before + 1);
        CHECK(game.get_pool() == pool - 1);

        spy->block_action(Actions::Gather);
        CHECK_THROWS(spy->gather());

        spy->add_coins(10);
        CHECK_THROWS(spy->gather());

        game.get_pool() = 0;
        CHECK_THROWS(spy->gather());

        game.set_index_turn(g);
        CHECK_THROWS(spy->gather());
    }

    SUBCASE("tax") {
        game.set_index_turn(spy);
        spy->block_action(Actions::Tax);
        CHECK_THROWS(spy->tax());

        spy->clear_blocked();
        spy->add_coins(10);
        CHECK_THROWS(spy->tax());

        spy->reduce_coins(10);
        game.get_pool() = 2;
        CHECK_NOTHROW(spy->tax());

        game.set_index_turn(g);
        CHECK_THROWS(spy->tax());

        game.set_index_turn(spy);
        game.get_pool() = 3;
        CHECK_NOTHROW(spy->tax());
    }

    SUBCASE("bribe") {
        game.set_index_turn(spy);
        spy->add_coins(4);
        CHECK_NOTHROW(spy->bribe());

        spy->reduce_coins(spy->get_coins());
        CHECK_THROWS(spy->bribe());

        spy->add_coins(10);
        CHECK_THROWS(spy->bribe());

        spy->reduce_coins(6);
        spy->block_action(Actions::Bribe);
        CHECK_THROWS(spy->bribe());

        game.set_index_turn(g);
        CHECK_THROWS(spy->bribe());
    }

    SUBCASE("arrest") {
        g->add_coins(3);
        int s_before = spy->get_coins();
        int g_before = g->get_coins();
        game.set_index_turn(spy);
        CHECK_NOTHROW(spy->arrest(*g));
        CHECK(spy->get_coins() == s_before + 1);
        CHECK(g->get_coins() == g_before - 1);

        g->reduce_coins(g->get_coins());
        CHECK_THROWS(spy->arrest(*g));

        g->add_coins(2);
        spy->block_action(Actions::Arrest);
        CHECK_THROWS(spy->arrest(*g));

        spy->set_free_arrested(*g);
        CHECK_THROWS(spy->arrest(*g));

        CHECK_THROWS(spy->arrest(*spy));
    }

    SUBCASE("sanction and support abilities") {
        game.set_index_turn(spy);
        g->set_active(true);
        spy->add_coins(3);
        CHECK_NOTHROW(spy->sanction(*g));
        CHECK(g->is_blocked(Actions::Gather));
        CHECK(g->is_blocked(Actions::Tax));

        std::string s = spy->view_pile(*g);
        CHECK(s.find("Gov") != std::string::npos);

        spy->block_arrest(*g);
        CHECK(g->is_blocked(Actions::Arrest));
    }

    game.reset();
}

