#include "visualisation/visualiser.hpp"
#include "shopgenerator.hpp"
namespace CoffeeShop
{

Visualiser::Visualiser(std::unique_ptr<Consumer> consumer) :
    consumer(std::move(consumer))
{
}

void Visualiser::run()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::string msg;
        auto potentialMessage = consumer->get();
        if (potentialMessage.has_value())
        {
            msg = *potentialMessage;
        }

        if (! msg.empty())
        {
            std::stringstream ss {};
            ss << msg;
            ShopGenerator generator (ss);

            // clear the window with black color
            window.clear(sf::Color::Black);

            sf::Vector2f position {5.f, 5.f};
            while(generator.hasNext())
            {
                sf::Shape& ref = generator.next();
                ref.setPosition(position);
                window.draw(ref);
                position.x += 50;
            }

            // end the current frame
            window.display();
            usleep(500000);
        }

    }
}

} // namespace CoffeeShop
