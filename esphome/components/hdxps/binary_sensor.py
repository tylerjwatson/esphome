from esphome.components import binary_sensor
from esphome.const import CONF_ID, DEVICE_CLASS_CONNECTIVITY, ENTITY_CATEGORY_DIAGNOSTIC
from . import HDPSDevice
import esphome.codegen as cg

import esphome.config_validation as cv

DEPENDENCIES = ["hdxps"]

CODEOWNERS = ["@tylerjwatson"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID("device_id"): cv.use_id(HDPSDevice),
        cv.Optional("online"): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_CONNECTIVITY,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon="mdi:signal",
        ),
    }
)


async def to_code(config):
    device = await cg.get_variable(config["device_id"])

    if online_config := config.get("online"):
        sens = await binary_sensor.new_binary_sensor(online_config)
        cg.add(device.set_online_sensor(sens))
