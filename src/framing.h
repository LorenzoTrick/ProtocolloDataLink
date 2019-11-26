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
    bool ans = false;

    if (f.message[0] != '\0' || f.message[f.size + 1] != '\0')
        ans = false;
    else
    {
        f.message[0] = (int)FLAG;
        f.message[f.size + 1] = (int)FLAG;
        ans = true;
    }

    return ans;
}