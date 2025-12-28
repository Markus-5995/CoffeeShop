#pragma once
#include <istream>
#include <memory>
#include "SFML/Graphics/Drawable.hpp"
#include "concepts/world.hpp"
#include "shapefactory.hpp"
#include <ranges>
#include <unordered_map>
namespace CoffeeShop
{

class ShopGenerator
{
    using ShapePtr = std::unique_ptr<sf::Shape>;
    using ShapeContainer = std::vector<ShapePtr>;
public:
    void update(std::istream& stream)
    {
        World world;
        boost::archive::binary_iarchive archive(stream);
        archive >> world;
        auto actors = world.actors();
        std::ranges::sort(actors, [](Actor* lhs, Actor* rhs) {return lhs->id() < rhs->id(); });
        auto view = actors
            | std::ranges::views::transform([factory = ShapeFactory()](Actor* actor) mutable
                {
                    return factory.create(actor);
                })
            | std::ranges::views::filter([](const auto& shape) {return shape != nullptr; });
        generatedShapes = { view.begin(), view.end() };
    }

    struct Iterator
    {
        using value_type = sf::Shape;
        using difference_type = std::ptrdiff_t;
        Iterator(ShapeContainer::iterator it) : it(it) {}
        Iterator(const Iterator& other) = default;
        Iterator(Iterator&& other) = default;
        Iterator() = default;
        Iterator& operator=(const Iterator& other) = default;
        Iterator& operator=(Iterator&& other) = default;
        value_type& operator*() const { return *(it->get()); }
        Iterator& operator ++() { it++; return *this; }
        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const Iterator& other) const { return  it == other.it;}

    private:
        ShapeContainer::iterator it;
    };

    Iterator begin()
    {
        return Iterator(generatedShapes.begin());
    }
    Iterator end()
    {
        return Iterator(generatedShapes.end());
    }
private:
    ShapeContainer generatedShapes {};
};

static_assert(std::forward_iterator<ShopGenerator::Iterator>);

} // namespace CoffeeShop
