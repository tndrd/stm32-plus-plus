#ifndef _FLASH_H
#define _FLASH_H

#if defined(STM32F37X)
    #include "flash37x.h"
#elif defined(STM32F4)
    #include "flash4xx.h"
#elif defined(STM32G4)
    #include "flash_g4.h"
#endif

#endif
