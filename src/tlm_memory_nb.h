//////////////////////////////////////////////////////////////////////////////

#ifndef TLM_MEMORY_NB_H_
#define TLM_MEMORY_NB_H_

//////////////////////////////////////////////////////////////////////////////

#include "sc_memory.h"
#include <tlm_utils/simple_target_socket.h>

//////////////////////////////////////////////////////////////////////////////

namespace sc_memory {

struct tlm_memory_nb : sc_memory::memory {

public:
  tlm_memory_nb(sc_core::sc_module_name module_name, unsigned int start_address,
                unsigned int end_address);
  virtual ~tlm_memory_nb();

  tlm_utils::simple_target_socket<tlm_memory_nb> socket;
  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &payload,
                                     tlm::tlm_phase &phase,
                                     sc_core::sc_time &delay_time);

  tlm_utils::peq_with_get<tlm::tlm_generic_payload> peq;
  void proc();
};
};

#endif // TLM_MEMORY_NB_H_
