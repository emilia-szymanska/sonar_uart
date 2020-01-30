/*
 * frame_parser.c
 *
 *  Created on: 28.05.2019
 *      Author: proxima
 *
 *  Updated and modified according to the sonar's needs by emilia-szymanska
 */

#include "frame_parser.h"

// checksum is a sum of every bite in frame
uint16_t frame_GetCheckSum(frame_t *frame) 
{
	uint16_t sum = 0;
	sum += frame->message_id;
	sum += frame->length;
	sum += SRC_DEVICE_ID;
	sum += DST_DEVICE_ID;
	sum += START1_CHAR;
	sum += START2_CHAR;

	for (int i=0; i < frame->length; i++)
	{
		sum += frame->data[i];
	}

	return sum;
}

// setting initial values of a parser
void Parser_InitFrame(frame_t* frame) 
{
	frame->data_index = 0;
	frame->length = 0;
	frame->state = STATE_WAIT;
	frame->message_id = 0;
	for (int i=0; i < 256; i++)
	{
		frame->data[i] = 0;
	}
	frame->checksum = 0;
}


ParserStatus Parser_ParseByte(uint8_t byte, frame_t* frame) 
{
	//depending on frame's state we activate different procedures

	switch (frame->state) 
	{

	// waiting for the start bite
	case STATE_WAIT: 
		{
		if (byte == START1_CHAR) 	//if it's a start bite, we set next the state
			{
				frame->state = STATE_GOT_START1;
			} 
		else 
			{ 						//if not, we clear the frame and return with error
				Parser_InitFrame(frame);
				return PARSER_ERROR;
			}
		}
		break;

	case STATE_GOT_START1: 
		{
		if (byte == START2_CHAR) 
			{
				frame->state = STATE_GOT_START2;
			} 
		else 
			{
				Parser_InitFrame(frame);
				return PARSER_ERROR;
			}
		}
		break;

	//after receiving start bites, we receive length bites
	case STATE_GOT_START2: 
		{
			if(frame->data_index == 0)
			{
				frame->length = byte;
				frame->data_index += 1;
			}
			else
			{
				frame->length += (uint16_t) (byte << 8);
				frame->state = STATE_GOT_PAYLOAD_LEN;
				frame->data_index = 0;
			}
		}
		break;

	//receiving message_id
	case STATE_GOT_PAYLOAD_LEN: 
		{
			if(frame->data_index == 0)
			{
				frame->message_id = byte;
				frame->data_index += 1;
			}
			else
			{
				frame->message_id += (uint16_t) (byte << 8);
				frame->state = STATE_GOT_MESSAGE_ID;
				frame->data_index = 0;
			}
		}
		break;
	case STATE_GOT_MESSAGE_ID: 
		{
			if (byte == SRC_DEVICE_ID) 
				{
					frame->state = STATE_GOT_SRC_DEVICE_ID;
				} 
			else 
				{
					Parser_InitFrame(frame);
					return PARSER_ERROR;
				}
		}

		break;
	case STATE_GOT_SRC_DEVICE_ID: 
		{
			if (byte == DST_DEVICE_ID) 
				{

					if(frame->length == 0) frame->state = STATE_GOT_PAYLOAD;
					else frame->state = STATE_GOT_DST_DEVICE_ID;
				} 
			else 
				{
					Parser_InitFrame(frame);
					return PARSER_ERROR;
				}

		}
		break;

	// receiving data
	case STATE_GOT_DST_DEVICE_ID: 
		{

			frame->data[frame->data_index++] = byte;
			if (frame->data_index == frame->length) 
			{
				frame->state = STATE_GOT_PAYLOAD;
				frame->data_index = 0;
			}
		       	else 
			{
		      // 		frame->data[frame->data_index++] = byte;
			// continue reading data
			}
		}
		break;

	//receiving as many bites of data as there were declared in length bite

	// after data, we check the checksum
	case STATE_GOT_PAYLOAD: 
		{
			//frame->state = STATE_WAIT;
			if(frame->data_index == 0)
			{
				frame->checksum = byte;
				frame->data_index += 1;
			}
			else
			{
				frame->checksum += (uint16_t) (byte << 8);
				frame->data_index = 0;
				if (frame->checksum == frame_GetCheckSum(frame)) 
				{
					frame->state = STATE_WAIT;
					return PARSER_COMPLETE;
				} 
				else 
				{
					Parser_InitFrame(frame);
					return PARSER_ERROR;
				}

			}
		}
		break;

	}

	return PARSER_INCOMPLETE;
}


