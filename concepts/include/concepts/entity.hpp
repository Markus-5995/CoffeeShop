#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
namespace CoffeeShop
{

class SimulationContext;

class Actor
{
    friend class boost::serialization::access;
public:
    virtual ~Actor() = default;
    virtual void act() {}
    virtual void interact(SimulationContext&) {}
    template<typename Archive>
    void serialize(Archive & , const unsigned int)
    {
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Actor)


}
