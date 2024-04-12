from esphome.components import sensor
from . import HDPSDevice
import esphome.codegen as cg

import esphome.config_validation as cv

DEPENDENCIES = ["hdxps"]

CODEOWNERS = ["@tylerjwatson"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID("device_id"): cv.use_id(HDPSDevice),
        cv.Optional("temperature"): sensor.sensor_schema(
            unit_of_measurement="°C",
            accuracy_decimals=1,
            icon="mdi:thermometer",
        ),
        cv.Optional("rated_voltage"): sensor.sensor_schema(
            unit_of_measurement="V",
            accuracy_decimals=1,
            icon="mdi:flash",
        ),
        cv.Optional("rated_current"): sensor.sensor_schema(
            unit_of_measurement="A",
            accuracy_decimals=1,
            icon="mdi:flash",
        ),
        cv.Optional("output_voltage"): sensor.sensor_schema(
            unit_of_measurement="V",
            accuracy_decimals=1,
            icon="mdi:flash",
        ),
        cv.Optional("output_current"): sensor.sensor_schema(
            unit_of_measurement="A",
            accuracy_decimals=1,
            icon="mdi:flash",
        ),
    }
)


async def to_code(config):
    device = await cg.get_variable(config["device_id"])

    if conf := config.get("temperature"):
        cg.add(device.set_temperature_sensor(await sensor.new_sensor(conf)))

    if conf := config.get("rated_voltage"):
        cg.add(device.set_rated_voltage_sensor(await sensor.new_sensor(conf)))

    if conf := config.get("rated_current"):
        cg.add(device.set_rated_current_sensor(await sensor.new_sensor(conf)))

    if conf := config.get("output_voltage"):
        cg.add(device.set_output_voltage_sensor(await sensor.new_sensor(conf)))

    if conf := config.get("output_current"):
        cg.add(device.set_output_current_sensor(await sensor.new_sensor(conf)))
