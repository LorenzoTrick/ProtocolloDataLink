/**
 * @file framing.h
 * @author Lanzani Andrea
 * @author Pirola Lorenzo
 * @brief File per il framing
 * @version 0.1
 * @date 2019-11-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once
#include "tools.h"

/**
 * @brief Aggiunge il bit stuffing al frame.
 * 
 * @param f Oggetto @c frame in cui aggiungere bit_stuffing.
 * @return true Se l'operazione è andata a buon termine.
 * @return false Se la grandezza supera quella del MAXEL.
 *               In pratica se la grandezza è pari o superiore a 255.
 */
bool bit_stuffing(frame &f)
{
    bool ans = true;

    if (f.size < FRAME_MAXEL - 1)
    {
        // Inserisce flag all'inizio
        insert_at(f.message, f.size, 0, (char)FLAG);

        //Cambia bit se vede un flag da mandare come messaggio
        for (int i = 1; i <= f.size; i++)
            if (f.message[i] == (char)FLAG)
                f.message[i] -= 2; // Cambia penultimo bit

        // Inserisce flag alla fine
        f.message[f.size++] = (char)FLAG;
    }
    else
        ans = false;

    return ans;
}

/**
 * @brief Rimuove il bit stuffing dal frame.
 * 
 * @param f Oggetto @c frame su cui rimuovere bit_stuffing.
 */
void remove_bit_stuffing(frame &f)
{
    for (int i = 0; i < f.size; i++)
    {
        if (f.message[i] == (char)FLAG)
            delete_at(f.message, f.size, i);
        if (f.message[i] == (char)(FLAG - 2))
            f.message[i] += 2; // Il carattere diventa quello originale uguale al FLAG.
    }
}

/**
 * @brief Aggiunge il byte stuffing/character stuffing al frame.
 * 
 * @param f Oggetto @c frame in cui aggiungere byte stuffing.
 * @return true Se l'operazione è andata a buon termine.
 * @return false Se il primo o l'ultimo elemento sono già pieni.
 * 
 * @todo Usare i metodi delete_at e insert_at in modo da evitare di dover
 *       lasciare uno spazio vuoto nel vettore per far posto al byte stuffing.
 *       Guardare la funzione bit stuffing per essere sicuri.
 */
bool byte_stuffing(frame &f)
{
    bool ans = true;

    if (f.message[0] != '\0' || f.message[1] != '\0' || f.message[f.size + 1] != '\0' || f.message[f.size + 2] != '\0')
        ans = false;
    else if (f.size <= FRAME_MAXEL - 2)
    {
        f.message[0] = ESC;
        f.message[1] = STX;
        f.message[f.size + 1] = ETX;
        f.message[f.size + 2] = ESC;
        f.size += 2;
    }
    else //come bit_stuffing
        ans = false;

    //todo: duplica caratteri in f.message

    return ans;
}
