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
#include "..\src\framing.h"
#include "..\src\checksum.h"
#include "..\src\flux.h"
#include <iostream>
#include <string>

// Funzioni di prova
void cin();
void test_bit_stuffing();
void test_parity_bit();
void test_stop_and_wait();
void test_crc();

frame f;
int main()
{
    test_crc();
    system("pause");
    return 0;
}

void cin()
{
    // Inserisce nel frame la stringa passata in ingresso
    std::string message;
    std::cout << "Inserire il messaggio: ";
    std::cin >> message;

    f.size = message.size();
    int i = 0;
    for (char c : message)
        f.message[i++] = c;
}

void test_bit_stuffing()
{
    cin();

    // Aggiunge bit stuffing e stampa il messaggio
    bit_stuffing(f);
    for (size_t i = 0; i < f.size; i++)
        std::cout << f.message[i];
    std::cout << std::endl;

    // Rimuove bit stuffing e stampa il messaggio
    remove_bit_stuffing(f);
    for (size_t i = 0; i < f.size; i++)
        std::cout << f.message[i];
    std::cout << std::endl;
}

void test_parity_bit()
{
    cin();

    //parity_bit(f);

    // Forzo il checksum a essere sbagliato
    for (size_t i = 0; i < f.size; i++)
        f.checksum[i] = '0';

    std::cout << (check_parity_bit(f) ? "Corretto" : "Non corretto") << std::endl;
}

void test_stop_and_wait()
{
    Heaven protocol = Heaven();

    packet p;
    p.size = 0;
    for (int i = 0; i < PACKET_MAXEL; i++)
    {
        if (i % 2 == 0)
            p.message[i] = 'A';
        else
            p.message[i] = 'C';

        p.size++;
    }

    protocol.send(p);

    protocol.receive(p);
    for (int i = 0; i < p.size; i++)
        std::cout << p.message[i];
    std::cout << std::endl;
}

void test_crc()
{
    cin();
    crc(f);
}