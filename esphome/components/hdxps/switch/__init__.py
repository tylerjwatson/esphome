from esphome.components import switch
from .. import HDPSDevice, hdxps
import esphome.codegen as cg
import esphome.config_validation as cv

DEPENDENCIES = ["hdxps", "uart"]

CODEOWNERS = ["@tylerjwatson"]

HDPSOutputEnabledSwitch = hdxps.class_("HDPSOutputEnabledSwitch", switch.Switch)
HDPSControlSwitch = hdxps.class_("HDPSControlSwitch", switch.Switch)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID("device_id"): cv.use_id(HDPSDevice),
        cv.Optional("output_enabled"): switch.switch_schema(
            HDPSOutputEnabledSwitch,
        ),
        cv.Optional("control"): switch.switch_schema(
            HDPSControlSwitch,
        ),
    }
)


async def to_code(config):
    device = await cg.get_variable(config["device_id"])

    if conf := config.get("output_enabled"):
        n = await switch.new_switch(conf)
        await cg.register_parented(n, device)
        cg.add(device.set_output_enabled_switch(n))

    if conf := config.get("control"):
        n = await switch.new_switch(conf)
        await cg.register_parented(n, device)
        cg.add(device.set_control_switch(n))
