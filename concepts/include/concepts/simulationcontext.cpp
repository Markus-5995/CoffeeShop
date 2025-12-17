#include "concepts/simulationcontext.hpp"
#include "simulationcontext.hpp"

namespace CoffeeShop
{

	SimulationContext::SimulationContext(const std::vector<std::shared_ptr<Actor>>& newActors)
	{
		for (const auto& actor : newActors)
		{
			if (actor != nullptr)
			{
				actor->setId(nextId++);
				world.m_actors[actor] = nullptr;
			}
		}
	}

	void SimulationContext::runSimulation()
	{
		for (const auto& actor : world.m_actors)
		{
			actor.first->act();
		}
		for (const auto& actor : world.m_actors)
		{
			actor.first->interact(world);
		}
		world.m_runtime++;
	}

	const World& SimulationContext::getWorld() const
	{
		return world;
	}

}
