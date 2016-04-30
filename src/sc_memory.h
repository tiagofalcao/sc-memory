#ifndef SC_MEMORY_H_
#define SC_MEMORY_H_

//////////////////////////////////////////////////////////////////////////////

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>

//////////////////////////////////////////////////////////////////////////////

namespace sc_memory {

/// A TLM memory
struct memory : sc_core::sc_module {

public:
  memory(sc_core::sc_module_name module_name, unsigned int start_address,
         unsigned int end_address);
  virtual ~memory();

  void dump(const char *filename);;

  unsigned int start_address() const;
  unsigned int end_address() const;
  unsigned int size() const;

  /* memory direct access functions  - useful to load the application in memory
   */
  unsigned int read(unsigned int address, unsigned int size,
                    unsigned char *data);
  unsigned int write(unsigned int address, unsigned int size,
                     const unsigned char *data);

  void delay_receive_set(const sc_core::sc_time &delay_time);
  void delay_answer_set(const sc_core::sc_time &delay_time);

private:
  uint8_t *data;
  unsigned int m_size;
  unsigned int m_start_address;
  unsigned int m_end_address;

protected:
  sc_core::sc_time delay_receive;
  sc_core::sc_time delay_answer;
};
};

#endif // SC_MEMORY_H_
