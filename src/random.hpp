#ifndef RANDOM_H
#define RANDOM_H



#include <random>


float getRandom(int min,int max)
{


    std::mt19937 mt{ std::random_device{}() }; // creating and seeding the PRNG

    std::uniform_int_distribution<int> range{ min,max };// fixing the range 

    
    return static_cast<float>(range(mt));

}
#endif // !RANDOM_H
