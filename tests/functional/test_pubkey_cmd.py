import pytest

from client.exception import *
from speculos.client import ApduException
from client.cmd_builder import InsType


def test_get_public_key(cmd):

    # No Display, No Chain Code
    pub_key, chain_code = cmd.get_public_key(
        bip32_path="44'/3333'/0'/0/0", display=0, chaincode=0
    )

    assert len(pub_key) == 33
    assert len(chain_code) == 0

    # Display, No Chain Code
    pub_key2, chain_code2 = cmd.get_public_key(
        bip32_path="44'/3333'/0'/0/0",
        display=1,
        chaincode=0,
        n_screens=5,
    )

    assert len(pub_key2) == 33
    assert len(chain_code2) == 0

    # Display, Chain Code
    pub_key3, chain_code3 = cmd.get_public_key(
        bip32_path="44'/3333'/0'/0/0",
        display=1,
        chaincode=1,
        n_screens=5,
    )

    assert len(pub_key3) == 33
    assert len(chain_code3) == 32

    # No Display, Chain Code
    pub_key4, chain_code4 = cmd.get_public_key(
        bip32_path="44'/3333'/0'/0/0", display=0, chaincode=1
    )

    assert len(pub_key4) == 33
    assert len(chain_code4) == 32


@pytest.mark.xfail(raises=WrongP1P2Error, strict=True)
def test_wrong_chaincode_option(cmd):
    try:
        cmd.get_public_key(bip32_path="44'/3333'/0'/0/0", display=0, chaincode=2)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_GET_PUBLIC_KEY)


@pytest.mark.xfail(raises=WrongP1P2Error, strict=True)
def test_wrong_display_option(cmd):
    try:
        cmd.get_public_key(bip32_path="44'/3333'/0'/0/0", display=2, chaincode=0)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_GET_PUBLIC_KEY)
