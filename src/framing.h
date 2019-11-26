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
 * @return false Se il primo o l'ultimo elemento sono già pieni.
 */
bool bit_stuffing(frame &f)
{
    bool ans = true;

    if (f.message[0] != '\0' || f.message[f.size + 1] != '\0')
        ans = false;
    else if (f.size <= FRAME_MAXEL - 1)
    {
        f.message[0] = (char)FLAG;
        f.message[f.size + 1] = (char)FLAG;
        size++;
    }
    else //subottimale ma piú chiaro, probabilmente si puó fare meglio
        ans = false;
    
    //cambia bit se vede un flag da mandare come messaggio
    if (ans == true)
    {
        for (int i = 1; i <= f.size; i++)
            if (f.message[i] == (char)FLAG)
                f.message[i] -= 2; //cambia penultimo bit
    }

    return ans;
}

/**
 * @brief Aggiunge il byte stuffing/character stuffing al frame.
 * 
 * @param f Oggetto @c frame in cui aggiungere bit_stuffing.
 * @return true Se l'operazione è andata a buon termine.
 * @return false Se il primo o l'ultimo elemento sono già pieni.
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
        size += 2;
    }
    else //come bit_stuffing
        ans = false;
    
    //todo: duplica caratteri in f.message
    
    return ans;
}
