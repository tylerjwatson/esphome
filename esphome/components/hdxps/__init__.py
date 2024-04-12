import esphome.codegen as cg
from .const import CONF_OUTPUT_ENABLED
import esphome.config_validation as cv
from esphome.components import uart
from esphome.components.uart import UARTComponent
from esphome.const import CONF_ID, CONF_UART_ID

hdxps = cg.esphome_ns.namespace("hdxps")
HDPSDevice = hdxps.class_("HDPSDevice", cg.PollingComponent, uart.UARTDevice)

CODEOWNERS = ["@tylerjwatson"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["binary_sensor", "sensor", "text_sensor", "number", "switch"]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(HDPSDevice),
            cv.Required(CONF_UART_ID): cv.use_id(UARTComponent),
            cv.Required(CONF_OUTPUT_ENABLED): cv.boolean,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
