#pragma once

#include "entity.hpp"
#include "actortypes.hpp"
#include "conceptexport.hpp"
namespace CoffeeShop
{

struct CONCEPTS_API Mug : public Actor
{
    int temperature = 20;
    int fillStand = 0;

    static constexpr int s_threshold {20};

    void act() override;
    uint32_t type() const override;

    bool full() const;
    bool hot() const;

    template<typename Archive>
    void serialize(Archive & archive, const unsigned int)
    {
        archive & boost::serialization::base_object<Actor>(*this);
        archive & temperature & fillStand;
    }

private:
    friend class boost::serialization::access;
};

} // namespace CoffeeShop
