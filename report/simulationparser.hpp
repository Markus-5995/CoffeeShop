#pragma once
#include <string_view>
#include <filesystem>
#include "concepts/world.hpp"
#include "streamwriter.hpp"


namespace CoffeeShop {

class SimulationParser
{
public:
    SimulationParser(std::string_view filepath) :
        m_writer(filepath)
    {
        m_root = m_writer({"SIMULATION", {}});
    }

    void addWorldState(World& w)
    {
        std::string runtime = std::to_string(w.m_runtime);
        Node worldInfo {"STATE", {{"Runtime", runtime}}};

        auto worldNode = m_writer(m_root, worldInfo);


        auto actors = w.actors();
        for (const Actor* actor : actors)
        {
            std::string id = std::to_string(actor->id());
            std::string type = std::to_string(actor->type());

            Node actorInfo {"ACTOR", {{"ID", id}, {"TYPE", type}}};
            m_writer(worldNode, actorInfo);

        }
    }

private:
    XmlWriter m_writer;
    XmlNodeHandle m_root;
};

} // namespace CoffeeShop
