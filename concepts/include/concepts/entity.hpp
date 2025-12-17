#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include "conceptexport.hpp"
#include <boost/archive/binary_iarchive.hpp>
namespace CoffeeShop
{

class World;

class CONCEPTS_API Actor
{
    friend class boost::serialization::access;
public:
    virtual ~Actor() = default;
    virtual uint32_t type() const = 0;
    virtual void act() {}
    virtual void interact(World&) {}

    uint32_t id() const {return m_id;}
    void setId(uint32_t id) {m_id = id;}

    template<typename Archive>
    void serialize(Archive & archive, const unsigned int) { archive & m_id; }

private:
    uint32_t m_id {};
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Actor)


}
