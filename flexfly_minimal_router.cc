
//#include <sstmac/hardware/router/ugal_routing.h>
#include <sstmac/hardware/topology/structured_topology.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>

#include "flexfly_minimal_router.h"  
#include "flexfly_topology_simplified.h"

namespace sstmac {
namespace hw {

flexfly_minimal_router::flexfly_minimal_router(sprockit::sim_parameters *params, topology *top, network_switch *netsw)
  :  router(params, top, netsw)
{

  sid_ = addr();
  ftop_ = safe_cast(flexfly_topology_simplified, top);
};

void flexfly_minimal_router::route(packet* pack) {
  packet::path& path = pack->current_path();
  switch_id my_addr = my_addr_;
  uint16_t port;
  switch_id ej_addr = ftop_->node_to_injection_switch(pack->toaddr(), port);
  if (my_addr_ == ej_addr) {
  	path.set_outport(port);
  } else {
  	ftop_->minimal_route_to_switch(my_addr, ej_addr, path);
  }
  return;
};

}
}