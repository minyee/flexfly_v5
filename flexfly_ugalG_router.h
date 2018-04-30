#ifndef sstmac_hardware_network_topology_routing_FLEXFLY_UGAL_ROUTING_H
#define sstmac_hardware_network_topology_routing_FLEXFLY_UGAL_ROUTING_H

#include <sstmac/hardware/interconnect/interconnect.h>
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
class flexfly_ugalG_router : public ugal_router
{
  struct header : public ugal_router::header {
    char stage : 3;
    uint16_t entrySWID;
    uint16_t exitSWID;
  };
  public:
  FactoryRegister("flexfly_simplified_ugalG", router, flexfly_ugalG_router,
              "router implementing ugal global-congestion-aware routing in the flexfly_topology")
  flexfly_ugalG_router(sprockit::sim_parameters* params, topology* top, network_switch* netsw);
  

  std::string to_string() const override {
    return "flexfly_simplified_ugal";
  };




  virtual int num_vc() const override {
    return 3;
  }


  virtual void route(packet* pac) override;

 protected:
  int val_threshold_;
  int val_preference_factor_;
 
 private:
  void route_to_intermediate_group_stage(packet* pkt);

  void route_to_dest(packet* pkt);

  flexfly_topology_simplified* ftop_;
  
  hw::interconnect* ic_;
};  

}
}


#endif // UGAL_ROUTING_H