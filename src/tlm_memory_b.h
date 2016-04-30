//////////////////////////////////////////////////////////////////////////////

#ifndef TLM_MEMORY_B_H_
#define TLM_MEMORY_B_H_

//////////////////////////////////////////////////////////////////////////////

#include "sc_memory.h"
#include <tlm_utils/simple_target_socket.h>

//////////////////////////////////////////////////////////////////////////////

namespace sc_memory {

struct tlm_memory_b : sc_memory::memory {

public:
  tlm_memory_b(sc_core::sc_module_name module_name, unsigned int start_address,
               unsigned int end_address);
  virtual ~tlm_memory_b();

  tlm_utils::simple_target_socket<tlm_memory_b> socket;
  void b_transport(tlm::tlm_generic_payload &payload,
                   sc_core::sc_time &delay_time);
};
};

#endif // TLM_MEMORY_B_H_
