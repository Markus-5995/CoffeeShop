#pragma once
#include "v8pp/call_v8.hpp"
#include "messagequeue/producer.hpp"
#include "concepts/simulationcontext.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "sstream"

namespace CoffeeShop
{
class Simulation
{
public:
    Simulation(std::vector<Actor*> actors) :
        context(actors)
    {

    }
    void run(v8::Local<v8::Function> function)
    {
        auto* currentIsolate = v8::Isolate::GetCurrent();
        while (! endConditionMet(function, currentIsolate))
        {
            context.runSimulation();
            std::stringstream ss {};
            boost::archive::binary_oarchive archive {ss};
            archive << context.world;

            m_producer.push(Message(ss.str()));
        }
    }
    int runtime() const
    {
        return context.runtime();
    }

private:
    bool endConditionMet(v8::Local<v8::Function>& function, v8::Isolate* isolate)
    {
        auto context = isolate->GetCurrentContext();
        v8::Local<v8::Value> recv = context->Global();

        v8::Local<v8::Value> result = v8pp::call_v8(isolate, function, recv);
        if (! result.IsEmpty())
        {
            return v8pp::from_v8<bool>(isolate, result);
        }
        return true;
    }


    SimulationContext context;
    Producer m_producer {};
};

} // namespace CoffeeShop
