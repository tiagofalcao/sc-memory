#include "tlm_memory_nb.h"
#include <expio.h>
#include <string.h>

using sc_memory::tlm_memory_nb;
typedef tlm_memory_nb SC_CURRENT_USER_MODULE;

/// Constructor
tlm_memory_nb::tlm_memory_nb(sc_core::sc_module_name module_name,
                             unsigned int start_address,
                             unsigned int end_address)
    : sc_memory::memory(module_name, start_address, end_address),
      socket("socket"), peq("peq") {

  SC_THREAD(proc);
  socket.register_nb_transport_fw(this, &tlm_memory_nb::nb_transport_fw);
}

/// Destructor
tlm_memory_nb::~tlm_memory_nb() {}

tlm::tlm_sync_enum
tlm_memory_nb::nb_transport_fw(tlm::tlm_generic_payload &payload,
                               tlm::tlm_phase &phase,
                               sc_core::sc_time &delay_time) {
  tlm::tlm_command cmd = payload.get_command();

  switch (cmd) {
  case tlm::TLM_READ_COMMAND:
  case tlm::TLM_WRITE_COMMAND:
    payload.set_response_status(tlm::TLM_OK_RESPONSE);
    break;
  default:
    EXPIO_LOG_CRIT("Command %d is not implemented.", cmd);
    break;
  }

  peq.notify(payload, delay_receive);

  phase = tlm::END_REQ;
  return tlm::TLM_UPDATED;
}

void tlm_memory_nb::proc() {
  tlm::tlm_generic_payload *payload;
  sc_core::sc_time delay;
  tlm::tlm_phase ph;

  while (true) {
    tlm::tlm_command cmd;
    unsigned int addr;
    unsigned char *ptr;
    unsigned int len;

    wait(peq.get_event());
    while ((payload = peq.get_next_transaction())) {

      cmd = payload->get_command();
      addr = payload->get_address();
      ptr = payload->get_data_ptr();
      len = payload->get_data_length();

      switch (cmd) {
      case tlm::TLM_READ_COMMAND: {
        read(addr, len, ptr);
        payload->set_response_status(tlm::TLM_OK_RESPONSE);
        break;
      }

      case tlm::TLM_WRITE_COMMAND: {
        write(addr, len, ptr);
        payload->set_response_status(tlm::TLM_OK_RESPONSE);
        break;
      }

      default:
        EXPIO_LOG_CRIT("Command %d is not implemented.", cmd);
        break;
      }

      delay = delay_answer;

      ph = tlm::BEGIN_RESP;

      {
        tlm::tlm_sync_enum resp = socket->nb_transport_bw(*payload, ph, delay);
        if (resp != tlm::TLM_COMPLETED) {
          EXPIO_LOG_ERR("Response not completed: %d.", resp);
        }
      }
    }
  }
}
