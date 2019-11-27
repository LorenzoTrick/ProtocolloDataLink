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
 * Contiene funzione per applicare il "frame synchronization" (o framing).
 */

#pragma once
#include "tools.h"

/**
 * @brief Aggiunge il bit stuffing al frame.
 * 
 * @param f Oggetto @c frame in cui aggiungere bit_stuffing.
 * @return true Se l'operazione è andata a buon termine.
 * @return false Se la grandezza supera quella del MAXEL.
 *               In pratica se f.size è pari o superiore a 256 - i flag = 254.
 */
bool bit_stuffing(frame &f)
{
    bool ans = true;

    if (f.size < FRAME_MAXEL - 2)
    {
        // Inserisce flag all'inizio
        insert_at(f.message, f.size, 0, (char)FLAG);

        //Cambia bit se vede un flag da mandare come messaggio
        for (int i = 1; i < f.size; i++)
            if (f.message[i] == (char)FLAG)
                f.message[i] -= 2; // Cambia penultimo bit

        // Inserisce flag alla fine
		insert_at(f.message, f.size, f.size, (char)FLAG);
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

void remove_byte_stuffing(frame &f)
{
	for (int i = 0; i < f.size - 1; i++)
	{
		if (is_special_char(f.message[i]) && f.message[i + 1] == f.message[i])
			delete_at(f.message, f.size, i);
	}
}
