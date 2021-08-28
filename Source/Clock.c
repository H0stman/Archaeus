#include "Clock.h"

static INT64 freq, current;
static LARGE_INTEGER	largeFreq, count;
UINT dtacc;

void StartClock(void)
{
   QueryPerformanceFrequency(&largeFreq);
   freq = largeFreq.QuadPart;
   current = GetCount();
}

double DeltaTime(void)
{
   return (((double)GetCount() - (double)current) / (double)freq);
}

INT64 GetCount(void)
{
   QueryPerformanceCounter(&count);
   return count.QuadPart;
}

void UpdateClock(void)
{
   current = GetCount();
}