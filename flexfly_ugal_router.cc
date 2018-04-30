
//#include <sstmac/hardware/router/ugal_routing.h>
#include <sstmac/hardware/topology/structured_topology.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>

#include "flexfly_ugal_router.h"  
#include "flexfly_topology_simplified.h"

namespace sstmac {
namespace hw {

flexfly_ugal_router::flexfly_ugal_router(sprockit::sim_parameters *params, topology *top, network_switch *netsw)
  :  ugal_router(params, top, netsw)
{
  val_threshold_ = params->get_optional_int_param("ugal_threshold", 0);
  val_preference_factor_ = params->get_optional_int_param("valiant_preference_factor",1);
};

bool flexfly_ugal_router::route_common(packet* pkt) const {
  uint16_t port;
  switch_id ej_addr = ftop_->netlink_to_ejection_switch(pkt->toaddr(), port);
  if (my_addr_ == ej_addr) return true; // return 

  // if we got here, that means that we haven't reached our destination switch yet
  return false;
}


void flexfly_ugal_router::route(packet* pkt) {

  bool eject = route_common(pkt);
  if (eject) return; // if eject then no need to route anymore

  packet::path& path = pkt->current_path();




    ftop_->minimal_route_to_switch(my_addr_, pkt->dest_switch(), path);
    auto hdr = pkt->get_header<header>();
    path.vc = hdr->num_group_hops;
    if (ftop_->is_global_port(path.outport())){
      ++hdr->num_group_hops;
    }
    ++hdr->num_hops;
};



}
}