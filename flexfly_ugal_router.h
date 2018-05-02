#ifndef sstmac_hardware_network_topology_routing_FLEXFLY_UGAL_ROUTING_H
#define sstmac_hardware_network_topology_routing_FLEXFLY_UGAL_ROUTING_H

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
class flexfly_ugal_router : public ugal_router
{
  const char initial = 0;
  const char ugal_stage = 1;
  const char valiant_stage = 2;
  const char final_stage = 3;
  


  struct header : public ugal_router::header {
     char num_hops : 3;
     char num_group_hops : 3;
     char stage: 3;
  };
  public:
  FactoryRegister("flexfly_simplified_ugal", router, flexfly_ugal_router,
              "router implementing ugal congestion-aware routing in the flexfly_topology")
  flexfly_ugal_router(sprockit::sim_parameters* params, topology* top, network_switch* netsw);
  

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
  bool switch_paths(switch_id orig_dst, switch_id new_dst, packet::path& orig_path, packet::path& new_path);

  bool route_common(packet* pkt) const;

  flexfly_topology_simplified* ftop_;

  hw::interconnect ic_;
};

}
}


#endif // UGAL_ROUTING_H