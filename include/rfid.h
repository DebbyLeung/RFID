#ifndef MODBUS
#define MODBUS

#include "serialCom_v2.h"

class Rfid: public SerialCom {
public:
    /*
     * function: track_tags_once
     *     we send the command once to read the device. If the command is sent, the device will stop after the label is saved.
     * input:
     *     arr [std::vector<BYTE>]: the com id of the serial port to control the motor
     * output:
     *     val [int]: return value
     */
	int track_tags_once(std::vector<BYTE> arr);
    /*
     * function: get_str
     * input:
     *     mode [char]: 
     * output:
     *     val [string]: returm value
     */
    std::string get_str(char mode);






	int buffer = 128;
    std::vector <BYTE> track_tags_once_tx = { 0x52, 0x46, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00 };//ASCII 'R','F', header, 0x22
    std::vector<BYTE> write_tag = { 0x52, 0x46, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00};
    std::vector<BYTE> write_tag_once = { 0x52, 0x46, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x26, 0x03, 0x01, 0x09, 0xc4, 0x00 };//for one scenario

};



#endif