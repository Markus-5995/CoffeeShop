#pragma once
#include "messagequeue/consumer.hpp"
#include "iostream"
#include <boost/archive/text_iarchive.hpp>
#include <sstream>
#include "concepts/world.hpp"
#include "SFML/Graphics.hpp"
#include <format>
namespace CoffeeShop
{

class Visualiser
{
public:
    Visualiser();


    // BLOCKS
    void run()
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

            std::string msg (consumer.get());

            World w (true);
            if (! msg.empty())
            {
                std::stringstream ss {};
                ss << msg;
                boost::archive::binary_iarchive archive(ss);
                archive >> w;

                std::cout << "New message with " << w.m_actors.size() << " entities!" << std::endl;
            }

            // clear the window with black color
            window.clear(sf::Color::Black);

            // draw everything here...
            sf::RectangleShape shape({50.f,50.f});
            shape.setFillColor(sf::Color(100, 250, 50));
            shape.setPosition({400-25, 300-25});

            window.draw(shape);

            // end the current frame
            window.display();
        }
    }

private:
    Consumer consumer {};
};

} // namespace CoffeeShop
