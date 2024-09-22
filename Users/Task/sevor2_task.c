#include "main.h"
#include "cmsis_os.h"

void StartSevor2Task(void const * argument)
{
    uint32_t time = xTaskGetTickCount();

    for(;;)
    {
        osDelayUntil(&time, 1);
    }
}