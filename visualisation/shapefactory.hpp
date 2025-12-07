#pragma once
#include "SFML/Graphics.hpp"
#include "concepts/entity.hpp"
#include "concepts/coffeemachine.hpp"
#include "concepts/actortypes.hpp"
#include "mugshape.hpp"
namespace CoffeeShop
{

class ShapeFactory
{
public:
    sf::Shape* create(Actor* actor)
    {
        switch(actor->type())
        {
        case ActorTypes::CoffeeMachineType:
            return createCM(actor);
        case ActorTypes::MugType:
            return createM(actor);
        }
        return nullptr;
    }

private:
    sf::Shape* createCM(Actor* actor)
    {
        auto* shape = new sf::CircleShape(10.f);

        if (auto* cm = dynamic_cast<CoffeeMachine*>(actor))
        {
            sf::Color c;
            c.r = std::max(0.f, cm->temperature * 255.f / CoffeeMachine::temperatureLimit);
            c.b = 255 - c.r;
            shape->setFillColor(c);
            shape->setOutlineColor(sf::Color::White);
            shape->setOutlineThickness(2.f);
        }
        return shape;
    }
    sf::Shape* createM(Actor* actor)
    {
        if (auto* cm = dynamic_cast<Mug*>(actor))
        {
            return new MugShape(cm);
        }
        return new sf::RectangleShape({20.f,20.f});;
    }
};

} // namespace CoffeeShop
