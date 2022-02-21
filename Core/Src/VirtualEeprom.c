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

#define NUMERO_ELEMENTI 63

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

union {
	struct {
		uint8_t byte1;
		uint8_t byte2;
	};
	uint16_t word;

} sedici_union;
union {
	struct {
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t byte4;
	};
	float word;

} trentadue_float_union;
union {
	struct {
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t byte4;
	};
	unsigned int word;

} trentadue_union;
union {
	struct {
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t byte4;
		uint8_t byte5;
		uint8_t byte6;
		uint8_t byte7;
		uint8_t byte8;
	};
	unsigned long word;

} sessantaquattro_union;

void FactoryDefaultEEPROM()    //rester the FACTORY DEFAULT values in the EEPROM
{

	//18 variabili di cui un float quindi +1

	/*
	 UI = one_turn_mandrel_steps_default;   //
	 EEPROM.put(0, UI);                     //number of mandrel encoder steps per revolution
	 F = screw_pitch_default;               //
	 EEPROM.put(2, F);                      //lead screw pitch
	 UI = one_turn_screw_steps_default;     //
	 EEPROM.put(6, UI);                     //number of lead screw steps (stepper motor steps per revolution)
	 UI = AccelerationDelay_default;        //
	 EEPROM.put(8, UI);                     //stepper motor acceleration delay
	 UI = DecelerationDelay_default;        //
	 EEPROM.put(10, UI);                    //stepper motor deceleration delay

	 //CARRIAGE AND CROSS SLIDE FEEDS FOR EVERY NORTON GEARBOX RATIO (4 RATIOS IN TOTAL)
	 UI = av_carro_1_default;
	 EEPROM.put(12, UI);
	 UI = av_trasv_1_default;
	 EEPROM.put(14, UI);
	 UI = av_carro_2_default;
	 EEPROM.put(16, UI);
	 UI = av_trasv_2_default;
	 EEPROM.put(18, UI);
	 UI = av_carro_3_default;
	 EEPROM.put(20, UI);
	 UI = av_trasv_3_default;
	 EEPROM.put(22, UI);
	 UI = av_carro_4_default;
	 EEPROM.put(24, UI);
	 UI = av_trasv_4_default;
	 EEPROM.put(26, UI);

	 BOO = CW_default;         //
	 EEPROM.put(28, BOO);      //
	 BOO = CCW_default;        //STEPPER MOTOR DIRECTION
	 EEPROM.put(29, BOO);      //

	 UI = offset_filetto_default;          //
	 EEPROM.put(30, UI);                   //offset for thread start
	 B = norton_default;                   //
	 EEPROM.put(32,B);                     //NORTON GEARBOX POSITION
	 UL = MaxStepperSpeed_default;         //
	 EEPROM.put(33,UL);                    //Max stepper speed
	 */
}

