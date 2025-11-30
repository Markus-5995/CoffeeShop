#pragma once

#include "entity.hpp"
namespace CoffeeShop
{

struct Mug : public Actor
{
    int temperature = 20;
    int fillStand = 0;

    // Coffee below that temp is cold
    static void setTemperatureThreshold(int threshold);

    void act() override;
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

    static inline int s_threshold {20};
};

} // namespace CoffeeShop
