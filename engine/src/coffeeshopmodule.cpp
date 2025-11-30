#include "engine/coffeeshopmodule.hpp"
#include "simulation.hpp"
#include "v8pp/class.hpp"
namespace CoffeeShop
{

v8pp::module CppModule::generateModule(v8::Isolate *isolate) const
{

    v8pp::class_<Actor> actorClass(isolate);

    v8pp::class_<Mug> mugClass(isolate);
    mugClass.ctor()
        .inherit<Actor>()
        .function("full", &Mug::full)
        .function("hot", &Mug::hot);

    v8pp::class_<CoffeeMachine> machineClass(isolate);
    machineClass.ctor()
        .inherit<Actor>();

    v8pp::class_<Simulation> simulationClass(isolate);
    simulationClass.ctor<std::vector<Actor*>>()
        .function("run", &Simulation::run)
        .function("runtime", &Simulation::runtime);

    v8pp::module module (isolate);
    module.class_("Actor", actorClass);
    module.class_("Mug", mugClass);
    module.class_("Machine", machineClass);
    module.class_("Simulation", simulationClass);
    return module;
}

std::string_view CppModule::name() const
{
    return "CoffeeShop";
}

}
