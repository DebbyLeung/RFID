#ifndef SERIAL
#define SERIAL

#include <stdio.h>
#include <iostream>
#include <vector>
#include "windows.h"
class SerialCom {
public:
    HANDLE hCom;
    COMMTIMEOUTS TimeOuts;
    DCB dcb;
    BYTE rv_arr[8];


    
    bool lock = true; // Wait until the transfer is complete before making the next transfer

    /*
     * function: open_serial_port
     *     open the serial port to control the motor
     * input:
     *     com_id[const char array]: the com id of the serial port to control the motor
     * output:
     *     state[bool]: success return true
     */
    bool openSerialPort(const char com_id[], int baud_rate);


    /*
     *  function: Calculate CRC16 modbus and returns Low bytes and high bytes
     *  input:
     *      buf[unsigned char*]: data packet to be calculated
     *      len[int]: number of bytes before calculation (usually total # - 2 )
     *      CRC_L[uint8_t*]: low bytes of CRC16 modbus
     *      CRC_H[uint8_t*]: high bytes of CRC16 modbus
     *  output:
     *      state[bool]: success return true
    */
    void calculateCRC16(unsigned char* buf, int len, uint8_t* CRC_L, uint8_t* CRC_H);
    void calculateCheckSum(unsigned char* buf, int len);
    /*
    *  function: transmit ptr of the 1st digit to the 8th digit,
    *  input:
    *      d_arr[unsigned char*]: data packet to be calculated
    *      len[int]: number of bytes before calculation (usually total # - 2 )

    *  output:
    *      state[bool]: success return true
   */
    bool sendByte(BYTE* d_arr, int len);
    /*
     *  function: rv_arr ptr of the 1st digit read until the 8th digit,
     *  input:
     *      d_arr[unsigned char*]: data packet to be calculated
     *      len[int]: number of bytes before calculation (usually total # - 2 )

     *  output:
     *      state[bool]: success return true
    */
    std::vector<BYTE> readByte(BYTE* d_arr, int len);
    bool closeSerialPort();
};

#endif