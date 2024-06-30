#include <iostream>
#include "src/MainContext.hpp"

int main()
{
    MainContext context;

    while (context.open())
    {
        context.pollEvents();
        context.clear();
        context.draw();
    }
}