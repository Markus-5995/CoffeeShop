#include "coffeeshopmodule.hpp"
#include "v8engine.hpp"
#include "v8pp/class.hpp"
namespace CoffeeShop
{


struct CppModule::Impl
{
    Impl(std::unique_ptr<Producer> producer) :
        simulation(std::move(producer))
    {

    }
    V8Engine simulation;
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

    v8pp::class_<V8Engine> simulationClass(isolate);
    simulationClass
        .function("start", &V8Engine::start)
        .function("run", &V8Engine::run)
        .function("runtime", &V8Engine::runtime);

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
