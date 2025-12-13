#include "visualisation/visualiser.hpp"
#include "shopgenerator.hpp"
#include "iostream"
#include <boost/archive/text_iarchive.hpp>
#include <sstream>
#include "concepts/world.hpp"
#include "SFML/Graphics.hpp"
#include <format>

namespace CoffeeShop
{

Visualiser::Visualiser(std::unique_ptr<Consumer> consumer) :
    consumer(std::move(consumer))
{
}

void Visualiser::run()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    std::string msg;
    bool setup {};
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        if (setup)
        {

            auto potentialMessage = consumer->get();
            if (potentialMessage.has_value() && ! potentialMessage->operator std::string().empty())
            {
                msg = *potentialMessage;
            }

            if (! msg.empty())
            {
                std::stringstream ss {};
                ss << msg;
                ShopGenerator generator (ss);
                sf::Vector2f position {5.f, 5.f};
                while(generator.hasNext())
                {
                    sf::Shape& ref = generator.next();
                    ref.setPosition(position);
                    window.draw(ref);
                    position.x += 50;
                }

            }

        }
        else
        {
            setup = true;
        }

        // end the current frame
        window.display();

    }
}

} // namespace CoffeeShop
