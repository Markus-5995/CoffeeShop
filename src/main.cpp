#include "v8pp-lab/runner.hpp"
#include "v8pp-lab/scripts.hpp"
#include "v8pp-lab/filelocator.hpp"
#include "v8pp-lab/context.hpp"
#include "engine/coffeeshopmodule.hpp"
#include "visualisation/visualiser.hpp"

#include <thread>

void runV8()
{
    using namespace V8ppLab;
    FileLocator locator (DEFAULT_JS_DIR);
    constexpr CoffeeShop::CppModule coffeeModule {};

    std::tuple<std::vector<FileScript>> scripts{ locator.getScripts() };
    std::tuple<std::vector<CoffeeShop::CppModule>> modules { std::vector {coffeeModule} };

    Runner<V8Context> runner {};
    runner.loadModules(modules);
    runner.run(scripts);
}

void runVisualizer()
{
    CoffeeShop::Visualiser().run();
}

int main()
{
    std::jthread v8Thread ([](){runV8();});
    std::jthread visThread ([](){runVisualizer();});

    return 0;

}
