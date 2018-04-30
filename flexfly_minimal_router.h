#ifndef sstmac_hardware_network_topology_routing_UGAL_ROUTING_H
#define sstmac_hardware_network_topology_routing_UGAL_ROUTING_H

#include <sstmac/hardware/router/ugal_routing.h>
#include <sstmac/hardware/router/router.h>
#include <sstmac/hardware/router/valiant_routing.h>
#include "flexfly_topology_simplified.h"
namespace sstmac {
namespace hw {

/**
 * @brief The ugal_router class
 * Encapsulates a router that performs Univeral Globally Adaptive Load-balanced
 * routing as described in PhD Thesis "Load-balanced in routing in interconnection networks"
 * by A Singh.
 */
class flexfly_minimal_router :
  public router
{
  FactoryRegister("flexfly_simplified_minimal", router, flexfly_minimal_router,
              "router implementing minimal routing in the flexfly_topology")
  flexfly_minimal_router(sprockit::sim_parameters* params, topology* top, network_switch* netsw);
 public:
  

  std::string to_string() const override {
    return "flexfly_simplified_ugal";
  };

  void route(packet* pack) override;

 protected:


  virtual int num_vc() const override {
    return 2;
  }

 protected:

 private:
  flexfly_topology_simplified* ftop_;
  switch_id sid_;
};

}
}


#endif // UGAL_ROUTING_H