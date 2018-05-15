
//#include <sstmac/hardware/router/ugal_routing.h>
#include <sstmac/hardware/topology/structured_topology.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <sstmac/common/event_manager.h>

#include "flexfly_valiant_router.h"  
#include "flexfly_topology_simplified.h"



namespace sstmac {
namespace hw {



flexfly_valiant_router::flexfly_valiant_router(sprockit::sim_parameters *params, topology *top, network_switch *netsw)
  :  ugal_router(params, top, netsw)
{
  seed_ = params->get_optional_int_param("seed", 30);
  ic_ = nullptr;
  ftop_ = safe_cast(flexfly_topology_simplified, top);
};


void flexfly_valiant_router::route(packet* pkt) {
  // make sure that the interconnect pointer isnt null
  if (ic_ == nullptr) {
    ic_ = netsw_->event_mgr()->interconn();
  }
  
  uint16_t outport;
  uint16_t inport;
  switch_id ej_addr = ftop_->node_to_ejection_switch(pkt->toaddr(), outport);
  switch_id inj_addr = ftop_->node_to_ejection_switch(pkt->fromaddr(), inport);
  auto hdr = pkt->get_header<header>();
  if (my_addr_ == inj_addr) {
    hdr->stage = initial_stage;
  }

  if (my_addr_ == ej_addr) {
    pkt->set_outport(outport);
    return;
  }

  switch_id intermediate_group = ftop_->random_intermediate_switch(my_addr_, ej_addr, seed_);
  switch(hdr->stage) {
    case(initial_stage):    
      pkt->set_dest_switch(intermediate_group);
      hdr->stage = valiant_stage;
      break;
    case(valiant_stage):
      pkt->set_dest_switch(ej_addr);
      hdr->stage = final_stage;
      break;
    case(final_stage):
      abort(); // there is an error, we should never reach this part of the code
      break;
  }

  packet::path& pth = pkt->current_path();
  ftop_->minimal_route_to_switch(my_addr_, pkt->dest_switch(), pth);
};



}
}