#ifndef sstmac_hardware_network_topology_routing_FLEXFLY_UGAL_ROUTING_H
#define sstmac_hardware_network_topology_routing_FLEXFLY_UGAL_ROUTING_H

#include <sstmac/hardware/router/ugal_routing.h>
#include <sstmac/hardware/router/router.h>
#include <sstmac/hardware/router/valiant_routing.h>
#include <sstmac/hardware/topology/topology.h>
#include <sstmac/hardware/interconnect/interconnect.h>

#include "flexfly_topology_simplified.h" 



namespace sstmac {
namespace hw {

/**
 * @brief The ugal_router class
 * Encapsulates a router that performs Univeral Globally Adaptive Load-balanced
 * routing as described in PhD Thesis "Load-balanced in routing in interconnection networks"
 * by A Singh.
 */
class flexfly_valiant_router : public ugal_router
{

  struct header : public ugal_router::header {
     char num_hops : 3;
     char num_group_hops : 3;
     char stage: 3;
  };

  public:

  static const char initial = 0;
  static const char valiant_stage = 1;
  static const char final_stage = 2;
  
  FactoryRegister("flexfly_simplified_ugal", router, flexfly_valiant_router,
              "router implementing ugal congestion-aware routing in the flexfly_topology")

  flexfly_valiant_router(sprockit::sim_parameters* params, topology* top, network_switch* netsw);
  

  std::string to_string() const override {
    return "flexfly_simplified_ugal";
  };

  virtual int num_vc() const override {
    return 3;
  }

  virtual void route(packet* pkt) override;

 protected:
  int val_threshold_;
  int val_preference_factor_;
 
 private:

  flexfly_topology_simplified* ftop_;

  hw::interconnect* ic_;

  uint32_t seed_;
};

}
}

#endif // UGAL_ROUTING_H