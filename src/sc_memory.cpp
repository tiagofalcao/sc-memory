#include "sc_memory.h"
#include <endian.h>
#include <expio.h>
#include <string.h>
#include <sys/mman.h>

#ifndef MAP_HUGE_2MB
#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)
#endif

using sc_memory::memory;
typedef memory SC_CURRENT_USER_MODULE;

/// Constructor
memory::memory(sc_core::sc_module_name module_name, unsigned int start_address,
               unsigned int end_address)
    : sc_module(module_name) {

  delay_receive_set(sc_core::SC_ZERO_TIME);
  delay_answer_set(sc_core::SC_ZERO_TIME);

  m_start_address = start_address;
  m_end_address = end_address;

  sc_assert(m_start_address <= m_end_address);
  m_size = (m_end_address - m_start_address + 1);
  EXPIO_LOG_INFO("Memory %u-%u [%x]", m_start_address, m_end_address, m_size);

  sc_assert(m_size % 4 == 0);

  data = (uint8_t *)mmap(
      NULL, m_size, PROT_WRITE | PROT_READ,
      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (data < 0) {
    EXPIO_LOG_CRIT("Mmap failed.");
  }
}

/// Destructor
memory::~memory() {
  munmap(data, m_size);
  data = NULL;
}

void memory::dump(const char *filename) {
  char dbg[50];
  if (!filename) {
    filename = "memory_0x%x-0x%x.dump";
  }
  snprintf(dbg, 50, filename, m_start_address, m_end_address);

  FILE *f = fopen(dbg, "w");
  fwrite(data, sizeof(unsigned char), m_size, f);
  fclose(f);
}

void memory::delay_receive_set(const sc_core::sc_time &delay_time) {
  delay_receive = delay_time;
}

void memory::delay_answer_set(const sc_core::sc_time &delay_time) {
  delay_answer = delay_time;
}

unsigned int memory::read(unsigned int address, unsigned int size,
                          unsigned char *value) {
  unsigned int addr = address - m_start_address;

  if (addr < 0) {
    EXPIO_LOG_ERR("Out of bounds memory position (%d) [%d - %d].", address,
                  m_start_address, m_end_address);
    return 0;
  }

  if ((addr + size) > m_size) {
    EXPIO_LOG_ERR("Request more memory than have [(%d) %d / %d].", addr, size,
                  m_size);
    size = m_size - addr;
  }

  memcpy(value, &(data[addr]), size);

  return size;
}
unsigned int memory::write(unsigned int address, unsigned int size,
                           const unsigned char *value) {
  unsigned int addr = address - m_start_address;

  if (addr < 0) {
    EXPIO_LOG_ERR("Out of bounds memory position (%d) [%d - %d].", address,
                  m_start_address, m_end_address);
    return 0;
  }

  if ((addr + size) > m_size) {
    EXPIO_LOG_ERR("Request more memory than have [(%d) %d / %d].", addr, size,
                  m_size);
    size = m_size - addr;
  }

  memcpy(&(data[addr]), value, size);

  return size;
}

unsigned int memory::start_address() const { return m_start_address; }

unsigned int memory::end_address() const { return m_end_address; }

unsigned int memory::size() const { return m_size; };
