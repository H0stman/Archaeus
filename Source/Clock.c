#include "time.h"

static INT64 freq, current;
static LARGE_INTEGER	largeFreq, count;

void StartClock(void)
{
   QueryPerformanceFrequency(&largeFreq);
   freq = largeFreq.QuadPart;
   QueryPerformanceCounter(&count);
   current = count.QuadPart;
}

double DeltaTime(void)
{
   QueryPerformanceCounter(&count);
   return (((double)count.QuadPart - (double)current) / (double)freq);
}

void UpdateClock(void)
{
<<<<<<< HEAD
   QueryPerformanceCounter(&count);
   current = count.QuadPart;
=======
   current = GetCount();
   //dtacc += DeltaTime();
>>>>>>> parent of a1becee (Depth stencil.)
}