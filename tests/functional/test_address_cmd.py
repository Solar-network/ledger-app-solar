import pytest

from ragger.error import ExceptionRAPDU
from ragger.navigator import NavInsID, NavIns
from utils import ROOT_SCREENSHOT_PATH

from application_client.solar_command_sender import (
    CLA,
    InsType,
    SolarCommandSender,
    Errors,
)
from application_client.solar_response_unpacker import unpack_get_address_response
from constants import (
    NETWORK_MAINNET,
    NETWORK_TESTNET,
    NETWORKS,
    PATH_MAINNET,
    PATH_TESTNET,
)


# In this test we check that the GET_ADDRESS works in non-confirmation mode
def test_get_address_no_confirm(backend):
    client = SolarCommandSender(backend)

    for network in NETWORKS:
        response = client.get_address(
            path=PATH_TESTNET if network is NETWORK_TESTNET else PATH_MAINNET,
            network=network,
        ).data

        address_len, address = unpack_get_address_response(response)

        assert address_len == 34
        assert address[0] == (ord("D") if network is NETWORK_TESTNET else ord("S"))


# In this test we check that the GET_ADDRESS throws on unsupported networks
def test_get_address_unsupported_network(backend):
    client = SolarCommandSender(backend)
    no_confirm = 0x00
    unsupported_network = 0x3E

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA, ins=InsType.GET_ADDRESS, p1=no_confirm, p2=unsupported_network
        )
    assert e.value.status == Errors.SW_WRONG_P1P2


# In this test we check that the GET_ADDRESS works in confirmation mode
def test_get_address_confirm_accepted(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    with client.get_address_with_confirmation(
        path=PATH_MAINNET, network=NETWORK_MAINNET
    ):
        # if firmware.device.startswith("nano"):
        navigator.navigate_until_text_and_compare(
            NavInsID.RIGHT_CLICK,
            [NavInsID.BOTH_CLICK],
            "Approve",
            ROOT_SCREENSHOT_PATH,
            test_name,
        )
        # else:
        #     instructions = [
        #         NavInsID.USE_CASE_REVIEW_TAP,
        #         NavIns(NavInsID.TOUCH, (200, 335)),
        #         NavInsID.USE_CASE_ADDRESS_CONFIRMATION_EXIT_QR,
        #         NavInsID.USE_CASE_ADDRESS_CONFIRMATION_TAP,
        #         NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CONFIRM,
        #         NavInsID.USE_CASE_STATUS_DISMISS
        #     ]
        #     navigator.navigate_and_compare(ROOT_SCREENSHOT_PATH,
        #                                 test_name,
        #                                 instructions)
    response = client.get_async_response().data
    address_len, address = unpack_get_address_response(response)

    assert address_len == 34
    assert address[0] == ord("S")


# In this test we check that the GET_ADDRESS in confirmation mode replies an error if the user refuses
def test_get_address_confirm_refused(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    # if firmware.device.startswith("nano"):
    with pytest.raises(ExceptionRAPDU) as e:
        with client.get_address_with_confirmation(
            path=PATH_MAINNET, network=NETWORK_MAINNET
        ):
            navigator.navigate_until_text_and_compare(
                NavInsID.RIGHT_CLICK,
                [NavInsID.BOTH_CLICK],
                "Reject",
                ROOT_SCREENSHOT_PATH,
                test_name,
            )
    # Assert that we have received a refusal
    assert e.value.status == Errors.SW_DENY
    assert len(e.value.data) == 0
    # else: # stax
    #     instructions_set = [
    #         [
    #             NavInsID.USE_CASE_REVIEW_REJECT,
    #             NavInsID.USE_CASE_STATUS_DISMISS
    #         ],
    #         [
    #             NavInsID.USE_CASE_REVIEW_TAP,
    #             NavInsID.USE_CASE_ADDRESS_CONFIRMATION_CANCEL,
    #             NavInsID.USE_CASE_STATUS_DISMISS
    #         ]
    #     ]
    #     for i, instructions in enumerate(instructions_set):
    #         with pytest.raises(ExceptionRAPDU) as e:
    #             with client.get_public_key_with_confirmation(path=path):
    #                 navigator.navigate_and_compare(ROOT_SCREENSHOT_PATH,
    #                                                test_name + f"/part{i}",
    #                                                instructions)
    #         # Assert that we have received a refusal
    #         assert e.value.status == Errors.SW_DENY
    #         assert len(e.value.data) == 0
