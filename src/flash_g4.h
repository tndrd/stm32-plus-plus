#ifndef _FLASH4XX_H
#define _FLASH4XX_H

#include "stm32.h"
#include "cpuid.h"
#include "core/coreexception.h"

class Flash
{
public:
    #if defined(STM32G431xx)
    static const uint32_t Size     = 0x10000;  // 64K
    static const uint32_t PageSize = 0x00800;  // 2K
    #else
      #error "Not implemented"
    #endif

    static const uint32_t PageCount = Size / PageSize;

    enum Sector: uint8_t
    {
        Page0 = 0,
        Page1 = 1,
        // ...
        Page62 = 62,
        Page63 = 63,

        InvalidPage = 0xFF,
    };

    enum Status: uint32_t
    {
        sCompleted          = 0,
        sBusy               = FLASH_SR_BSY_Msk,
        sErrorOptInvalid    = FLASH_SR_OPTVERR_Msk,
        sErrorReadProtect   = FLASH_SR_RDERR_Msk,
        sErrorFastProg      = FLASH_SR_FASTERR_Msk,
        sErrorFastProgMiss  = FLASH_SR_MISERR_Msk, 
        sErrorProgSeq       = FLASH_SR_PGSERR_Msk,
        sErrorSize          = FLASH_SR_SIZERR_Msk,
        sErrorAlignment     = FLASH_SR_PGAERR_Msk,
        sErrorWriteProtect  = FLASH_SR_WRPERR_Msk,
        sErrorProgramming   = FLASH_SR_PROGERR_Msk,
        sErrorOperation     = FLASH_SR_OPERR_Msk,
        sEndOfOperation     = FLASH_SR_EOP_Msk,
        StatusMask          = 0x0001c3fb
    } ;

private:
    static void clearProgrammingFlags();
    
    static Status status();
    static Status wait();

    static void programDWord_impl(void* address, uint64_t value);
public:
    static Sector getSectorByAddress(const void* addr);
    static void* getBeginOfSector(Sector sector);
    static uint32_t getSizeOfSector(Sector sector);

    static void unlock();
    static void lock();

    static Status eraseSector(Sector sector);
    static uint32_t readWord(uint32_t address) {return *reinterpret_cast<uint32_t*>(address);}

    static Status programData(void* address, const void *data, uint32_t size);
    
    // Adapters for backward compatibility
    static Status programData(unsigned long address, const void *data, unsigned long size);
    static Status programWord(unsigned long address, unsigned long value);


    template<typename T>
    static Status program(void* address, const T& value)
    {
      return programData(address, &value, sizeof(value));
    }

    static Status programDWord(uint64_t* address, uint64_t value);
    static Status programWord(uint32_t* address, uint32_t value);
    static Status programHWord(uint16_t* address, uint16_t value);
    static Status programByte(uint8_t* address, uint8_t value);
};

#endif
