#pragma once
#include "v8pp/call_v8.hpp"
#include "messagequeue/producer.hpp"
#include "v8wrappers.hpp"

namespace CoffeeShop
{
class Simulation
{
public:
    Simulation(std::unique_ptr<Producer> producer);

    void start(std::vector<V8Actor*> v8actors);

    void run(v8::Local<v8::Function> function);
    int runtime() const;

private:
    bool endConditionMet(v8::Local<v8::Function>& function, v8::Isolate* isolate);

    std::unique_ptr<SimulationContext> context;
    std::unique_ptr <Producer> m_producer;
};

} // namespace CoffeeShop