ParserStatus Parser_ParseSimpleDistance(frame_t *frame, simple_distance_t *out_data) 
{
	if (frame->message_id != ID_DISTANCE_SIMPLE)
		return PARSER_ERROR;
	
	out_data->distance = (uint32_t) ((frame->data[0] << 24) | (frame->data[1] << 16) | (frame->data[2] << 8) | (frame->data[3])) ; 
	out_data->confidence = frame->data[4];
	return PARSER_COMPLETE;
}


ParserStatus Parser_ParseSpeedOfSound(frame_t *frame, uint32_t *out_data) 
{
	if (frame->message_id != ID_SPEED_OF_SOUND)
		return PARSER_ERROR;
	
	*out_data = (uint32_t) ((frame->data[0] << 24) | (frame->data[1] << 16) | (frame->data[2] << 8) | (frame->data[3])) ; 
	return PARSER_COMPLETE;
	
}


ParserStatus Parser_ParseDistance(frame_t *frame, distance_t *out_data) 
{
	if (frame->message_id != ID_DISTANCE)
		return PARSER_ERROR;
	
	out_data->distance = (uint32_t) ((frame->data[0] << 24) | (frame->data[1] << 16) | (frame->data[2] << 8) | (frame->data[3])) ; 
	out_data->confidence = (uint16_t) ((frame->data[4] << 8) | (frame->data[5]));
	out_data->transmit_duration = (uint16_t) ((frame->data[6] << 8) | (frame->data[7]));
	out_data->ping_number = (uint32_t) ((frame->data[8] << 24) | (frame->data[9] << 16) | (frame->data[10] << 8) | (frame->data[11])) ; 
	out_data->scan_start = (uint32_t) ((frame->data[12] << 24) | (frame->data[13] << 16) | (frame->data[14] << 8) | (frame->data[15])) ; 
	out_data->scan_length = (uint32_t) ((frame->data[16] << 24) | (frame->data[17] << 16) | (frame->data[18] << 8) | (frame->data[19])) ; 
	out_data->gain_setting = (uint32_t) ((frame->data[20] << 24) | (frame->data[21] << 16) | (frame->data[22] << 8) | (frame->data[23])) ; 
	
	return PARSER_COMPLETE;
}


ParserStatus Parser_ParseSimpleDistanceRequest(frame_t *frame) 
{
	return (frame->message_id == ID_DISTANCE_SIMPLE ? PARSER_COMPLETE : PARSER_ERROR);
}


ParserStatus Parser_ParseSpeedOfSoundRequest(frame_t *frame) 
{
	return (frame->message_id == ID_SPEED_OF_SOUND ? PARSER_COMPLETE : PARSER_ERROR);
}


ParserStatus Parser_ParseDistanceRequest(frame_t *frame) 
{
	return (frame->message_id == ID_DISTANCE ? PARSER_COMPLETE : PARSER_ERROR);
}


