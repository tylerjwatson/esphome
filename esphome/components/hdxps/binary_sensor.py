from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_CONNECTIVITY, ENTITY_CATEGORY_DIAGNOSTIC
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
        cv.Optional("ovp_shutdown"): binary_sensor.binary_sensor_schema(),
        cv.Optional("otp_shutdown"): binary_sensor.binary_sensor_schema(),
        cv.Optional("olp_shutdown"): binary_sensor.binary_sensor_schema(),
        cv.Optional("fan_failure"): binary_sensor.binary_sensor_schema(),
        cv.Optional("smps_failure"): binary_sensor.binary_sensor_schema(),
        cv.Optional("high_temp_alarm"): binary_sensor.binary_sensor_schema(),
        cv.Optional("ac_derating"): binary_sensor.binary_sensor_schema(),
        cv.Optional("ac_failure"): binary_sensor.binary_sensor_schema(),
    }
)


async def to_code(config):
    device = await cg.get_variable(config["device_id"])

    if online_config := config.get("online"):
        sens = await binary_sensor.new_binary_sensor(online_config)
        cg.add(device.set_online_sensor(sens))

    if conf := config.get("ovp_shutdown"):
        cg.add(
            device.set_ovp_shutdown_sensor(await binary_sensor.new_binary_sensor(conf))
        )

    if conf := config.get("otp_shutdown"):
        cg.add(
            device.set_otp_shutdown_sensor(await binary_sensor.new_binary_sensor(conf))
        )

    if conf := config.get("olp_shutdown"):
        cg.add(
            device.set_olp_shutdown_sensor(await binary_sensor.new_binary_sensor(conf))
        )

    if conf := config.get("fan_failure"):
        cg.add(
            device.set_fan_failure_sensor(await binary_sensor.new_binary_sensor(conf))
        )

    if conf := config.get("smps_failure"):
        cg.add(
            device.set_smps_failure_sensor(await binary_sensor.new_binary_sensor(conf))
        )

    if conf := config.get("high_temp_alarm"):
        cg.add(
            device.set_high_temp_alarm_sensor(
                await binary_sensor.new_binary_sensor(conf)
            )
        )

    if conf := config.get("ac_derating"):
        cg.add(
            device.set_ac_derating_sensor(await binary_sensor.new_binary_sensor(conf))
        )

    if conf := config.get("ac_failure"):
        cg.add(
            device.set_ac_failure_sensor(await binary_sensor.new_binary_sensor(conf))
        )
