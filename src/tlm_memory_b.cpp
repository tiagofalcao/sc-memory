#include "tlm_memory_b.h"
#include <expio.h>
#include <string.h>

using sc_memory::tlm_memory_b;
typedef tlm_memory_b SC_CURRENT_USER_MODULE;

/// Constructor
tlm_memory_b::tlm_memory_b(sc_core::sc_module_name module_name,
                           unsigned int start_address, unsigned int end_address)
    : sc_memory::memory(module_name, start_address, end_address),
      socket("socket") {

  socket.register_b_transport(this, &tlm_memory_b::b_transport);
}

/// Destructor
tlm_memory_b::~tlm_memory_b() {}

void tlm_memory_b::b_transport(tlm::tlm_generic_payload &payload,
                               sc_core::sc_time &delay_time) {
  tlm::tlm_command cmd = payload.get_command();
  unsigned int addr = payload.get_address();
  unsigned char *ptr = payload.get_data_ptr();
  unsigned int len = payload.get_data_length();

  switch (cmd) {
  case tlm::TLM_READ_COMMAND: {
    read(addr, len, ptr);
    payload.set_response_status(tlm::TLM_OK_RESPONSE);
    break;
  }

  case tlm::TLM_WRITE_COMMAND: {
    write(addr, len, ptr);
    payload.set_response_status(tlm::TLM_OK_RESPONSE);
    break;
  }

  default:
    EXPIO_LOG_CRIT("Command %d is not implemented.", cmd);
    break;
  }

  delay_time = delay_receive + delay_answer;
}
