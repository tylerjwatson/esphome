from esphome.components import number
from .. import HDPSDevice, hdxps
import esphome.codegen as cg
import esphome.config_validation as cv

DEPENDENCIES = ["hdxps"]

CODEOWNERS = ["@tylerjwatson"]

HDPSVoltageNumber = hdxps.class_("HDPSVoltageNumber", number.Number)
HDPSCurrentNumber = hdxps.class_("HDPSCurrentNumber", number.Number)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID("device_id"): cv.use_id(HDPSDevice),
        cv.Optional("voltage"): number.number_schema(
            HDPSVoltageNumber,
            unit_of_measurement="V",
        ),
        cv.Optional("current"): number.number_schema(
            HDPSCurrentNumber,
            unit_of_measurement="A",
        ),
    }
)


async def to_code(config):
    device = await cg.get_variable(config["device_id"])

    if conf := config.get("voltage"):
        n = await number.new_number(
            conf,
            min_value=0,
            max_value=120,
            step=0.01,
        )

        await cg.register_parented(n, device)
        cg.add(device.set_voltage_number(n))

    if conf := config.get("current"):
        n = await number.new_number(
            conf,
            min_value=0,
            max_value=120,
            step=0.01,
        )

        await cg.register_parented(n, device)
        cg.add(device.set_current_number(n))
