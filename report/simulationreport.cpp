#include "report/simulationreport.hpp"

#include "concepts/world.hpp"
#include <sstream>
#include <iostream>
#include "simulationparser.hpp"
namespace CoffeeShop
{

SimulationReport::SimulationReport(std::unique_ptr<Consumer> consumer) :
    consumer(std::move(consumer))
{

}

void SimulationReport::run(std::string_view path, std::stop_token stop)
{

    SimulationParser parser (path);
    while(! stop.stop_requested() && consumer->alive())
    {
        std::string msg;
        auto potentialMessage = consumer->waitForNext();
        if (potentialMessage.has_value())
        {
            msg = *potentialMessage;
        }

        if (! msg.empty())
        {
            std::stringstream stream {};
            stream << msg;
            World world;
            boost::archive::binary_iarchive archive(stream);
            archive >> world;
            parser.addWorldState(world);
        }
    }
    std::cout << "Report finished." << std::endl;
}


} // namespace CoffeeShop
