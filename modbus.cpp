#include "modbus.h"

BYTE ModbusRTU::read_torque() {
	
    calculateCRC16(_read_tq_tx, 6, &_read_tq_tx[7], &_read_tq_tx[6]);
    //_read_tq_tx;
    BYTE* d_arr = _read_tq_tx;
    this->sendByte(d_arr, 8);
    int8_t val = this->rv_arr[3] << 4 | this->rv_arr[4];
    printf("Torque: %d\n", val);
	return val;

}

void ModbusRTU::set_zero() {
    calculateCRC16(_set_zero, 6, &_set_zero[7], &_set_zero[6]);
    BYTE* d_arr = _set_zero;
    this->sendByte(d_arr, 8);

}