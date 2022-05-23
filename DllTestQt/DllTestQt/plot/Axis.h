#ifndef AXIS_H
#define AXIS_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
    www.instrustar.com
*********************************************************************************************************************/

#include <string>
#include <vector>

class Axis
{
public:
	static const int MAX_STRING_LEN = 128;
	//---------------------------------------------------AxisInterval--------------------------------------------------
	static const int AxisIntervalCenter = 30;
	static const int AxisIntervalCount = 84;
	static double AxisInterval[AxisIntervalCount];

	static int CalLargerAxisIntervalIndex(double intv)
	{
		int k = 0;
		for (int i = 0; i < AxisIntervalCount; i++)
		{
			if (intv >= AxisInterval[i])
				k = i;
		}
		return k;
	}


	//---------------------------------------------------Y_AXIS_DISP_DANWEI--------------------------------------------------
	enum class Y_AXIS_DANWEI {
		NO = 0x00000001,
		VOLTAGE = 0x00000002,
		DB = 0x10000004,

		RAD = 0x00000008,
		DEG = 0x00000010,

		DB_V = 0x10000020,
		DB_MV = 0x10000040,
		DB_M = 0x10000080,
		DB_U = 0x10000100
	};

	//---------------------------------------------X_AXIS_DISP_DANWEI---------------------------------------------
	enum class X_AXIS_DANWEI {
		NO = 0x00000001,
		S = 0x00000002,
		FREQ = 0x00000004,
		VOLTAGE = 0x00000010,
	};
};

#endif
