from esphome.components import text_sensor
from . import HDPSDevice
import esphome.codegen as cg

import esphome.config_validation as cv

DEPENDENCIES = ["hdxps"]

CODEOWNERS = ["@tylerjwatson"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID("device_id"): cv.use_id(HDPSDevice),
        cv.Optional("serial"): text_sensor.text_sensor_schema(
            icon="mdi:signal",
        ),
    }
)


async def to_code(config):
    device = await cg.get_variable(config["device_id"])

    if conf := config.get("serial"):
        cg.add(device.set_serial_sensor(await text_sensor.new_text_sensor(conf)))
