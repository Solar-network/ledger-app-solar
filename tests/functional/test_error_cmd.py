import pytest

from client.exception import *
from speculos.client import ApduException


@pytest.mark.xfail(raises=ClaNotSupportedError, strict=True)
def test_bad_cla(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xA0, ins=0xB2, p1=0x00, p2=0x00, cdata=b""  # 0xa0 instead of 0xe0
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=0xB2)


@pytest.mark.xfail(raises=InsNotSupportedError, strict=True)
def test_bad_ins(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xFF, p1=0x00, p2=0x00, cdata=b""  # bad INS
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=0xFF)


@pytest.mark.xfail(raises=WrongP1P2Error, strict=True)
def test_wrong_p1p2(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xA1, p1=0x01, p2=0x00, cdata=b""  # 0x01 instead of 0x00
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=0xA1)


@pytest.mark.xfail(raises=WrongDataLengthError, strict=True)
def test_wrong_data_length(cmd):
    try:
        # APDUs must be at least 5 bytes: CLA, INS, P1, P2, Lc.
        cmd.transport.apdu_exchange_raw("E000")
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=None)


@pytest.mark.xfail(raises=OutOfOrderReqError, strict=True)
def test_wrong_order(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC1,
            p1=0x00,
            p2=0x80,
            cdata=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xC1, p1=0x01, p2=0x80, cdata=b"a" * 255
        )

        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xC1, p1=0x03, p2=0x00, cdata=b"a"  # wrong p1
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=0xC1)


@pytest.mark.xfail(raises=WrongTxLengthError, strict=True)
def test_not_full_req(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC1,
            p1=0x00,
            p2=0x80,
            cdata=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xC1, p1=0x01, p2=0x80, cdata=b"a" * 255
        )

        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC1,
            p1=0x02,
            p2=0x80,
            cdata=b"a" * 254,  # more but chunk not full
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=0xC1)


@pytest.mark.xfail(raises=BadStateError, strict=True)
def test_instruction_swap(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC1,
            p1=0x00,
            p2=0x80,
            cdata=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xC1, p1=0x01, p2=0x80, cdata=b"a" * 255
        )

        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xC2, p1=0x02, p2=0x80, cdata=b"b" * 255
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=0xC2)


@pytest.mark.xfail(raises=BadStateError, strict=True)
def test_state_cleared(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC1,
            p1=0x00,
            p2=0x80,
            cdata=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        cmd.transport.apdu_exchange_with_buttons(
            cla=0xE0,
            ins=0xC1,
            p1=0x01,
            p2=0x00,
            cdata=bytes.fromhex("fd00" + "63" * 253),
            n_screens=13,
        )

        cmd.transport.apdu_exchange(
            cla=0xE0, ins=0xC1, p1=0x02, p2=0x00, cdata=b"b" * 255
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=0xC1)
