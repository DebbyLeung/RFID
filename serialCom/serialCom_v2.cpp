#pragma once
#include "serialCom_v2.h"

#define DEBUG

bool SerialCom::openSerialPort(const char com_id[], int baud_rate) {
    hCom = CreateFile(LPCSTR(com_id),  // purple VS
        GENERIC_READ | GENERIC_WRITE,  // open mode: read & write  
        0,
        NULL,
        CREATE_ALWAYS,
        0,
        NULL);
    if (hCom == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    else fprintf(stderr, "OK\n");



    GetCommState(hCom, &dcb);
    dcb.BaudRate = baud_rate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    if (!SetCommState(hCom, &dcb))
    {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hCom);
        return 1;
    }
    //timeout setting
    TimeOuts.ReadIntervalTimeout = 1000;
    TimeOuts.ReadTotalTimeoutMultiplier = 0;
    TimeOuts.ReadTotalTimeoutConstant = 0;
    TimeOuts.WriteTotalTimeoutMultiplier = 0;
    TimeOuts.WriteTotalTimeoutConstant = 500;
    if (!SetCommTimeouts(hCom, &TimeOuts))
    {
        fprintf(stderr, "Error setting timeout\n");
        CloseHandle(hCom);
        return 1;
    }

    lock = false;
    return 0;
}
void SerialCom::calculateCRC16(unsigned char* buf, int len, uint8_t* CRC_L, uint8_t* CRC_H) {
    unsigned int crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (unsigned int)buf[pos];          //XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {         // Loop over each bit
            if ((crc & 0x0001) != 0) {         // If the LSB is set
                crc >>= 1;                     // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else                              //  Else LSB is not set
                crc >>= 1;                    //  Just shift right
        }
    }
    crc = (((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8));  //Combined return
    *CRC_L = ((crc & 0x00ff));
    *CRC_H = ((crc & 0xff00) >> 8);
}

void SerialCom::calculateCheckSum(unsigned char* buf, int len) {

    BYTE checksum =0;
    for (int i = 0; i < len; i++)
    {
        checksum += *(buf+i);
    }
    checksum = ~checksum + 1;//recipocal +1
    *(buf + len-1) = checksum;

}
bool SerialCom::sendByte(BYTE* d_arr, int len) {
    if (lock) {
        return false;  //do not open send anything before finish
    }
    lock = 1; //  lock the com 
    COMSTAT ComStat;
    DWORD dwErrorFlags;
    ClearCommError(hCom, &dwErrorFlags, &ComStat);

    //d_arr ptr of the 1st digit write until the 8th digit,
    DWORD dwBytesToWrite = len;
    DWORD dwBytesWrite = 0;
    int dwRel;
#ifdef DEBUG 
    printf("Serial Transmitted: ");
    for (int i = 0; i < len; i++) {
        printf("%02x ", *(d_arr + i));
    }
    printf("\n");
#endif
    dwRel = WriteFile(hCom, d_arr, len, &dwBytesWrite, NULL);

    if (!dwRel) {
        lock = 0;
        return false;
    }
}
    
std::vector<BYTE> SerialCom::readByte(BYTE * d_arr, int len) {
    if (lock) {
        return {};  //do not open send anything before finish
    }
    lock = 1; //  lock the com 
    DWORD dwBytesToRead = len;
    DWORD dwBytesRead = 0;
    int dwRel;
    dwRel = ReadFile(hCom, d_arr, dwBytesToRead, &dwBytesRead, NULL);
    
    std::vector<BYTE> vec;
    //first 4 byte
    for (int i = 0; i < 5; i++) {
        vec.push_back(*(d_arr + i));
    }
    for (int i = 5; i < *(d_arr + 4); i++) {
        vec.push_back(*(d_arr + i));
    }

    if (!dwRel) {
        lock = 0;
        return { 0,0,0,0,0,0,0,0 };
    }
#ifdef DEBUG 
    printf("Serial Received: ");
    for (int i = 0; i < 8; i++) {
        //print only when byte exist
        if(d_arr[i]!= 0xcc){printf("%02x ", d_arr[i]);}
        

    }
    printf("\n");
#endif
    lock = 0;
    return vec;
}
bool SerialCom::closeSerialPort() {

    bool closed = CloseHandle(hCom);
    std::cout << "Close Port Success. GoodBye!" << std::endl;
    return closed;

}
