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

/**
 * @brief Simula il protocollo Stop&Wait con bit stuffing
 */
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
                bit_stuffing(f);
                physical.send(f);

                // Aspetta ricevimento ack
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

        // Questo metodo riceve solo un frame, per riceverne altri sullo stesso pacchetto ripetere la receive nel main
    }

private:
    frame f;
    Physical physical;
};

/**
 * @brief Simula il protocollo a Stop&Wait per canale rumoroso con bit stuffing e parity bit
 */
class StopAndWaitNoisy
{
public:
    StopAndWaitNoisy()
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

                bool sent = false;
                do
                {
                    physical.send(f);

                    // Aspetta ricevimento ack
                    frame ack;
                    unsigned long time_out = 0; // Lo aspetta per 10mila volte
                    do
                    {
                        ack.size = 0;
                        physical.receive(ack);
                    } while (ack.message[0] != ACK || time_out++ > 10000);
                    if (ack.message[0] == ACK)
                        sent = true;
                } while (!sent); // Se ancora non è arrivato l'ACK, invia di nuovo il frame

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

class GoBackN
{
    public:
    GoBackN()
    {
        sentIndex = 0;
        receivedIndex = 0;
    }
    
    void setupFrame(frame &f)
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
        int j = 0; //posizione nel pacchetto
        
        //riempie e manda i primi 5 frame
        for (int i = 0; i < 5; i++)
        {
            //manda seq
            f[i].message[0] = (char)i;
            f[i].size++;
            // Costruisce il frame
            f[i].message[f.size] = p.message[j];
            f[i].size++;
            j++;

            // Se il frame è pieno (meno i due caratteri per il bit stuffing)
            // oppure se il messaggio del pacchetto è finito
            if (f[i].size >= FRAME_MAXEL - 2 || j + 1 == p.size)
            {
                // Esegue bit stuffing e lo invia
                bit_stuffing(f);
                physical.send(f);
            }
        }
        
        // Aspetta ricevimento ack
        frame ack;
        unsigned long time_out = 0; // Lo aspetta per 10mila volte
        do
        {
             ack.size = 0;
             physical.receive(ack);
        } while (ack.message[0] == '\0' || time_out++ > 10000);
        
        //se ack corretto
        if (ack.message[0] == (char)(sentIndex))
        {
            //togli frame da memoria
            for (int i = 0; i < 5; i++)
                f[i] = f[i + 1];
            sentIndex++;
            
            //manda quello dopo
            setupFrame(f[5]);
            //manda seq
            f[5].message[0] = (char)i;
            f[5].size++;
            // Costruisce il frame
            f[5].message[f.size] = p.message[j];
            f[5].size++;
            j++;

            // Se il frame è pieno (meno i due caratteri per il bit stuffing)
            // oppure se il messaggio del pacchetto è finito
            if (f[5].size >= FRAME_MAXEL - 2 || j + 1 == p.size)
            {
                // Esegue bit stuffing e lo invia
                bit_stuffing(f[5]);
                physical.send(f[5]);
            }
            
        }
        else //se ack non corretto (frame persi)
        {
            //rimanda tutto da quello perso
            for (int i = sentIndex; i < sentIndex + 5; i++)
            {
                physical.send(f[i]);
            }
        }
            
                    
    }
    
    void receive(packet &p)
    {
        frame rf;
        int j = 0; //posizione pacchetto
        
        physical.receive(rf);
        remove_bit_stuffing(rf);
        
        //se il seq corrisponde
        if (rf.message[1] == (char)(receivedIndex))
        {
            // Controllo sull'errore
            if (check_parity_bit(rf))
            {
                // Ricostruisce il pacchetto
                // Si dà per scontato che il pacchetto abbia size = 0
                // o che contenga già parte del messaggio e quindi abbia size = n
                for (int i = j; i < rf.size; i++)
                {
                    if (p.size < PACKET_MAXEL - 1)
                    {
                        p.message[p.size++] = rf.message[i];
                        j++;
                    }
                }
                
                receivedIndex++;
                
                // Se è giusto, invia l'ACK
                frame ack;
                ack.message[0] = (char)(receivedIndex);
                ack.size = 1;
                physical.send(ack);
            }
        }
    }
    
    private:
    frame f[5];
    int sentIndex;            
    int receivedIndex;
    Physical physical;
};

