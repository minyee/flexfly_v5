
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
  ic_ = nullptr;

};

void flexfly_ugal_router::route_initial(packet* pkt) const {
  packet::path& pth = pkt->get_current_path();
  std::vector<topology::connection> reachable_groups;
  packet::path min_path;
  packet::path valiant_path;
  switch_paths();

};

bool flexfly_ugal_router::switch_paths(switch_id orig_dst, 
                                        switch_id new_dst, 
                                        packet::path& orig_path, 
                                        packet::path& new_path) {
  switch_id src = my_addr_;
  ftop_->minimal_route_to_switch(src, orig_dst, orig_path);
  ftop_->minimal_route_to_switch(src, new_dst, new_path);
  int orig_queue_length = netsw_->queue_length(orig_path.outport());
  int new_queue_length = netsw_->queue_length(new_path.outport());
  int orig_distance = ftop_->minimal_distance(src, orig_dst);
  int new_distance = ftop_->minimal_distance(src, new_dst)
                      + ftop_->minimal_distance(new_dst, orig_dst);
  int orig_weight = orig_queue_length * orig_distance * val_preference_factor_;
  int valiant_weight = new_queue_length * new_distance;
  return valiant_weight < orig_weight;
}

bool flexfly_ugal_router::route_common(packet* pkt) const {
  uint16_t port;
  switch_id ej_addr = ftop_->node_to_ejection_switch(pkt->toaddr(), port);
  packet::path& pth = pkt->get_current_path();
  pth->set_outport(port);
  if (my_addr_ == ej_addr) {
    // checks if we are at the ejection switch yet
    return true;
  } 
  auto hdr = pkt->get_header<header>();
  switch(hdr->stage) {
    case(initial): 
        route_initial(pkt);
        hdr->stage = valiant;
        break;
    case(valiant):
        break;
  }
  // if we got here, that means that we haven't reached our destination switch yet
  
  uint64_t minimal_queue_length = UINT_MAX; 
  switch_id target_group = my_addr_;

  assert(target_group != my_addr_);
  pkt->set_dest_switch(target_group);
  return false;
};


void flexfly_ugal_router::route(packet* pkt) {
  // make sure that the interconnect pointer isnt null
  if (ic_ == nullptr) {
    ic_ = netsw_->event_mgr()->interconn();
  }


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