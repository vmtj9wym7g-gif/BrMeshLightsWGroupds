"""Light platform for Fastcon/BRMesh group controls."""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID

DEPENDENCIES = ["fastcon"]
AUTO_LOAD = ["light"]

CONF_CONTROLLER_ID = "controller_id"
CONF_GROUP_ID = "group_id"

fastcon_ns = cg.esphome_ns.namespace("fastcon")
FastconController = fastcon_ns.class_("FastconController", cg.Component)
FastconGroupLight = fastcon_ns.class_("FastconGroupLight", light.LightOutput, cg.Component)

CONFIG_SCHEMA = (
    light.LIGHT_SCHEMA
    .extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(FastconGroupLight),
            cv.Required(CONF_GROUP_ID): cv.int_range(min=1, max=255),
            cv.Optional(CONF_CONTROLLER_ID, default="fastcon_controller"): cv.use_id(FastconController),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], config[CONF_GROUP_ID])

    await cg.register_component(var, config)
    await light.register_light(var, config)

    controller = await cg.get_variable(config[CONF_CONTROLLER_ID])
    cg.add(var.set_controller(controller))
