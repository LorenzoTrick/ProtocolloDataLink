/**
 * @file checksum.h
 * @author Lanzani Andrea
 * @author Pirola Lorenzo
 * @brief File per il controllo sugli errori.
 * @version 0.1
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 * Contiene i metodi per il controllo e la correzione sugli errori.
 */

#pragma once
#include "tools.h"
#include <bitset>

/**
 * @brief Calcola il parity bit del messaggio.
 *
 * Calcola il parity bit sul messaggio del frame e lo aggiunge al campo @c checksum 
 * della struct.
 * 
 * @param f Oggetto @c frame contenente il messaggio di cui calcolare il parity bit.
 */
void parity_bit(frame &f)
{
    std::bitset<8> binary_char;
    for (int i = 0; i < f.size; i++)
    {
        binary_char = (int)f.message[i];
        f.checksum[i] = ((binary_char.count() % 2) == 0 ? '0' : '1');
    }
}