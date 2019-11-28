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
#include <iostream>

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
const char FLAG = 0b01111110;

/**
 * @brief Polinomio generatore equivalente al decimale 11.
 * 
 * È usato per il calcolo del CRC.
 */
const int polynom = 0b1011;

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

/**
 * @brief Esegue lo shiftSx.
 * 
 * @param message char* su cui eseguire l'eliminazione dell'elemento.
 * @param size int contenente la grandezza del messaggio.
 * @param pos int contenente la posizione da eliminare.
 */
void delete_at(char *message, int &size, int pos)
{
    for (int i = pos; i < size - 1; i++)
        message[i] = message[i + 1];
    size--;
}

/**
 * @brief Esegue lo shiftDx.
 * 
 * @param message char* su cui eseguire l'aggiunta dell'elemento.
 * @param size int contenente la grandezza del messaggio.
 * @param pos int contenente la posizione da eliminare.
 * @param value char contenente il nuovo valore da inserire.
 */
void insert_at(char *message, int &size, int pos, char value)
{
    for (int i = size; i > pos; i--)
        message[i] = message[i - 1];
    message[pos] = value;
    size++;
}

/**
 * @brief Ritorna un booleano che indica se il carattere passato
 *        come parametro è un carattere speciale.
 * 
 * @param c char contente il carattere da controllare.
 * @return true se il carattere è speciale.
 * @return false se il carattere non è speciale.
 */
bool is_special_char(char c)
{
    return (c == ESC || c == STX || c == ETX);
}

/**
 * @brief Unisce due numeri nel seguente modo 2 + 7 = 27.
 * 
 * @param int1 int contenente il primo numero.
 * @param int2 int contenente il secondo numero.
 * @return int contenente l'unione dei due numeri.
 */
int merge(int int1, int int2)
{
    int int2_copy = int2;
    do
    {
        int1 *= 10;
        int2_copy /= 10;
    } while (int2_copy);

    return int1 + int2;
}

/**
 * @brief Classe per il livello fisico.
 * 
 * È una classe finta che serve solo per dei test.
 */
class Physical
{
public:
    Physical() {}

    void send(frame &f)
    {
        std::cout << "Frame inviato: ";
        for (int i = 0; i < f.size; i++)
            std::cout << f.message[i];
        std::cout << std::endl;
    }

    void receive(frame &f)
    {
        // Con bit stuffing
        f.size = 2;

        int i = 0;
        f.message[i++] = (char)FLAG;
        for (char c = 'A'; c <= 'Z'; c++)
        {
            f.message[i] = c;
            f.checksum[i] = '0';
            f.size++;
            i++;
        }
        f.message[i] = (char)FLAG;

        /*// Con byte stuffing
        f.size = 4;

        int i = 0;
        f.message[i++] = (char)ESC;
        f.message[i++] = (char)STX;
        for (char c = 'A'; c <= 'Z'; c++)
        {
            f.message[i] = c;
            f.checksum[i] = '0';
            f.size++;
            i++;
        }
        f.message[i++] = (char)ESC;
        f.message[i++] = (char)ETX;
        */
    }
};
