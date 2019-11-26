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
#include "..\src\tools.h"
#include "..\src\checksum.h"
#include <iostream>
#include <string>

int main()
{
    frame f;
    f.size = 4;

    std::string messaggio;
    std::cout << "Inserire il messaggio: ";
    std::cin >> messaggio;

    int i = 0;
    for (char var : messaggio)
    {
        f.message[i] = var;
        i++;
    }

    parity_bit(f);
    i = 0;
    for (i = 0; i < f.size; i++)
    {
        std::cout << f.checksum[i];
    }

    std::cout << std::endl;
    system("pause");
    return 0;
}