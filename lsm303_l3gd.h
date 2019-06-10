// L3GD20H & LSM303D
// minIMU v3

#include "w2.h"

#define LSM_adr		0x3D
#define LSM_A_xyz	0x28
#define LSM_M_xyz	0x08
#define L3GD_adr	0xD5
#define L3GD_xyz	0x28

struct LSM
{
	unsigned char XH_A;
	unsigned char XL_A;
	unsigned char YH_A;
	unsigned char YL_A;
	unsigned char ZH_A;
	unsigned char ZL_A;	
	unsigned char XH_M;
	unsigned char XL_M;
	unsigned char YH_M;
	unsigned char YL_M;
	unsigned char ZH_M;
	unsigned char ZL_M;
}LSM;

struct L3GD
{
	unsigned char XH;
	unsigned char XL;
	unsigned char YH;
	unsigned char YL;
	unsigned char ZH;
	unsigned char ZL;
}L3GD;

void reg_write(unsigned char dev, unsigned char adr, unsigned char data)
{
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(dev & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(adr);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(data);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
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
		TWDR = 0x00;
		return;
	}
	w2_write(L3GD_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(L3GD_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	w2_start;
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(L3GD_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.XL = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	L3GD.XH = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.YL = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.YH = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.ZL = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	};
	L3GD.ZH = w2_read(0);
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00;
		return;
	};
	w2_stop;
}

void lsm_a_read()
{
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(LSM_adr | 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(LSM_A_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	w2_start;
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(LSM_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XL_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XH_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YL_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YH_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZL_A = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZH_A = w2_read(0);
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00;
		return;
	}
	w2_stop;
}

void lsm_m_read()
{
	w2_start;
	if((TWSR & 0xF8) != 0x08)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(LSM_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(LSM_M_xyz | 0x80);
	if((TWSR & 0xF8) != 0x28)
	{
		TWDR = 0x00;
		return;
	}
	w2_start;
	if((TWSR & 0xF8) != 0x10)
	{
		TWDR = 0x00;
		return;
	}
	w2_write(LSM_adr | 0x01);
	if((TWSR & 0xF8) != 0x40)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.XH_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.YL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZL_M = w2_read(1);
	if((TWSR & 0xF8) != 0x50)
	{
		TWDR = 0x00;
		return;
	}
	LSM.ZH_M = w2_read(0);
	if((TWSR & 0xF8) != 0x58)
	{
		TWDR = 0x00;
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
		TWDR = 0x00;
		return 0;
	}
	w2_write(LSM_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
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
		TWDR = 0x00;
		return 0;
	}
	w2_write(L3GD_adr & 0xFE);
	if((TWSR & 0xF8) != 0x18)
	{
		TWDR = 0x00;
		return 0;
	}
	w2_stop;
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
