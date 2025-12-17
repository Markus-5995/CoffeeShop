#pragma once
#include "concepts/coffeemachine.hpp"

namespace CoffeeShop
{

class V8Actor
{
public:
    virtual ~V8Actor() = default;
    virtual std::shared_ptr<Actor> get() = 0;
};

struct V8Mug : public V8Actor
{
    V8Mug() :
        mug(std::make_shared<Mug>())
    {

    }

    bool full() const
    {
        if (mug != nullptr)
        {
            return mug->full();
        }
        return false;
    }

    bool hot() const
    {
        if (mug != nullptr)
        {
            return mug->hot();
        }
        return false;
    }

    std::shared_ptr<Actor> get() override
    {
        return mug;
    }

    std::shared_ptr<Mug> mug;
};

struct V8CoffeeMachine : public V8Actor
{
    V8CoffeeMachine() :
        coffeeMachine(std::make_shared<CoffeeMachine>())
    {

    }

    std::shared_ptr<Actor> get() override
    {
        return coffeeMachine;
    }

    std::shared_ptr<CoffeeMachine> coffeeMachine;
};

}
