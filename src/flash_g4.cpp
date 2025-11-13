#include "flash.h"
#include <assert.h>

#if defined(STM32G4)

Flash::Sector Flash::getSectorByAddress(const void* addr_)
{
  uint64_t addr = reinterpret_cast<uint64_t>(addr_);

  if (addr < FLASH_BASE || addr >= FLASH_BASE + Size)
    return InvalidPage;

  return static_cast<Sector>((addr - FLASH_BASE) / PageSize);
}

void* Flash::getBeginOfSector(Sector sector)
{
  if (sector >= PageCount)
    THROW(Exception::OutOfRange);

  return reinterpret_cast<void*>(FLASH_BASE + sector * PageSize);
}

uint32_t Flash::getSizeOfSector(Sector sector)
{
  return PageSize;
}

//---------------------------------------------------------------------------

Flash::Status Flash::status()
{
  return static_cast<Status>(FLASH->SR & StatusMask);
}

Flash::Status Flash::wait()
{
  while (FLASH->SR & sBusy);
  return status();
}
//---------------------------------------------------------------------------

void Flash::unlock()
{
  if (FLASH->CR & FLASH_CR_LOCK)
  {
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
  }
}

void Flash::lock()
{
  FLASH->CR |= FLASH_CR_LOCK;
}

void Flash::clearProgrammingFlags()
{
  // Each flag is cleared by writing "1" to it
  FLASH->SR |= FLASH_SR_PROGERR | FLASH_SR_SIZERR | FLASH_SR_PGAERR  | 
               FLASH_SR_WRPERR  | FLASH_SR_MISERR | FLASH_SR_FASTERR ;
}

//---------------------------------------------------------------------------

Flash::Status Flash::eraseSector(Sector sector)
{
  wait();
  clearProgrammingFlags();
  
  // Set page number and start erasing
  FLASH->CR |= (sector << FLASH_CR_PNB_Pos) & FLASH_CR_PNB_Msk;
  FLASH->CR |= FLASH_CR_PER | FLASH_CR_STRT;
  
  return wait();
}

//---------------------------------------------------------------------------

void Flash::programDWord_impl(void* address, uint64_t value)
{
  *reinterpret_cast<volatile uint64_t*>(address) = value;
}

static uint32_t mod(const void* addr, uint32_t val)
{
    return uint32_t(addr) % val;
}

// bitshift operators' actual directions are inverted due to endianness
Flash::Status Flash::programData(void* dst_, const void* src_, uint32_t size)
{
  auto dst = reinterpret_cast<uint8_t*>(dst_);
  auto src = reinterpret_cast<const uint8_t*>(src_);
  
  // Enable programming
  wait();
  clearProgrammingFlags();
  FLASH->CR |= FLASH_CR_PG;
  
  // If dst address is not 8-aligned
  if (mod(dst, 8))
  {
    uint8_t offs = mod(dst, 8);
    uint64_t val = *reinterpret_cast<const uint64_t*>(src);
    
    val <<= 8 * offs;
    val |= (~uint64_t(0)) >> 8 * (8 - offs); 

    programDWord_impl(dst - offs, val);
    
    dst  += 8 - offs;
    src  += 8 - offs;
    size -= 8 - offs;
  }

  assert(mod(dst, 8) == 0);

  uint32_t oldsz = size;
  for (int i = 0; i < oldsz / 8; ++i)
  {
    programDWord_impl(dst, *reinterpret_cast<const uint64_t*>(src));
    dst  += 8;
    src  += 8;
    size -= 8;
  }

  assert(size < 8);

  // If data less than a dword remains
  if (size != 0)
  {
    uint64_t val = *reinterpret_cast<const uint64_t*>(src);
    val |= (~uint64_t(0)) << size * 8;

    programDWord_impl(dst, val);
  }

  // Disable programming
  FLASH->SR |= FLASH_SR_EOP;
  FLASH->CR &= ~FLASH_CR_PG;

  return wait();
}

//---------------------------------------------------------------------------

Flash::Status Flash::programDWord(uint64_t* address, uint64_t value)
{
  return program<uint64_t>(address, value);
}

Flash::Status Flash::programWord(uint32_t* address, uint32_t value)
{
  return program<uint32_t>(address, value);
}

Flash::Status Flash::programHWord(uint16_t* address, uint16_t value)
{
  return program<uint16_t>(address, value);
}

Flash::Status Flash::programByte(uint8_t* address, uint8_t value)
{
  return program<uint8_t>(address, value);
}

//---------------------------------------------------------------------------

Flash::Status Flash::programData(unsigned long address, const void *data, unsigned long size)
{
  return programData(reinterpret_cast<void*>(address), data, size);
}

Flash::Status Flash::programWord(unsigned long address, unsigned long value)
{
  return programWord(reinterpret_cast<uint32_t*>(address), value);
}

#endif