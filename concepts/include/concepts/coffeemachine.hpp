#pragma once
#include "mug.hpp"
#include "conceptexport.hpp"
namespace CoffeeShop
{

class CONCEPTS_API CoffeeMachine : public Actor
{
public:
    void act() override;
    void interact(World &world) override;
    uint32_t type() const override
    {
        return ActorTypes::CoffeeMachineType;
    }

    template<typename Archive>
    void serialize(Archive & archive, const unsigned int)
    {
        archive & boost::serialization::base_object<Actor>(*this);
        archive & temperature;
    }
    static constexpr int temperatureLimit = 40;

    int temperature = 20;
private:
    friend class boost::serialization::access;
    int slots = 2;
};

} // namespace CoffeeShop
