/**
 * @file tools.h
 * @author Lanzani Andrea
 * @author Pirola Lorenzo
 * @brief File di utilità.
 * @version 0.1
 * @date 2019-11-26
 *
 * @copyright Copyright (c) 2019
 *
 * @todo Aggiungere altre macro o struct se necessario.
 *
 * Contiene macro e struct utili per l'implementazione
 * dei protocolli negli altri file.
 * Inserire qui tutto ciò che può servire in più parti del codice principale.
 */

#pragma once

/**
 * @brief Definisce il carattere ASCII 'ESC'. 
 *
 * È usato nel Character Stuffing come carattere di sincronizzazione.
 */
const char ESC = 27;
/**
 * @brief Definisce il carattere ASCII 'STX'.
 *
 * È usato nel Character Stuffing per indicare lo 'Start of Text'.
 */
const char STX = 2;
/**
 * @brief Definisce il carattere ASCII 'ETX'.
 *
 * È usato nel Character Stuffing per indicare lo 'End of Text'.
 */
const char ETX = 3;

/**
 * @brief Definisce il flag per il bit stuffing.
 *
 * La sintassi qui sotto di @c FLAG è equivalente alla "string".
 */
const char *FLAG = "01111110";

/**
 * @brief Definisce la lunghezza massima di un frame.
 */
#define FRAME_MAXEL 256

/**
 * @brief Struttura di un frame.
 */
struct frame
{
    /** Messaggio del frame. */
    char message[FRAME_MAXEL];
    /**
     * @brief Controllo sull'errore.
     * 
     * La lunghezza del checksum è uguale a quella del messaggio.
     * In caso di parity bit servirà infatti un bit per carattere.
     */
    char checksum[FRAME_MAXEL];
    /** Equivalente al numEl. */
    int size;
};

/**
 * @brief Definisce la lunghezza massima di un pacchetto. 
 */
#define PACKET_MAXEL 512

/**
 * @brief Struttura di un pacchetto.
 */
struct packet
{
    /** Messaggio del pacchetto. */
    char message[512];
    /** Equivalente al numEl. */
    int size;
};