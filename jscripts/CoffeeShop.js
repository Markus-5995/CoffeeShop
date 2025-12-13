function endCondition()
{
    return CoffeeShop.runtime > 20;
}

let interior = []

for (let i = 0; i < 5; i++)
{
    interior.push(new CoffeeShop.Mug())
}

for (let i = 0; i < 2; i++)
{
    interior.push(new CoffeeShop.Machine())
}

CoffeeShop.start(interior);
CoffeeShop.run(endCondition);
"Simulation finished!"
