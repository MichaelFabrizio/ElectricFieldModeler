#include "MCIntegrator.h"
#include <iostream>
#include <math.h>

MCIntegrator::MCIntegrator()
{
    
}

void MCIntegrator::Integrate()
{
    std::mt19937 gen(rd_());

    std::uniform_real_distribution<> dis1(0, 2);
    std::uniform_real_distribution<> dis2(1, 2);
    std::uniform_real_distribution<> dis3(0, 3.14);

    double sum = 0;

    for (size_t i = 0; i < 1000; i++)
    {
        sum+= dis1(gen) * dis2(gen) * std::sin(dis3(gen));
    }

    double normalized = sum * 2 * 1 * 3.14 / 1000;
    std::cout << normalized << std::endl;

    //std::random_device rd;
    //for (int n = 0; n < 10; ++n) 
    //{
    //    std::cout << dis_(gen) << '\n';
    //}
}