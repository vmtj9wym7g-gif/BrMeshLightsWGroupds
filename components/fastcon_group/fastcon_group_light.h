#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/fastcon/fastcon_controller.h"

namespace esphome {
namespace fastcon {

class FastconGroupLight : public Component, public light::LightOutput {
 public:
  explicit FastconGroupLight(int group_id) { this->group_id_ = static_cast<uint8_t>(group_id); }

  void set_controller(FastconController *controller) { this->controller_ = controller; }

  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;

 protected:
  uint8_t group_id_{0};
  FastconController *controller_{nullptr};
};

}  // namespace fastcon
}  // namespace esphome

