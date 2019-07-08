/*
 * imuv3.c
 *
 * Created: 11.06.2019 22:43:33
 *  Author: bear1ake
 */ 
// L3GD20H & LSM303D
// minIMU v3

#ifndef _IMUV3_H
#define _IMUV3_H

//#include "../../drivers/twi.h"

#define LSM_adr		0x3B
#define LSM_A_xyz	0x28
#define LSM_M_xyz	0x08
#define L3GD_adr	0xD7
#define L3GD_xyz	0x28

void lsm_init();
void l3gd_init();
void l3gd_read();
void lsm_a_read();
void lsm_m_read();
void imu_init();
void imu_read();

#endif