void WriteToEEPROM() //Writes the current value to the EEPROM if they've been modified by the user
//Factory default setting can be reset with the FactoryDefaultEEPROM function
{
	uint8_t bytes_to_store[NUMERO_ELEMENTI];

	sedici_union.word = one_turn_mandrel_steps;
	bytes_to_store[0] = sedici_union.byte1;
	bytes_to_store[1] = sedici_union.byte2;

	trentadue_float_union.word = screw_pitch;

	bytes_to_store[2] = trentadue_float_union.byte1;
	bytes_to_store[3] = trentadue_float_union.byte2;
	bytes_to_store[4] = trentadue_float_union.byte3;
	bytes_to_store[5] = trentadue_float_union.byte4;

	trentadue_union.word = one_turn_screw_steps;
	bytes_to_store[6] = trentadue_union.byte1;
	bytes_to_store[7] = trentadue_union.byte2;
	bytes_to_store[8] = trentadue_union.byte3;
	bytes_to_store[9] = trentadue_union.byte4;
	//--
	trentadue_union.word = AccelerationDelay;
	bytes_to_store[10] = trentadue_union.byte1;
	bytes_to_store[11] = trentadue_union.byte2;
	bytes_to_store[12] = trentadue_union.byte3;
	bytes_to_store[13] = trentadue_union.byte4;

	trentadue_union.word = DecelerationDelay;
	bytes_to_store[14] = trentadue_union.byte1;
	bytes_to_store[15] = trentadue_union.byte2;
	bytes_to_store[16] = trentadue_union.byte3;
	bytes_to_store[17] = trentadue_union.byte4;

	trentadue_union.word = av_carro[0];
	bytes_to_store[18] = trentadue_union.byte1;
	bytes_to_store[19] = trentadue_union.byte2;
	bytes_to_store[20] = trentadue_union.byte3;
	bytes_to_store[21] = trentadue_union.byte4;
	trentadue_union.word = av_carro[1];
	bytes_to_store[22] = trentadue_union.byte1;
	bytes_to_store[23] = trentadue_union.byte2;
	bytes_to_store[24] = trentadue_union.byte3;
	bytes_to_store[25] = trentadue_union.byte4;
	trentadue_union.word = av_carro[2];
	bytes_to_store[26] = trentadue_union.byte1;
	bytes_to_store[27] = trentadue_union.byte2;
	bytes_to_store[28] = trentadue_union.byte3;
	bytes_to_store[29] = trentadue_union.byte4;
	trentadue_union.word = av_carro[3];
	bytes_to_store[30] = trentadue_union.byte1;
	bytes_to_store[31] = trentadue_union.byte2;
	bytes_to_store[32] = trentadue_union.byte3;
	bytes_to_store[33] = trentadue_union.byte4;
	trentadue_union.word = av_trasv[0];
	bytes_to_store[34] = trentadue_union.byte1;
	bytes_to_store[35] = trentadue_union.byte2;
	bytes_to_store[36] = trentadue_union.byte3;
	bytes_to_store[37] = trentadue_union.byte4;
	trentadue_union.word = av_trasv[1];
	bytes_to_store[38] = trentadue_union.byte1;
	bytes_to_store[39] = trentadue_union.byte2;
	bytes_to_store[40] = trentadue_union.byte3;
	bytes_to_store[41] = trentadue_union.byte4;
	trentadue_union.word = av_trasv[2];
	bytes_to_store[42] = trentadue_union.byte1;
	bytes_to_store[43] = trentadue_union.byte2;
	bytes_to_store[44] = trentadue_union.byte3;
	bytes_to_store[45] = trentadue_union.byte4;
	trentadue_union.word = av_trasv[3];
	bytes_to_store[46] = trentadue_union.byte1;
	bytes_to_store[47] = trentadue_union.byte2;
	bytes_to_store[48] = trentadue_union.byte3;
	bytes_to_store[49] = trentadue_union.byte4;
	bytes_to_store[50] = CW == true ? 1 : 0;
	bytes_to_store[51] = CCW == true ? 1 : 0;
	sedici_union.word = thread_offset_steps;
	bytes_to_store[52] = sedici_union.byte1;
	bytes_to_store[53] = sedici_union.byte2;
	bytes_to_store[54] = NORTON_gearbox;

	sessantaquattro_union.word = MaxStepperSpeed;
	bytes_to_store[55] = sessantaquattro_union.byte1;
	bytes_to_store[56] = sessantaquattro_union.byte2;
	bytes_to_store[57] = sessantaquattro_union.byte3;
	bytes_to_store[58] = sessantaquattro_union.byte4;
	bytes_to_store[59] = sessantaquattro_union.byte5;
	bytes_to_store[60] = sessantaquattro_union.byte6;
	bytes_to_store[61] = sessantaquattro_union.byte7;
	bytes_to_store[62] = sessantaquattro_union.byte8;

	ee_writeToRam(0, NUMERO_ELEMENTI, bytes_to_store);
	ee_commit();

	/*


	 EEPROM.put(30, thread_offset_steps);    //offset for thread start
	 EEPROM.put(32,NORTON_gearbox);          //NORTON GEARBOX POSITION
	 EEPROM.put(33,MaxStepperSpeed);         //Max stepper speed limit

	 */
}

void LoadFromEEPROM() //Loads the parameters from the EEPROM and put them in the proper variables
{
	uint8_t bytes_to_store[NUMERO_ELEMENTI];
	ee_read(0, NUMERO_ELEMENTI, bytes_to_store);
	one_turn_mandrel_steps = value_due_int(0,bytes_to_store);
	screw_pitch = value_quattro_float(2, bytes_to_store);
	one_turn_screw_steps = value_quattro_int(6, bytes_to_store);
	AccelerationDelay= value_quattro_int(10, bytes_to_store);
	DecelerationDelay= value_quattro_int(14, bytes_to_store);
	av_carro[0]= value_quattro_int(18, bytes_to_store);
	av_carro[1]= value_quattro_int(22, bytes_to_store);
	av_carro[2]= value_quattro_int(26, bytes_to_store);
	av_carro[3]= value_quattro_int(30, bytes_to_store);
	av_trasv[0]= value_quattro_int(34, bytes_to_store);
	av_trasv[1]= value_quattro_int(38, bytes_to_store);
	av_trasv[2]= value_quattro_int(42, bytes_to_store);
	av_trasv[3]= value_quattro_int(46, bytes_to_store);
	CW = bytes_to_store[50] == 1 ? true : false;
	CCW = bytes_to_store[51] == 1 ? true : false;
	NORTON_gearbox = value_due_int(52, bytes_to_store);
	MaxStepperSpeed = value_otto_int(54, bytes_to_store);
	single_step_pitch = screw_pitch/one_turn_screw_steps;   //calculates the linear movement (in mm) of the carriage for a single step of the stepper motor

}

unsigned int value_quattro_int(int begin, uint8_t elements[]) {
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
unsigned int value_otto_int(int begin, uint8_t elements[]) {
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
int value_due_int(int begin, uint8_t elements[]) {
	sedici_union.byte1 = elements[begin];
	sedici_union.byte2 = elements[begin + 1];
	return sedici_union.word;
}
