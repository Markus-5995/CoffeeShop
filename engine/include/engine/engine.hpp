#pragma once
#include "messagequeue/producer.hpp"
#include <stop_token>
#include "engineexport.hpp"
namespace CoffeeShop
{

class ENGINE_API Engine
{
public:
    enum class EngineType
    {
        V8Engine,
        JSONEngine
    };
    explicit Engine(std::unique_ptr<Producer> producer);
    void run(std::stop_source&& stopSrc, EngineType engineType = EngineType::JSONEngine);
private:
    std::unique_ptr<Producer> producer;
};

} // namespace CoffeeShop
