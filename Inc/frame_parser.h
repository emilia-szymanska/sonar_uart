/*
 * frame_parser.h
 *
 *  Created on: 28.05.2019
 *      Author: proxima
 *
 * Updated and modified according to the sonar's needs by emilia-szymanska
 *
 */

#ifndef FRAME_PARSER_H_
#define FRAME_PARSER_H_


#define START1_CHAR 'B'
#define START2_CHAR 'R'
#define SRC_DEVICE_ID 0
#define DST_DEVICE_ID 0
#define ID_DISTANCE_SIMPLE 1211
#define ID_SPEED_OF_SOUND 1203
#define ID_DISTANCE 1212

#include <stdint.h>

// type defining state of parser
typedef enum
{
	PARSER_ERROR = 0,	// incorrect data
	PARSER_COMPLETE,	// correct data received
	PARSER_INCOMPLETE	// waiting for data
} ParserStatus;

// Type defining parser according to data types being received during parsing
typedef enum
{
    STATE_WAIT,			// oczekiwanie na poczÄ…tek pakietu
    STATE_GOT_START1,		// odebrano start1
    STATE_GOT_START2, 		// odebrano start2
    STATE_GOT_PAYLOAD_LEN,	// odebrano dĹ‚ugoĹ›Ä‡ pakietu
    STATE_GOT_MESSAGE_ID,	// odebrano message_id
    STATE_GOT_SRC_DEVICE_ID,	// odebrano src_device_id
    STATE_GOT_DST_DEVICE_ID,	// odebrano dst_device_id
    STATE_GOT_PAYLOAD,		// odebrano zawartoĹ›Ä‡
} ParserState;


// Struktura opisujÄ…ca ramkÄ™ parsera
typedef struct
{
	uint16_t message_id;		// ID pakietu
	uint16_t length;		// DĹ‚ugoĹ›Ä‡ pakietu
	uint8_t data[256];		// Payload - dane zawarte w pakiecie	
	ParserState state;		// Status parsera - wykorzystywane przy odbieraniu
	uint32_t data_index;		// Index parsera - wykorzystywany przy odbieraniu
	uint16_t checksum;
} frame_t;

// Struktura opisujÄ…ca zawartoĹ›Ä‡ pakietu simple_distance
typedef struct
{
	uint32_t distance;
	uint8_t confidence; 
} simple_distance_t;

// Struktura opisujÄ…ca zawartoĹ›Ä‡ pakietu distance
typedef struct
{
	uint32_t distance;
	uint16_t confidence;
        uint16_t transmit_duration;
	uint32_t ping_number;
	uint32_t scan_start;
	uint32_t scan_length;
	uint32_t gain_setting;	
} distance_t;

// Wyliczenie sumy kontrolnej
uint16_t frame_GetCheckSum(frame_t *frame);

// Inicjalizacja ramki parsera
void Parser_InitFrame(frame_t* frame);

// Przetworzenie jednego bajtu
// byte - przetwarzany bajt
// working_frame - odbierana ramka
// return: status sygnalizujÄ…cy czy udaĹ‚o siÄ™ przetworzyÄ‡ pakiet
ParserStatus Parser_ParseByte(uint8_t byte, frame_t* frame);

//funkcje wyciÄ…gajÄ…ce z ramki dane typu SimpleDistance, SpeedOfSound, Distance
ParserStatus Parser_ParseSimpleDistance(frame_t *frame, simple_distance_t *out_data);
ParserStatus Parser_ParseSpeedOfSound(frame_t *frame, uint32_t *out_data);
ParserStatus Parser_ParseDistance(frame_t *frame, distance_t *out_data);

//requests
ParserStatus Parser_ParseSimpleDistanceRequest(frame_t *frame);
ParserStatus Parser_ParseSpeedOfSoundRequest(frame_t *frame);
ParserStatus Parser_ParseDistanceRequest(frame_t *frame);

// Funkcja tworzÄ…ca bufor gotowy do nadawania
// frame - ramka ktĂłra chcemy wysĹ‚aÄ‡
// out_buff - bufor do ktĂłrego jest wpisywana przetworzona ramka
// max_len - dĹ‚ugoĹ›Ä‡ bufora wyjsciowego -> USUNÄ�Ĺ�AM, BO NIE UĹ»YWAĹ�EĹš NIGDZIE W FUNKCJI
// len_out - zwraca informacjÄ™ ile bajtĂłw zapisano do bufora
void Parser_CreateTxBuffer(frame_t *frame, uint8_t out_buff[], uint32_t* len_out);

// Tworzy ramkÄ™ z zapytaniem o konkretne wartoĹ›ci
uint32_t Parser_CreateSimpleDistanceRequest(uint8_t out_buff[]);
uint32_t Parser_CreateSpeedOfSoundRequest(uint8_t out_buff[]);
uint32_t Parser_CreateDistanceRequest(uint8_t out_buff[]);


// Funkcje tworzÄ…ce ramki podanego typu, dla podanych danych
frame_t Parser_CreateSimpleDistanceFrame(simple_distance_t *sdistData);
frame_t Parser_CreateSpeedOfSoundFrame(uint32_t *speedData);
frame_t Parser_CreateDistanceFrame(distance_t *distData);



#endif /* FRAME_PARSER_H_ */
