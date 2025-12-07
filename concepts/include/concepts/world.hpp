#pragma once
#include "coffeemachine.hpp"
#include "mug.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/unordered_map.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include <set>
namespace CoffeeShop
{
using Owner = Actor;

struct World
{
    bool transferOwnership(Owner* newOwner, Actor* target);
    std::vector<Actor*> actors(uint32_t types = 0xFFFFFFFF);
    std::vector<Actor*> getOwnedActors(Owner* owner, uint32_t types = 0xFFFFFFFF);


    template<class Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar.register_type(static_cast<Mug *>(NULL));
        ar.register_type(static_cast<CoffeeMachine *>(NULL));
        ar & m_actors & m_runtime;
    }

    // Key are all m_actors in the simluation...
    // Values are who owns another actor
    std::unordered_map<std::shared_ptr<Actor>, Owner*> m_actors;
    int m_runtime {};
};
}
