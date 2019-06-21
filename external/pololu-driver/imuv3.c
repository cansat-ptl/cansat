/*
 * imuv3.c
 *
 * Created: 11.06.2019 22:43:33
 *  Author: bear1ake
 */ 

#include "imuv3.h"
#include "../../kernel/globals.h"

volatile struct LSM_t LSM;
volatile struct L3GD_t L3GD;


void reg_write(unsigned char dev, unsigned char adr, unsigned char data)
{
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(dev & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(adr);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(data);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	twi_stop();
}

void lsm_init()
{
	reg_write(LSM_adr, 0x1F, 0x00);
	reg_write(LSM_adr, 0x20, 0x87);
	reg_write(LSM_adr, 0x21, 0x20);
	reg_write(LSM_adr, 0x22, 0x00);
	reg_write(LSM_adr, 0x23, 0x00);
	reg_write(LSM_adr, 0x24, 0x64);
	reg_write(LSM_adr, 0x25, 0x60);
	reg_write(LSM_adr, 0x26, 0x00);
}

void l3gd_init()
{
	reg_write(L3GD_adr, 0x39, 0x00);
	reg_write(L3GD_adr, 0x20, 0x6F);
	reg_write(L3GD_adr, 0x21, 0x00);
	reg_write(L3GD_adr, 0x22, 0x00);
	reg_write(L3GD_adr, 0x23, 0x30);
	reg_write(L3GD_adr, 0x24, 0x00);
}

void l3gd_read()
{
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(L3GD_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(L3GD_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(L3GD_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.XL = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	L3GD.XH = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.YL = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.YH = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.ZL = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.ZH = twi_read_nack();
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00;
		return;
	};
	twi_stop();
}

void lsm_a_read()
{
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(LSM_adr | 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(LSM_A_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(LSM_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XL_A = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XH_A = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YL_A = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YH_A = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZL_A = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZH_A = twi_read_nack();
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00;
		return;
	}
	twi_stop();
}

void lsm_m_read()
{
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(LSM_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(LSM_M_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00;
		return;
	}
	twi_write(LSM_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XL_M = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XH_M = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YL_M = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YL_M = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZL_M = twi_read_ack();
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZH_M = twi_read_nack();
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00;
		return;
	}
	twi_stop();
}

int lsm_ack()
{
	if((TWSR & 0xF8) == 0x00)
	return 0;
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return 0;
	}
	twi_write(LSM_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return 0;
	}
	twi_stop();
	return 1;
}

int l3gd_ack()
{
	if((TWSR & 0xF8) == 0x00)
	return 0;
	twi_start(0x3D);
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return 0;
	}
	twi_write(L3GD_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return 0;
	}
	twi_stop();
	return 1;
}

void imu_init()
{
	if(lsm_ack())
	lsm_init();
	if(l3gd_ack())
	l3gd_init();
}


void imu_read()
{
	if(lsm_ack())
	{
		lsm_a_read();
		lsm_m_read();
	}
	if(l3gd_ack())
	l3gd_read();
}
