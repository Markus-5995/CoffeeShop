#pragma once
#include "v8pp-lab/concepts.hpp"
namespace CoffeeShop
{
class Producer;
struct CppModule
{
    CppModule(std::unique_ptr<Producer> producer);
    ~CppModule();
    CppModule(CppModule&&);
    CppModule& operator=(CppModule&&);

    v8pp::module generateModule(v8::Isolate* isolate) const;
    std::string_view name() const;
    struct Impl;
    std::unique_ptr<Impl> impl;
};

static_assert(V8ppLab::CppModule<CppModule>, "Must adhere to 'CppModule' concept!");
}
