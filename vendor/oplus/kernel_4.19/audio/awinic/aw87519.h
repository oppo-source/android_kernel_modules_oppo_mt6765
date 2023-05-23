#ifndef __AW87519_H__
#define __AW87519_H__


#define AW87519_REG_CHIPID		0x00
#define AW87519_REG_SYSCTRL		0x01
#define AW87519_REG_BATSAFE		0x02
#define AW87519_REG_BSTOVR		0x03
#define AW87519_REG_BSTVPR		0x04
#define AW87519_REG_PAGR		0x05
#define AW87519_REG_PAGC3OPR		0x06
#define AW87519_REG_PAGC3PR		0x07
#define AW87519_REG_PAGC2OPR		0x08
#define AW87519_REG_PAGC2PR		0x09
#define AW87519_REG_PAGC1PR		0x0A

#define AW87519_CHIPID			0x59

/********************************************
 * Register Access
 *******************************************/
#define AW87519_REG_MAX			11

#define AW87519_REG_NONE_ACCESS		0
#define AW87519_REG_RD_ACCESS		(1 << 0)
#define AW87519_REG_WR_ACCESS		(1 << 1)

const unsigned char aw87519_reg_access[AW87519_REG_MAX] = {
	[AW87519_REG_CHIPID]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_SYSCTRL]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_BATSAFE]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_BSTOVR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_BSTVPR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_PAGR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_PAGC3OPR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_PAGC3PR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_PAGC2OPR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_PAGC2PR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,
	[AW87519_REG_PAGC1PR]	= AW87519_REG_RD_ACCESS | AW87519_REG_WR_ACCESS,

};

#endif
