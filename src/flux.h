/**
 * @file flux.h
 * @author Lanzani Andrea
 * @author Pirola Lorenzo
 * @brief File per il controllo del flusso.
 * @version 0.1
 * @date 2019-11-27
 * 
 * @copyright Copyright (c) 2019
 * 
 * Contiene classi che simulano il controllo del flusso di un protocollo Data Link.
 */

#pragma once
#include "tools.h"
#include "framing.h"
#include "checksum.h"

/**
 * @brief Simula il protocollo Heaven.
 * 
 * In questo caso si usa il bit stuffing.
 */
class Heaven
{
public:
    /**
     * @brief Construct a new Heaven object.
     */
    Heaven()
    {
        for (int i = 0; i < FRAME_MAXEL; i++)
        {
            f.message[i] = '\0';
            f.checksum[i] = '\0';
        }
        f.size = 0;
    }

    /**
     * @brief Costruisce i frame dal pacchetto e li invia.
     * 
     * @param p Oggetto @c packet da inviare.
     */
    void send(packet &p)
    {
        for (int i = 0; i < p.size; i++)
        {
            // Costruisce il frame
            f.message[f.size] = p.message[i];
            f.size++;

            // Se il frame è pieno (meno i due caratteri per il bit stuffing)
            // oppure se il messaggio del pacchetto è finito
            if (f.size >= FRAME_MAXEL - 2 || i + 1 == p.size)
            {
                // Esegue bit stuffing e lo invia
                bit_stuffing(f);
                physical.send(f);

                // Resetta il frame
                f.size = 0;
            }
        }
    }

    /**
     * @brief Ricostruisce il pacchetto, dopo aver ricevuto i frame.
     * 
     * @param p Oggetto @c packet da ricostruire.
     */
    void receive(packet &p)
    {
        // Riceve il frame e rimuove il bit stuffing
        physical.receive(f);
        remove_bit_stuffing(f);

        // Ricostruisce il pacchetto
        // Si dà per scontato che il pacchetto abbia size = 0
        // o che contenga già parte del messaggio e quindi abbia size = n
        for (int i = 0; i < f.size; i++)
            if (p.size < PACKET_MAXEL - 1)
                p.message[p.size++] = f.message[i];

        // Questo metodo riceve solo un frame, per riceverne altri sullo stesso pacchetto ripetere la receive nel main
    }

private:
    frame f;
    Physical physical;
};

/**
 * @brief Simula il protocollo Heaven, con byte-stuffing.
 * @note A titolo informativo, ora si possono anche usare i metodi byte_stuffing e remove_byte_stuffing e crearla come la classe Heaven sopra, con qualche accortezza.
 */
class HeavenByte
{
public:
    HeavenByte()
    {
        for (int i = 0; i < FRAME_MAXEL; i++)
        {
            f.message[i] = '\0';
            f.checksum[i] = '\0';
        }
        f.size = 0;
    }

    void send(packet &p)
    {
        int count = 0; // Contatore dei caratteri da duplicare
        for (int i = 0; i < p.size; i++)
        {
            // Costruisce il frame
            f.message[f.size] = p.message[i];
            f.size++;

            // Se il frame è pieno (meno i quattro caratteri per il byte stuffing
            // e quelli per la duplicazione)
            // oppure se il messaggio è finito
            if (f.size >= FRAME_MAXEL - 4 - count || i + 1 == p.size)
            {
                // Esegue byte stuffing e lo invia
                if (byte_stuffing(f))
                {
                    physical.send(f);
                    f.size = 0;
                }
                else // Se non riesce perché l'ultimo carattere è un ESC
                {
                    // Invia il messaggio senza l'ultimo carattere
                    f.size--;
                    byte_stuffing(f);
                    physical.send(f);

                    // Resetta il frame e ci inserisce già il carattere ESC
                    f.size = 0;
                    f.message[f.size++] = p.message[i];
                    // Se è l'ultimo carattere del pacchetto lo invia
                    if (i + 1 == p.size)
                    {
                        byte_stuffing(f);
                        physical.send(f);
                    }
                }
            }
        }
    }

    void receive(packet &p)
    {
        // Riceve il frame e rimuove il byte stuffing
        physical.receive(f);
        remove_byte_stuffing(f);

        // Ricostruisce il pacchetto
        // Si dà per scontato che il pacchetto abbia size = 0
        // o che contenga già parte del messaggio e quindi abbia size = n
        for (int i = 0; i < f.size; i++)
            if (p.size < PACKET_MAXEL - 1)
                p.message[p.size++] = f.message[i];
    }

private:
    frame f;
    Physical physical;
};

class StopAndWait
{
public:
    StopAndWait()
    {
        for (int i = 0; i < FRAME_MAXEL; i++)
        {
            f.message[i] = '\0';
            f.checksum[i] = '\0';
        }
        f.size = 0;
    }

    void send(packet &p)
    {
        for (int i = 0; i < p.size; i++)
        {
            // Costruisce il frame
            f.message[f.size] = p.message[i];
            f.size++;

            // Se il frame è pieno (meno i due caratteri per il bit stuffing)
            // oppure se il messaggio del pacchetto è finito
            if (f.size >= FRAME_MAXEL - 2 || i + 1 == p.size)
            {
                // Esegue parity bit e bit stuffing e lo invia
                parity_bit(f);
                bit_stuffing(f);
                physical.send(f);

                //poi aspetta ricevimento ack
                frame ack;
                do
                {
                    ack.size = 0;
                    physical.receive(ack);
                } while (ack.message[0] != ACK);

                // Resetta il frame
                f.size = 0;
            }
        }
    }

    void receive(packet &p)
    {
        // Riceve il frame e rimuove il bit stuffing
        physical.receive(f);
        remove_bit_stuffing(f);

        // Controllo sull'errore
        if (check_parity_bit(f))
        {
            // Se è giusto, invia l'ACK
            frame ack;
            ack.message[0] = ACK;
            ack.size = 1;
            physical.send(ack);

            // Ricostruisce il pacchetto
            // Si dà per scontato che il pacchetto abbia size = 0
            // o che contenga già parte del messaggio e quindi abbia size = n
            for (int i = 0; i < f.size; i++)
                if (p.size < PACKET_MAXEL - 1)
                    p.message[p.size++] = f.message[i];
        }
        // Questo metodo riceve solo un frame, per riceverne altri sullo stesso pacchetto ripetere la receive nel main
    }

private:
    frame f;
    Physical physical;
};
