#include "engine/simulation.hpp"
#include <ranges>
#include <iostream>

namespace CoffeeShop
{
	std::vector<std::shared_ptr<Actor>> toActors(const std::vector<V8Actor*> v8Actors)
	{
		auto actorsView = v8Actors
			| std::views::filter([](V8Actor* actor) { return actor != nullptr; })
			| std::views::transform([](V8Actor* actor) { return actor->get(); });

		return { actorsView.begin(), actorsView.end() };
	}

	Simulation::Simulation(std::unique_ptr<Producer> producer) :
		m_producer(std::move(producer))
	{

	}

	void Simulation::start(std::vector<V8Actor*> v8actors)
	{
		context.reset(new SimulationContext(toActors(v8actors), std::move(m_producer));
	}

	void Simulation::run(v8::Local<v8::Function> function)
	{
		if (context == nullptr)
		{
			return;
		}

		auto* currentIsolate = v8::Isolate::GetCurrent();
		while (!endConditionMet(function, currentIsolate))
		{
			context->runSimulation();
		}
	}
	int Simulation::runtime() const
	{
		if (context != nullptr)
		{
			return context->getWorld().m_runtime;
		}
		return -1;
	}
	bool Simulation::endConditionMet(v8::Local<v8::Function>& function, v8::Isolate* isolate)
	{
		auto context = isolate->GetCurrentContext();
		v8::Local<v8::Value> recv = context->Global();

		v8::Local<v8::Value> result = v8pp::call_v8(isolate, function, recv);
		if (!result.IsEmpty())
		{
			return v8pp::from_v8<bool>(isolate, result);
		}
		return true;
	}
}