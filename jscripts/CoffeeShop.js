let machine = new CoffeeShop.Machine();
let mug = new CoffeeShop.Mug();
let mug2 = new CoffeeShop.Mug();

let sim = new CoffeeShop.Simulation([mug, mug2, machine]);

sim.run(endCondition);

"My Coffeemug is " + mug.hot()+ "(hot) " + mug.full() + "(full) after " + sim.runtime() +
"(Iterations) The other mug is " + mug2.hot()+ "(hot) " + mug2.full() + "(full)";

function endCondition()
{
    return sim.runtime() > 200;
}
