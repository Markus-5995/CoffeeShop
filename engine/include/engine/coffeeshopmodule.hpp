#pragma once
#include "v8pp-lab/concepts.hpp"
namespace CoffeeShop
{

struct CppModule
{
    v8pp::module generateModule(v8::Isolate* isolate) const;
    std::string_view name() const;
};

static_assert(V8ppLab::CppModule<CppModule>, "Must adhere to 'CppModule' concept!");
}
