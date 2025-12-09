#include "../streamwriter.hpp"
#include <gtest/gtest.h>

namespace CoffeeShop
{
TEST(WRITE, XML)
{
    XmlWriter writer ("/tmp/my.xml");
    XmlNodeHandle root = writer({"SIMULATION", {}});

    Node worldInfo {"STATE", {{"Runtime", "101"}}};
    auto worldNode = writer(root, worldInfo);

    Node actorInfo {"ACTOR", {{"ID", "43"}, {"TYPE", "Best"}}};
    writer(worldNode, actorInfo);
    actorInfo.name = "ACTOR_Nr2";
    writer(worldNode, actorInfo);

    worldNode = writer(root, worldInfo);

    writer(worldNode, actorInfo);

}

}
