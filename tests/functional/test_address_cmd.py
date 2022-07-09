import pytest

from client.exception import *
from speculos.client import ApduException
from client.cmd_builder import InsType


def test_get_address(cmd):

    # No Display, Solar Mainnet
    address = cmd.get_address(bip32_path="44'/3333'/0'/0/0", display=0, network=0x3F)

    assert len(address) == 34
    assert address[0] == ord("S")

    # No Display, Solar Testnet
    address2 = cmd.get_address(bip32_path="44'/1'/0'/0/0", display=0, network=0x1E)

    assert len(address2) == 34
    assert address2[0] == ord("D")

    # Display, Solar Mainnet
    address3 = cmd.get_address(
        bip32_path="44'/3333'/0'/0/0",
        display=1,
        network=0x3F,
        n_screens=4,
    )

    assert len(address3) == 34
    assert address3[0] == ord("S")

    # Display, Solar Testnet
    address4 = cmd.get_address(
        bip32_path="44'/1'/0'/0/0",
        display=1,
        network=0x1E,
        n_screens=4,
    )

    assert len(address4) == 34
    assert address4[0] == ord("D")


@pytest.mark.xfail(raises=WrongP1P2Error, strict=True)
def test_wrong_network(cmd):
    try:
        cmd.get_address(bip32_path="44'/3333'/0'/0/0", display=0, network=0x3E)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_GET_ADDRESS)
