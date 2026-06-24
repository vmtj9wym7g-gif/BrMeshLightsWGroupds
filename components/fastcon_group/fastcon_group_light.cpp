#include "esphome/core/log.h"
#include "esphome/components/light/light_state.h"
#include "fastcon_group_light.h"
#include <algorithm>

#ifndef FASTCON_VERSION
#define FASTCON_VERSION "0.3.3-dev"
#endif

namespace esphome {
namespace fastcon {

static const char *const TAG = "fastcon_group.light";

light::LightTraits FastconGroupLight::get_traits() {
  light::LightTraits t;
  t.set_supported_color_modes({light::ColorMode::ON_OFF});
  return t;
}

void FastconGroupLight::write_state(light::LightState *state) {
  if (this->controller_ == nullptr) {
    ESP_LOGW(TAG, "No controller bound; dropping group command");
    return;
  }

  auto &values = state->current_values;
  const bool is_on = values.is_on();
  uint8_t level = is_on ? 0x80 : 0x00;

  std::vector<uint8_t> group_bytes = {
      0x43, 0x2A, 0xA8, this->group_id_, level,
      0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00,
  };

  std::vector<uint8_t> payload = this->controller_->group_control(this->group_id_, group_bytes);
  this->controller_->queueCommand(this->group_id_, payload);

  ESP_LOGD(TAG, "Queued group state v%s: group_id=%u, level=0x%02X, payload_len=%d",
           FASTCON_VERSION, (unsigned)this->group_id_, level, (int)payload.size());
}

}  // namespace fastcon
}  // namespace esphome
