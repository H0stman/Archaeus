#include <error.h>

void HandleHR(HRESULT errorcode)
{
	if (FAILED(errorcode))
	{
		LPWSTR str = NULL;
		if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), str, 0, NULL))
		{
			OutputDebugStringW(str);
			LocalFree(str);
		}
	}
}