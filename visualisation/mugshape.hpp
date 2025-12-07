#pragma once
#include "SFML/Graphics.hpp"
#include "concepts/mug.hpp"
#include "concepts/coffeemachine.hpp"
namespace CoffeeShop
{
class MugShape : public sf::Shape
{
public:
    MugShape(Mug* mug)
        : mugShape({20.f,20.f}),
        coffeeShape({20.f,0})
    {
        mugShape.setFillColor(sf::Color::Transparent);
        mugShape.setOutlineThickness(2.f);
        mugShape.setOutlineColor(sf::Color::White);

        sf::Color c;
        c.r = std::max(0.f, mug->temperature * 255.f / CoffeeMachine::temperatureLimit);
        c.b = 255 - c.r;

        auto size = coffeeShape.getSize();
        size.y = mug->fillStand * 20.f / 100.f;
        coffeeShape.setSize(size);
        coffeeShape.setFillColor(c);
        auto pos = coffeeShape.getPosition();
        pos.y = 20.f - size.y;
        coffeeShape.setPosition(pos);
    }


    std::size_t getPointCount() const override
    {
        return mugShape.getPointCount();
    }
    sf::Vector2f getPoint(std::size_t index) const override
    {
        return mugShape.getPoint(index);
    }

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        // Apply this object's transform
        states.transform *= getTransform();

        target.draw(coffeeShape, states);
        target.draw(mugShape, states);
    }

    sf::RectangleShape mugShape;
    sf::RectangleShape coffeeShape;
};


} // namespace CoffeeShop
