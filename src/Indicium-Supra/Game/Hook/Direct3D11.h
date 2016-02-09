#pragma once
#include "Direct3DBase.h"

namespace Direct3D11Hooking
{
	class Direct3D11 :
		public Direct3DHooking::Direct3DBase
	{
	public:
		Direct3D11();
		~Direct3D11();

		bool GetDeviceVTable(UINTX* pVTable) const override;
	};
}
