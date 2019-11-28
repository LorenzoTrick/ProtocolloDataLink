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
 * Contiene i funzioni per applicare il controllo e la correzione sugli errori.
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
    // std::bitset definisce un set di <n> bit
    // In questo caso un set di 8 bit
    std::bitset<8> binary_char;

    for (int i = 0; i < f.size; i++)
    {
        // Trasforma ogni carattere del messaggio nel set di bit
        binary_char = (int)f.message[i];

        // Il metodo "count()" restituisce il numero di bit a 1
        f.checksum[i] = ((binary_char.count() % 2) == 0 ? '0' : '1');
    }
}

/**
 * @brief Controlla gli errori del frame (Con parity bit).
 * 
 * @param f Oggetto @c frame contenente il frame in cui controllare gli errori.
 * @return true Se non ci sono errori.
 * @return false Se sono presenti uno o più errori.
 */
bool check_parity_bit(frame &f)
{
    // Crea un frame uguale a quello passato per parametro.
    frame f_copy = f;
    // Esegue il parity bit sul frame copia.
    parity_bit(f_copy);

    bool ans = true;
    // Controlla che i checksum dei due frame siano uguali.
    for (int i = 0; i < f_copy.size; i++)
    {
        if (f_copy.checksum[i] != f.checksum[i])
        {
            ans = false;
            break;
        }
    }

    return ans;
}

/**
 * @brief Calcola il CRC del messaggio.
 * 
 * @param f Oggetto @c frame contenente il messaggio di cui calcolare il CRC.
 */
void crc(frame &f)
{
    // Costruisce il polinomio
    // e.g. f.message = {C, I, A, O} -> bits = 67736579
    // Ossia trasforma il char in int (C = 67, I = 73 ...) e li mette insieme
    unsigned long bits = 0;
    for (int i = 0; i < f.size; i++)
        bits = merge(bits, f.message[i]);

    // Trova il resto e lo trasforma in bit
    // Il massimo resto ottenibile da una divisione per 11 (polinomio generatore)
    // è di 10. Quindi servono 4 bit per memorizzarlo
    unsigned long rem = bits % polynom;
    std::bitset<4> remainder_bits(rem);

    // Inserisce il resto nel checksum
    int i = 0;
    for (char c : remainder_bits.to_string())
        f.checksum[i++] = c;
}

/**
 * @brief Controlla gli errori del frame (Con CRC).
 * 
 * @param f Oggetto @c frame contenente il frame in cui controllare gli errori.
 * @return true Se non ci sono errori.
 * @return false Se sono presenti uno o più errori.
 */
bool check_crc(frame &f)
{
    // Crea un frame uguale a quello passato per parametro.
    frame f_copy = f;
    // Esegue il CRC sul frame copia.
    crc(f_copy);

    bool ans = true;
    // Controlla che i checksum dei due frame siano uguali.
    for (int i = 0; i < 4; i++)
    {
        if (f_copy.checksum[i] != f.checksum[i])
        {
            ans = false;
            break;
        }
    }

    return ans;
}