/**
 * @file main.cpp
 * @author Lanzani Andrea
 * @author Pirola Lorenzo
 * @brief App di test
 * @version 0.1
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "tools.h"
#include "checksum.h"
#include <iostream>
#include <string>

int main()
{
    frame f;
    f.size = 4;

    std::string messaggio;
    std::cin >> messaggio;

    int i = 0;
    for (char var : messaggio)
    {
        f.message[i] = var;
        i++;
    }

    parity_bit(f);
    std::cout << f.checksum;
    return 0;
}