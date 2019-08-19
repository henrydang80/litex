# This file is Copyright (c) 2019 Henry Dang <henrydang@fossil.com>
# License: BSD

from litex.build.generic_platform import *
from litex.build.xilinx import XilinxPlatform, VivadoProgrammer

# IOs ----------------------------------------------------------------------------------------------

_io = [
    ("user_led", 0,  Pins("U16"), IOStandard("LVCMOS33")),
    ("user_led", 1,  Pins("E19"), IOStandard("LVCMOS33")),
    ("user_led", 2,  Pins("U19"), IOStandard("LVCMOS33")),
    ("user_led", 3,  Pins("V19"), IOStandard("LVCMOS33")),
    ("user_led", 4,  Pins("W18"), IOStandard("LVCMOS33")),
    ("user_led", 5,  Pins("U15"), IOStandard("LVCMOS33")),
    ("user_led", 6,  Pins("U14"), IOStandard("LVCMOS33")),
    ("user_led", 7,  Pins("V14"), IOStandard("LVCMOS33")),
    ("user_led", 8,  Pins("V13"), IOStandard("LVCMOS33")),
    ("user_led", 9,  Pins("V3"),  IOStandard("LVCMOS33")),
    ("user_led", 10, Pins("W3"),  IOStandard("LVCMOS33")),
    ("user_led", 11, Pins("U3"),  IOStandard("LVCMOS33")),
    ("user_led", 12, Pins("P3"),  IOStandard("LVCMOS33")),
    ("user_led", 13, Pins("N3"),  IOStandard("LVCMOS33")),
    ("user_led", 14, Pins("P1"),  IOStandard("LVCMOS33")),
    ("user_led", 15, Pins("L1"),  IOStandard("LVCMOS33")),

    ("user_sw",  0,  Pins("V17"), IOStandard("LVCMOS33")),
    ("user_sw",  1,  Pins("V16"), IOStandard("LVCMOS33")),
    ("user_sw",  2,  Pins("W16"), IOStandard("LVCMOS33")),
    ("user_sw",  3,  Pins("W17"), IOStandard("LVCMOS33")),
    ("user_sw",  4,  Pins("W15"), IOStandard("LVCMOS33")),
    ("user_sw",  5,  Pins("V15"), IOStandard("LVCMOS33")),
    ("user_sw",  6,  Pins("W14"), IOStandard("LVCMOS33")),
    ("user_sw",  7,  Pins("W13"), IOStandard("LVCMOS33")),
    ("user_sw",  8,  Pins("V2"),  IOStandard("LVCMOS33")),
    ("user_sw",  9,  Pins("T3"),  IOStandard("LVCMOS33")),
    ("user_sw",  10, Pins("T2"),  IOStandard("LVCMOS33")),
    ("user_sw",  11, Pins("R3"),  IOStandard("LVCMOS33")),
    ("user_sw",  12, Pins("W2"),  IOStandard("LVCMOS33")),
    ("user_sw",  13, Pins("U1"),  IOStandard("LVCMOS33")),
    ("user_sw",  14, Pins("T1"),  IOStandard("LVCMOS33")),
    ("user_sw",  15, Pins("R2"),  IOStandard("LVCMOS33")),

    ("clk100", 0, Pins("W5"), IOStandard("LVCMOS33")),

    ("serial", 0,
        Subsignal("rx", Pins("B18")),
        Subsignal("tx", Pins("A18")),
        IOStandard("LVCMOS33")
    ),

    ("spi", 0,
        Subsignal("clk", Pins("F1")),
        Subsignal("cs_n", Pins("C1")),
        Subsignal("mosi", Pins("H1")),
        Subsignal("miso", Pins("G1")),
        IOStandard("LVCMOS33"),
    ),

    ("spiflash4x", 0,  # clock needs to be accessed through STARTUPE2
        Subsignal("sck", Pins("C11")),
        Subsignal("cs_n", Pins("K19")),
        Subsignal("dq", Pins("D18", "D19", "G18", "F18")),
        IOStandard("LVCMOS33")
    ),
    ("spiflash", 0,  # clock needs to be accessed through STARTUPE2
        Subsignal("sck", Pins("C11")),
        Subsignal("cs_n", Pins("K19")),
        Subsignal("mosi", Pins("D18")),
        Subsignal("miso", Pins("D19")),
        Subsignal("wp", Pins("G18")),
        Subsignal("hold", Pins("F18")),
        IOStandard("LVCMOS33"),
    ),
]

# Connectors ---------------------------------------------------------------------------------------

_connectors = [
    ("pmoda", "J1 L2 J2 G2 H1 K2 H2 G3"),
    ("pmodb", "A14 A16 B15 B16 A15 A17 C15 C16"),
    ("pmodc", "K17 M18 N17 P18 L17 M19 P17 R18"),
    ("pmodd", "J3 L3 M2 N2 K3 M3 M1 N1"),
]

# Platform -----------------------------------------------------------------------------------------

class Platform(XilinxPlatform):
    default_clk_name = "clk100"
    default_clk_period = 10.0

    def __init__(self):
        XilinxPlatform.__init__(self, "xc7a35tcpg236-1", _io, _connectors, toolchain="vivado")
        self.toolchain.bitstream_commands = \
            ["set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]"]
        self.toolchain.additional_commands = \
            ["write_cfgmem -force -format bin -interface spix4 -size 16 "
             "-loadbit \"up 0x0 {build_name}.bit\" -file {build_name}.bin"]
        self.add_platform_command("set_property INTERNAL_VREF 0.675 [get_iobanks 34]")

    def create_programmer(self):
        return VivadoProgrammer(flash_part="S25FL032")
