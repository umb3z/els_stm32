/*
 * VirtualEeprom.c
 *
 *  Created on: Feb 21, 2022
 *      Author: umbez
 */

/* EEPROM factory defaults and parameters to be stored
 *
 * PARAMETER               |TYPE          |SIZE  |STARTING LOCATION |FACTORY DEFAULT
 * one_turn_mandrel_steps  |unsigned int  |2     |0                 |1000
 * screw_pitch             |float         |4     |2                 |3.175
 * one_turn_screw_steps    |unsigned int  |2     |6                 |400
 * AccelerationDelay       |unsigned int  |2     |8                 |500
 * DecelerationDelay       |unsigned int  |2     |10                |500
 * av_carro_1              |unsigned int  |2     |12                |50
 * av_trasv_1              |unsigned int  |2     |14                |7
 * av_carro_2              |unsigned int  |2     |16                |30
 * av_trasv_2              |unsigned int  |2     |18                |5
 * av_carro_3              |unsigned int  |2     |20                |20
 * av_trasv_3              |unsigned int  |2     |22                |3
 * av_carro_4              |unsigned int  |2     |24                |10
 * av_trasv_4              |unsigned int  |2     |26                |2
 * CW                      |boolean       |1     |28                |HIGH
 * CCW                     |boolean       |1     |29                |LOW
 * offset_filetto          |unsigned int  |2     |30                |400
 * NORTON                  |byte          |1     |32                |1
 * MaxStepperSpeed         |unsigned long |4     |33                |500
 * FREE                    |              |      |37                |
 */

#include "VirtualEeprom.h"

#define NUMERO_ELEMENTI 17

extern int one_turn_mandrel_steps;
extern float screw_pitch;
extern unsigned int one_turn_screw_steps;
extern unsigned int AccelerationDelay;
extern unsigned int DecelerationDelay;
extern bool CW;
extern bool CCW;
extern unsigned int av_carro[];
extern unsigned int av_trasv[];
extern int thread_offset_steps;
extern unsigned char NORTON_gearbox;
extern unsigned long MaxStepperSpeed;
extern float single_step_pitch;


void WriteToEEPROM() //Writes the current value to the EEPROM if they've been modified by the user
//Factory default setting can be reset with the FactoryDefaultEEPROM function
{
/*	uint32_t bytes_to_store[NUMERO_ELEMENTI];

	bytes_to_store[0] = one_turn_mandrel_steps ;
	bytes_to_store[1] = one_turn_screw_steps;
	bytes_to_store[2] = AccelerationDelay;
	bytes_to_store[3] = DecelerationDelay;
	bytes_to_store[4] = av_carro[0];
	bytes_to_store[5] = av_carro[1];
	bytes_to_store[6] = av_carro[2];
	bytes_to_store[7] = av_carro[3];
	bytes_to_store[8] = av_trasv[0];
	bytes_to_store[9] = av_trasv[1];
	bytes_to_store[10] = av_trasv[2];
	bytes_to_store[11] = av_trasv[3];
	bytes_to_store[12] = CW == true ? 1 : 0;
	bytes_to_store[13] =CCW == true ? 1 : 0;
	bytes_to_store[15] = NORTON_gearbox;
	bytes_to_store[16] = thread_offset_steps;

	 Flash_Write_Data(0x08004410 , (uint32_t *)data2, NUMERO_ELEMENTI);

	trentadue_float_union.word = screw_pitch;
	sessantaquattro_union.word = MaxStepperSpeed;
*/


}

void LoadFromEEPROM() //Loads the parameters from the EEPROM and put them in the proper variables
{
	/*
	uint8_t bytes_to_store[NUMERO_ELEMENTI];
	ee_read(0, NUMERO_ELEMENTI, bytes_to_store);
	one_turn_mandrel_steps = value_quattro_int(0, bytes_to_store);
	screw_pitch = value_quattro_float(2, bytes_to_store);
	one_turn_screw_steps = value_quattro_uint(6, bytes_to_store);
	AccelerationDelay = value_quattro_uint(10, bytes_to_store);
	DecelerationDelay = value_quattro_uint(14, bytes_to_store);
	av_carro[0] = value_quattro_uint(18, bytes_to_store);
	av_carro[1] = value_quattro_uint(22, bytes_to_store);
	av_carro[2] = value_quattro_uint(26, bytes_to_store);
	av_carro[3] = value_quattro_uint(30, bytes_to_store);
	av_trasv[0] = value_quattro_uint(34, bytes_to_store);
	av_trasv[1] = value_quattro_uint(38, bytes_to_store);
	av_trasv[2] = value_quattro_uint(42, bytes_to_store);
	av_trasv[3] = value_quattro_uint(46, bytes_to_store);
	CW = bytes_to_store[50] == 1 ? true : false;
	CCW = bytes_to_store[51] == 1 ? true : false;
	NORTON_gearbox = (unsigned char)bytes_to_store[52];
	MaxStepperSpeed = value_otto_ulong(54, bytes_to_store);
	single_step_pitch = screw_pitch / (float)one_turn_screw_steps; //calculates the linear movement (in mm) of the carriage for a single step of the stepper motor
*/
}
/*

unsigned int value_quattro_uint(int begin, uint8_t elements[]) {
	trentadue_union.byte1 = elements[begin];
	trentadue_union.byte2 = elements[begin + 1];
	trentadue_union.byte3 = elements[begin + 2];
	trentadue_union.byte4 = elements[begin + 3];
	return trentadue_union.word;
}
float value_quattro_float(int begin, uint8_t elements[]) {
	trentadue_float_union.byte1 = elements[begin];
	trentadue_float_union.byte2 = elements[begin + 1];
	trentadue_float_union.byte3 = elements[begin + 2];
	trentadue_float_union.byte4 = elements[begin + 3];
	return trentadue_float_union.word;
}
unsigned int value_otto_ulong(int begin, uint8_t elements[]) {
	sessantaquattro_union.byte1 = elements[begin];
	sessantaquattro_union.byte2 = elements[begin + 1];
	sessantaquattro_union.byte3 = elements[begin + 2];
	sessantaquattro_union.byte4 = elements[begin + 3];
	sessantaquattro_union.byte5 = elements[begin + 4];
	sessantaquattro_union.byte6 = elements[begin + 5];
	sessantaquattro_union.byte7 = elements[begin + 6];
	sessantaquattro_union.byte8 = elements[begin + 7];
	return sessantaquattro_union.word;
}
int value_quattro_int(int begin, uint8_t elements[]) {
	trentadue_int_union.byte1 = elements[begin];
	trentadue_int_union.byte2 = elements[begin + 1];
	trentadue_int_union.byte1 = elements[begin + 2];
	trentadue_int_union.byte2 = elements[begin + 3];
	return trentadue_int_union.word;
}
*/
