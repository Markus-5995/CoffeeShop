#include "engine/coffeeshopmodule.hpp"
#include "simulation.hpp"
#include "v8pp/class.hpp"
namespace CoffeeShop
{


struct CppModule::Impl
{
    Impl(std::unique_ptr<Producer> producer) :
        simulation(std::move(producer))
    {

    }
    Simulation simulation;
};

CppModule::CppModule(std::unique_ptr<Producer> producer) :
    impl(std::make_unique<Impl>(std::move(producer)))
{
    
}

CppModule::~CppModule() = default;
CppModule::CppModule(CppModule &&) = default;
CppModule &CppModule::operator=(CppModule &&) = default;

v8pp::module CppModule::generateModule(v8::Isolate *isolate) const
{
    v8pp::class_<V8Actor> actorClass(isolate);

    v8pp::class_<V8Mug> mugClass(isolate);
    mugClass
        .ctor()
        .inherit<V8Actor>()
        .function("full", &Mug::full)
        .function("hot", &Mug::hot);

    v8pp::class_<V8CoffeeMachine> machineClass(isolate);
    machineClass
        .ctor()
        .inherit<V8Actor>();

    v8pp::class_<Simulation> simulationClass(isolate);
    simulationClass
        .function("start", &Simulation::start)
        .function("run", &Simulation::run)
        .function("runtime", &Simulation::runtime);

    v8pp::module module (isolate);
    module.class_("Actor", actorClass);
    module.class_("Mug", mugClass);
    module.class_("Machine", machineClass);
    module.function("start", [this](std::vector<V8Actor*> v8actors) {impl->simulation.start(v8actors);});
    module.function("run", [this](v8::Local<v8::Function> function) {impl->simulation.run(function);});
    module.property("runtime", [this]() {return impl->simulation.runtime();});
    return module;
}

std::string_view CppModule::name() const
{
    return "CoffeeShop";
}

}
