from esphome.components import sensor
from esphome.const import CONF_ID, DEVICE_CLASS_CONNECTIVITY, ENTITY_CATEGORY_DIAGNOSTIC
from . import HDPSDevice
import esphome.codegen as cg

import esphome.config_validation as cv

DEPENDENCIES = ["hdxps"]

CODEOWNERS = ["@tylerjwatson"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID("device_id"): cv.use_id(HDPSDevice),
    }
)


async def to_code(config):
    device = await cg.get_variable(config["device_id"])
