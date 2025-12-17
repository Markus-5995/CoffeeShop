#pragma once
#include "world.hpp"
#include "conceptexport.hpp"
namespace CoffeeShop
{
	class CONCEPTS_API SimulationContext
	{
	public:
		SimulationContext(const std::vector<std::shared_ptr<Actor>>& newActors);
		void runSimulation();
		const World& getWorld() const;
	private:
		World world{};
		static inline uint32_t nextId = 1;
	};

}
