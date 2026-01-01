# CoffeeShop
Who hasn’t dreamed of owning a coffee shop? I certainly haven’t.

This project is supposed to showcase some modern c++ features in a whimsical project. As a result,
it is more a playground than anything else.

## Concept
The idea behind this project is to write a simple simulation of a coffee shop, while using different technologies.
The heart piece of the project is actually the *Message Queue*. This component allows to communicate the state of
the simulation/world in different ways like, in memory or via TCP IP sockets.
Morever V8 is used to script the configuration of the shop and define a stop condition. It is also possible to
write an xml report file about the state of the shop in every frame.

### Simulation
The simulation itself is designed with the following idea in mind: 

*Update State* -> *Interact with World* -> *Repeat*

This is done for every actor. Once every actor has completed he *Update State* operations only then the first actor
gets to *Interact with World*.

### V8 Integration
V8pp (with my layer on top https://github.com/Markus-5995/V8pp-Lab.git) is used to establish a connection between
C++ and Java Script. An interesting point about this integration is *life time managment*. For resources that are
shared between C++ and Java Script, two types of *life time managment systems* collide. JS's garbage collector and 
C++ "self managed clean up". In order to synergize these two mechanisms the following solution has been chosen by the
Coffee Shop.
```
struct MyActor
{
	void doStuff() {std::cout << "Stuff" << std::endl;}
};
struct V8Wrapper
{
	V8Wrapper() : m_actor(std::make_shared<MyActor>()) {}
	V8Wrapper(std::shared_ptr<MyActor> actor) : m_actor(actor) {}
	void doStuff () {m_actor->doStuff();}
	std::shared_ptr<MyActor> m_actor {};
};
```
As one can see, this allows Java Script to create its own instance of MyActor and share it with C++ in a safe fashion.
With this approach, it doesn't matter when JS's garbage collector cleans up the wrapper, as long as C++ holds a 
reference, the underlying actor is still valid.
