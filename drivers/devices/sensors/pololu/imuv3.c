/*
 * imuv3.c
 *
 * Created: 11.06.2019 22:43:33
 *  Author: bear1ake
 */ 

#include "imuv3.h"
#include <stdio.h>
#include "../../../interfaces/uart.h"
#include "../../../../kernel/types.h"
#include "w2.h"

volatile struct LSM_t LSM;
volatile struct L3GD_t L3GD;

void reg_write(unsigned char dev, unsigned char adr, unsigned char data)
{
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(dev & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(adr);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(data);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_stop;
}

void lsm_init()
{
	reg_write(LSM_adr, 0x1F, 0x00);
	reg_write(LSM_adr, 0x20, 0b10000111);
	reg_write(LSM_adr, 0x21, 0b00100000);
	reg_write(LSM_adr, 0x22, 0x00);
	reg_write(LSM_adr, 0x23, 0x00);
	reg_write(LSM_adr, 0x24, 0b01100100);
	reg_write(LSM_adr, 0x25, 0b01100000);
	reg_write(LSM_adr, 0x26, 0x00);
}

void l3gd_init()
{
	reg_write(L3GD_adr, 0x39, 0x00);
	reg_write(L3GD_adr, 0x20, 0b01101111);
	reg_write(L3GD_adr, 0x21, 0x00);
	reg_write(L3GD_adr, 0x22, 0x00);
	reg_write(L3GD_adr, 0x23, 0b00110000);
	reg_write(L3GD_adr, 0x24, 0x00);
}

void l3gd_read()
{
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(L3GD_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(L3GD_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_start;
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(L3GD_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00; w2_stop;
		return;
	};
	L3GD.XL = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	L3GD.XH = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	};
	L3GD.YL = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	};
	L3GD.YH = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	};
	L3GD.ZL = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	};
	L3GD.ZH = w2_read(0);
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00; w2_stop;
		return;
	};
	w2_stop;
}

void lsm_a_read()
{
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(LSM_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(LSM_A_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_start;
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(LSM_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.XL_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.XH_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.YL_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.YH_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.ZL_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.ZH_A = w2_read(0);
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_stop;
}

void lsm_m_read()
{
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(LSM_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(LSM_M_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_start;
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_write(LSM_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.XL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.XH_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.YL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.YL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.ZL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	LSM.ZH_M = w2_read(0);
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00; w2_stop;
		return;
	}
	w2_stop;
}

int lsm_ack()
{
	if((TWSR & 0xF8) == 0x00)
	return 0;
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00; w2_stop;
		return 0;
	}
	w2_write(LSM_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00; w2_stop;
		return 0;
	}
	w2_stop;
	return 1;
}

int l3gd_ack()
{
	if((TWSR & 0xF8) == 0x00)
	return 0;
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00; w2_stop;
		return 0;
	}
	w2_write(L3GD_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00; w2_stop;
		return 0;
	}
	w2_stop;
	return 1;
}

void imu_init()
{
	lsm_init();
	l3gd_init();
}

void imu_read()
{
	lsm_a_read();
	lsm_m_read();
	l3gd_read();
}
