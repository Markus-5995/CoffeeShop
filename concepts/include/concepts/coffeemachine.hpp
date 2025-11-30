#pragma once
#include "mug.hpp"
namespace CoffeeShop
{

struct CoffeeMachine : public Actor
{
public:
    void act() override;
    void interact(SimulationContext & sim) override;

    template<typename Archive>
    void serialize(Archive & archive, const unsigned int)
    {
        archive & boost::serialization::base_object<Actor>(*this);
        archive & temperature;
    }

private:
    friend class boost::serialization::access;
    Mug* mug{};
    int temperature = 20;
};

} // namespace CoffeeShop
