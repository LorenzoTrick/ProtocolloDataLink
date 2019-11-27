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
            if (f.size == FRAME_MAXEL - 2 || i + 1 == p.size)
            {
                // Esegue parity bit e lo invia
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
        frame f;
        physical.receive(f);
        remove_bit_stuffing(f);

        // Ricostruisce il pacchetto
        p.size = 0;
        for (int i = 0; i < f.size; i++)
            p.message[p.size++] = f.message[i];
    }

private:
    frame f;
    Physical physical;
};

/**
 * @brief Simula il protocollo Heaven, con byte-stuffing.
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

	void send(packet& p)
	{
		int i = 0; //n. carattere sul pacchetto
		while (i < p.size)
		{
			frame f;
			//primi marcatori
			insert_at(f.message, f.size, 0, ESC);
			insert_at(f.message, f.size, 1, STX);

			//per ogni casella del messaggio
			for (int j = 2; j < FRAME_MAXEL - 2; j++)
			{
				//se non é finito il pacchetto
				if (i < p.size)
				{
					insert_at(f.message, f.size, j, p.message[i]);

					if (is_special_char(p.message[i])) //se speciale, duplica
					{
						j++;
						insert_at(f.message, f.size, j, p.message[i]);
					}

					i++;

					if (j == FRAME_MAXEL - 1) //manovra di sicurezza: se duplicando sono andato oltre
					{
						//cancella carattere
						//mi rendo conto che é brutto, ma si capisce cosa fa e funziona -piro
						delete_at(f.message, f.size, j);
						j--;
						delete_at(f.message, f.size, j);
						j--;
						i--;
					}
				}			
			}

			//ultimi marcatori
			insert_at(f.message, f.size, f.size, ESC);
			insert_at(f.message, f.size, f.size, ETX);

			//spedisci frame e resetta
			physical.send(f);
			f.size = 0;
		}
	}
};