void Parser_CreateTxBuffer(frame_t *frame, uint8_t out_buff[], uint32_t* len_out) 
{

	*len_out = 10 + frame->length;

	uint16_t sum = frame_GetCheckSum(frame);

	out_buff[0] = START1_CHAR;
	out_buff[1] = START2_CHAR;
	out_buff[2] = (uint8_t) (frame->length);
	out_buff[3] = (uint8_t) (frame->length >> 8);
	out_buff[4] = (uint8_t) (frame->message_id);
	out_buff[5] = (uint8_t) (frame->message_id >> 8);
	out_buff[6] = SRC_DEVICE_ID;
	out_buff[7] = DST_DEVICE_ID;

	for (int i = 0; i < frame->length; i++) 
	{
		out_buff[8 + i] = frame->data[i];
	}

	out_buff[8 + frame->length] = (uint8_t) (sum);
	out_buff[9 + frame->length] = (uint8_t) (sum >> 8);
}

uint32_t Parser_CreateSimpleDistanceRequest(uint8_t out_buff[])
{
	frame_t frame;
	frame.message_id = ID_DISTANCE_SIMPLE;
	frame.length = 0;
	uint32_t tmp;
	Parser_CreateTxBuffer(&frame, out_buff, &tmp);
	return tmp;
}


uint32_t Parser_CreateSpeedOfSoundRequest(uint8_t out_buff[])
{
	frame_t frame;
	frame.message_id = ID_SPEED_OF_SOUND;
	frame.length = 0;
	uint32_t tmp;
	Parser_CreateTxBuffer(&frame, out_buff, &tmp);
	return tmp;
}


uint32_t Parser_CreateDistanceRequest(uint8_t out_buff[])
{
	frame_t frame;
	frame.message_id = ID_DISTANCE;
	frame.length = 0;
	uint32_t tmp;
	Parser_CreateTxBuffer(&frame, out_buff, &tmp);
	return tmp;
}


frame_t Parser_CreateSimpleDistanceFrame(simple_distance_t *sdistData) 
{
	volatile frame_t frame;
	frame.message_id = ID_DISTANCE_SIMPLE;
	frame.length = 5;
	
	uint8_t shift = 0;
	for (int i = 0; i < 4; i++)
	{
		frame.data[i] = (uint8_t) (sdistData->distance >> shift);
		shift += 8;
	}

	frame.data[4] = sdistData->confidence;
	
	frame.checksum = frame_GetCheckSum(&frame);
	return frame;
}

frame_t Parser_CreateSpeedOfSoundFrame(uint32_t *speedData) 
{
	volatile frame_t frame;
	frame.message_id = ID_SPEED_OF_SOUND;
	frame.length = 4;
	
	uint8_t shift = 0;
	for (int i = 0; i < 4; i++)
	{
		frame.data[i] = (uint8_t) (*speedData >> shift);
		shift += 8;
	}
	

	frame.checksum = frame_GetCheckSum(&frame);
	return frame;
}

frame_t Parser_CreateDistanceFrame(distance_t *distData) 
{
	volatile frame_t frame;
	frame.message_id = ID_DISTANCE;
	frame.length = 24;
	
	uint8_t shift = 0;
	for (int i = 0; i < 4; i++)
	{
		frame.data[i] = (uint8_t) (distData->distance >> shift);
		shift += 8;
	}

	frame.data[4] = (uint8_t) (distData->confidence);
	frame.data[5] = (uint8_t) (distData->confidence >> 8);

	frame.data[6] = (uint8_t) (distData->transmit_duration);
	frame.data[7] = (uint8_t) (distData->transmit_duration >> 8);

	shift = 0;
	for (int i = 8; i < 12; i++)
	{
		frame.data[i] = (uint8_t) (distData->ping_number >> shift);
		shift += 8;
	}
	
	shift = 0;
	for (int i = 12; i < 16; i++)
	{
		frame.data[i] = (uint8_t) (distData->scan_start >> shift);
		shift += 8;
	}

	shift = 0;
	for (int i = 16; i < 20; i++)
	{
		frame.data[i] = (uint8_t) (distData->scan_length >> shift);
		shift += 8;
	}

	shift = 0;
	for (int i = 20; i < 24; i++)
	{
		frame.data[i] = (uint8_t) (distData->gain_setting >> shift);
		shift += 8;
	}

	frame.checksum = frame_GetCheckSum(&frame);
	return frame;
}

