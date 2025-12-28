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
		sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
		ShopGenerator  generator{};
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
			auto potentialMessage = consumer->get();
			if (potentialMessage.has_value())
			{
				std::stringstream ss{};
				ss << potentialMessage->operator std::string();
				generator.update(ss);
			}

			sf::Vector2f position{ 5.f, 5.f };
			for (auto& ref : generator)
			{
				ref.setPosition(position);
				window.draw(ref);
				position.x += 50;
			}
			// end the current frame
			window.display();

		}
	}

} // namespace CoffeeShop
