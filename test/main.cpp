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
#include "..\src\framing.h"
#include <iostream>
#include <string>

int main()
{
    frame f;

    std::string message;
    std::cout << "Inserire il messaggio: ";
    std::cin >> message;

    f.size = message.size();
    f.message[0] = '\0';
    f.message[f.size - 1] = '\0';
    int i = 1;
    for (char var : message)
    {
        f.message[i] = var;
        i++;
    }

    bit_stuffing(f);
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