#pragma once
#include "messagequeue/messagebus.hpp"
#include <variant>
namespace CoffeeShop
{
	template <typename T>
    concept MessageBusDriverConcept = requires(T driver,
        MessageBus::ProducerId pId,
        MessageBus::ConsumerId cId,
        Message msg)
    {
        { driver.registerAsServer(pId) };
        { driver.registerAsClient(cId) };
        { driver.alive() } -> std::convertible_to<bool>;
        { driver.tryPop(cId) } -> std::convertible_to<std::optional<Message>>;
        { driver.waitAndTryPop(cId) } -> std::convertible_to<std::optional<Message>>;
    };

    template <MessageBusDriverConcept T>
    struct DriverAssert : std::true_type {};

    template <typename ...Ts>
    struct DriverVariant : std::false_type {};

    template <typename ...Ts>
    struct DriverVariant<std::variant<Ts...>> : std::true_type
    {
        static constexpr bool value = (DriverAssert<Ts>::value && ...);
    };


}
