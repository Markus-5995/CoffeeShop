#pragma once
#include <istream>
#include <memory>
#include "SFML/Graphics/Drawable.hpp"
#include "concepts/world.hpp"
#include "shapefactory.hpp"
#include <ranges>
namespace CoffeeShop
{

class ShopGenerator
{
public:
    ShopGenerator(std::istream& stream)
    {
        World world;
        boost::archive::binary_iarchive archive(stream);
        archive >> world;
        auto actors = world.actors();
        std::ranges::sort(actors, [](Actor* lhs, Actor* rhs) {return lhs->id() < rhs->id();});
        auto view = actors
                    | std::ranges::views::transform([factory = ShapeFactory()] (Actor* actor) mutable
                                                    {return factory.create(actor);})
                    | std::ranges::views::filter([](auto* shape){return shape != nullptr;});
        generatedShapes = {view.begin(), view.end()};
        it = generatedShapes.begin();
    }


    sf::Shape& next()
    {
        return *(*it++);
    }

    bool hasNext() const
    {
        return it != generatedShapes.end();
    }

private:
    std::vector<std::unique_ptr<sf::Shape>>::iterator it;
    std::vector<std::unique_ptr<sf::Shape>> generatedShapes {};

};

} // namespace CoffeeShop
