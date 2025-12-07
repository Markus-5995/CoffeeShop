#pragma once
#include "v8pp/call_v8.hpp"
#include "messagequeue/producer.hpp"
#include "concepts/simulationcontext.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "sstream"
#include "v8wrappers.hpp"
#include "ranges"

namespace CoffeeShop
{

std::vector<std::shared_ptr<Actor>> toActors(const std::vector<V8Actor*> v8Actors)
{
    auto actorsView = v8Actors
                      | std::views::filter([](V8Actor* actor){ return actor != nullptr; })
                      | std::views::transform([](V8Actor* actor){ return actor->get(); });

    return {actorsView.begin(), actorsView.end()};
}

class Simulation
{
public:
    Simulation(std::unique_ptr<Producer> producer) :
        m_producer(std::move(producer))
    {

    }

    void start(std::vector<V8Actor*> v8actors)
    {
        context.reset(new SimulationContext(toActors(v8actors)));
    }

    void run(v8::Local<v8::Function> function)
    {
        if(context == nullptr)
        {
            return;
        }

        auto* currentIsolate = v8::Isolate::GetCurrent();
        while (! endConditionMet(function, currentIsolate))
        {
            context->runSimulation();
            std::stringstream ss {};
            boost::archive::binary_oarchive archive {ss};
            archive << context->world;

            m_producer->push(Message(ss.str()));
        }
    }
    int runtime() const
    {
        if (context != nullptr)
        {
            return context->world.m_runtime;
        }
        return -1;
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

    std::unique_ptr<SimulationContext> context;
    std::unique_ptr<Producer> m_producer;
};

} // namespace CoffeeShop
