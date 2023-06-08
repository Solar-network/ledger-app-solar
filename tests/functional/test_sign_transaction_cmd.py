import pytest

from ragger.error import ExceptionRAPDU
from ragger.navigator import NavInsID, NavIns
from utils import ROOT_SCREENSHOT_PATH

from btclib.ecc import ssa

from application_client.solar_command_sender import (
    CLA,
    InsType,
    P1,
    P2,
    MAX_APDU_LEN,
    SolarCommandSender,
    split_message,
    Errors,
)
from application_client.solar_response_unpacker import (
    unpack_get_public_key_response,
)
from transactions.transfer import Transfer
from transactions.vote import Vote
from transactions.burn import Burn
from transactions.ipfs import Ipfs
from application_client.solar_transaction import (
    Transaction,
)
from constants import PATH_MAINNET


# Check the behaviour of the device when asked to sign an ipfs transaction.
# TypeGroup 1, Type 5
def test_sign_transaction_ipfs(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    ipfs_transaction = Ipfs(
        nonce=1,
        senderPkey=public_key,
        fee=645365,
        memo="",
        ipfs="122040e8c8cc86493d35f01b603190499ad4046cf2d097f5d34de034cebbba904804",
    )

    with client.sign_transaction(path=PATH_MAINNET, transaction=ipfs_transaction):
        # if firmware.device.startswith("nano"):
        navigator.navigate_until_text_and_compare(
            NavInsID.RIGHT_CLICK,
            [NavInsID.BOTH_CLICK],
            "Approve",
            ROOT_SCREENSHOT_PATH,
            test_name,
        )
        # else:
        #     navigator.navigate_until_text_and_compare(NavInsID.USE_CASE_REVIEW_TAP,
        #                                               [NavInsID.USE_CASE_REVIEW_CONFIRM,
        #                                                NavInsID.USE_CASE_STATUS_DISMISS],
        #                                               "Hold to sign",
        #                                               ROOT_SCREENSHOT_PATH,
        #                                               test_name)
    response = client.get_async_response().data

    assert ssa.verify(ipfs_transaction.serialise(), public_key, response) is True


# Check the behaviour of the device when asked to sign a transfer transaction.
# TypeGroup 1, Type 6
def test_sign_transaction_transfer(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transfer_transaction = Transfer(
        nonce=2,
        senderPkey=public_key,
        fee=5645365,
        memo="This is an optional memo.",
        addresses=[
            "3f091327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f192327b917bf2b464e9b8f1acf1588f4cb6e7bb3",
            "3f293327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f394327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f495327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f596327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f697327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f798327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f899327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3f990327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fa91427b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fb91527b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91627b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fd91727b917bf2b464e9b8f1acf0588f4cb6e7bb3",
        ],
        amounts=[
            123456,
            1234567,
            12345678,
            123456789,
            1234567890,
            12345678900,
            123456789000,
            1234567890000,
            12345678900000,
            123456789000000,
            1234567890000000,
            12345678900000000,
            123456789000000000,
            18446744073709551615,
        ],
    )

    with client.sign_transaction(path=PATH_MAINNET, transaction=transfer_transaction):
        # if firmware.device.startswith("nano"):
        navigator.navigate_until_text_and_compare(
            NavInsID.RIGHT_CLICK,
            [NavInsID.BOTH_CLICK],
            "Approve",
            ROOT_SCREENSHOT_PATH,
            test_name,
        )
        # else:
        #     navigator.navigate_until_text_and_compare(NavInsID.USE_CASE_REVIEW_TAP,
        #                                               [NavInsID.USE_CASE_REVIEW_CONFIRM,
        #                                                NavInsID.USE_CASE_STATUS_DISMISS],
        #                                               "Hold to sign",
        #                                               ROOT_SCREENSHOT_PATH,
        #                                               test_name)
    response = client.get_async_response().data

    assert ssa.verify(transfer_transaction.serialise(), public_key, response) is True


# Check the behaviour of the device when asked to sign a burn transaction.
# TypeGroup 2, Type 0
def test_sign_transaction_burn(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    burn_transaction = Burn(
        nonce=3,
        senderPkey=public_key,
        fee=10000000,
        memo="",
        amount=250000000000,
    )

    with client.sign_transaction(path=PATH_MAINNET, transaction=burn_transaction):
        # if firmware.device.startswith("nano"):
        navigator.navigate_until_text_and_compare(
            NavInsID.RIGHT_CLICK,
            [NavInsID.BOTH_CLICK],
            "Approve",
            ROOT_SCREENSHOT_PATH,
            test_name,
        )
        # else:
        #     navigator.navigate_until_text_and_compare(NavInsID.USE_CASE_REVIEW_TAP,
        #                                               [NavInsID.USE_CASE_REVIEW_CONFIRM,
        #                                                NavInsID.USE_CASE_STATUS_DISMISS],
        #                                               "Hold to sign",
        #                                               ROOT_SCREENSHOT_PATH,
        #                                               test_name)
    response = client.get_async_response().data

    assert ssa.verify(burn_transaction.serialise(), public_key, response) is True


# Check the behaviour of the device when asked to sign a vote transaction.
# TypeGroup 2, Type 2
def test_sign_transaction_vote(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    vote_transaction = Vote(
        nonce=4,
        senderPkey=public_key,
        fee=10000000,
        memo="",
        votes=[["gym", 2000], ["cactus1549", 4000], ["sl33p", 4000]],
    )

    with client.sign_transaction(path=PATH_MAINNET, transaction=vote_transaction):
        # if firmware.device.startswith("nano"):
        navigator.navigate_until_text_and_compare(
            NavInsID.RIGHT_CLICK,
            [NavInsID.BOTH_CLICK],
            "Approve",
            ROOT_SCREENSHOT_PATH,
            test_name,
        )
        # else:
        #     navigator.navigate_until_text_and_compare(NavInsID.USE_CASE_REVIEW_TAP,
        #                                               [NavInsID.USE_CASE_REVIEW_CONFIRM,
        #                                                NavInsID.USE_CASE_STATUS_DISMISS],
        #                                               "Hold to sign",
        #                                               ROOT_SCREENSHOT_PATH,
        #                                               test_name)
    response = client.get_async_response().data

    assert ssa.verify(vote_transaction.serialise(), public_key, response) is True


# Check the behaviour of the device when asked to sign a cancel vote transaction.
# TypeGroup 2, Type 2
def test_sign_transaction_vote_cancel(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    cancel_vote_transaction = Vote(
        nonce=5,
        senderPkey=public_key,
        fee=10000000,
        memo="",
        votes=[],
    )

    with client.sign_transaction(path=PATH_MAINNET, transaction=cancel_vote_transaction):
        # if firmware.device.startswith("nano"):
        navigator.navigate_until_text_and_compare(
            NavInsID.RIGHT_CLICK,
            [NavInsID.BOTH_CLICK],
            "Approve",
            ROOT_SCREENSHOT_PATH,
            test_name,
        )
        # else:
        #     navigator.navigate_until_text_and_compare(NavInsID.USE_CASE_REVIEW_TAP,
        #                                               [NavInsID.USE_CASE_REVIEW_CONFIRM,
        #                                                NavInsID.USE_CASE_STATUS_DISMISS],
        #                                               "Hold to sign",
        #                                               ROOT_SCREENSHOT_PATH,
        #                                               test_name)
    response = client.get_async_response().data

    assert ssa.verify(cancel_vote_transaction.serialise(), public_key, response) is True


# Check the behaviour of the device when the transfer uses an invalid typegroup.
def test_sign_transaction_type_group_invalid(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transaction = Transaction(
        typeGroup=3,
        type=0,
        nonce=6,
        senderPkey=public_key,
        fee=2000000,
        memo="",
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=transaction.serialise(),
        )
    assert e.value.status == Errors.SW_TX_PARSING_FAIL


# Check the behaviour of the device when the transfer uses an invalid type.
def test_sign_transaction_type_invalid(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transaction = Transaction(
        typeGroup=20,
        type=20,
        nonce=6,
        senderPkey=public_key,
        fee=2000000,
        memo="",
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=transaction.serialise(),
        )
    assert e.value.status == Errors.SW_TX_PARSING_FAIL

# Check the behaviour of the device when the transfer limit is exceeded.
def test_sign_transfer_limit_exceeded(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transfer_transaction = Transfer(
        nonce=2,
        senderPkey=public_key,
        fee=5645365,
        memo="",
        addresses=["3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3" for _ in range(41)],
        amounts=[1 for _ in range(41)],
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )
        chunks = split_message(transfer_transaction.serialise(), MAX_APDU_LEN)
        idx: int = P1.P1_START + 1

        for chunk in chunks:
            backend.exchange(
                cla=CLA, ins=InsType.SIGN_TX, p1=idx, p2=P2.P2_MORE, data=chunk
            )
            idx += 1

    assert e.value.status == Errors.SW_WRONG_TX_LENGTH


# Check the behaviour of the device when the transfer uses an unsupported version.
def test_sign_transfer_version_unsupported(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transfer_transaction = Transfer(
        version=2,
        nonce=2,
        senderPkey=public_key,
        fee=5645365,
        memo="",
        addresses=["3f091327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
        amounts=[123456],
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=transfer_transaction.serialise(),
        )
    assert e.value.status == Errors.SW_TX_PARSING_FAIL


# Check the behaviour of the device when the transfer uses an unsupported network.
def test_sign_transfer_network_unsupported(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transfer_transaction = Transfer(
        network=62,
        nonce=2,
        senderPkey=public_key,
        fee=5645365,
        memo="",
        addresses=["3f091327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
        amounts=[123456],
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=transfer_transaction.serialise(),
        )
    assert e.value.status == Errors.SW_TX_PARSING_FAIL


# Check the behaviour of the device when the transfer uses an invalid starting byte.
def test_sign_transfer_starting_byte_invalid(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transfer_transaction = Transfer(
        startingByte=0xFE,
        nonce=2,
        senderPkey=public_key,
        fee=5645365,
        memo="",
        addresses=["3f091327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
        amounts=[123456],
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=transfer_transaction.serialise(),
        )
    assert e.value.status == Errors.SW_TX_PARSING_FAIL


# Check the behaviour of the device when the transfer uses an invalid memo encoding.
def test_sign_transfer_memo_invalid(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    transfer_transaction = Transfer(
        nonce=2,
        senderPkey=public_key,
        fee=5645365,
        memo="this\nis\nnot\nvalid",
        addresses=["3f091327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
        amounts=[123456],
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=transfer_transaction.serialise(),
        )
    assert e.value.status == Errors.SW_TX_PARSING_FAIL


# Check the behaviour of the device when the data length is invalid.
def test_transaction_data_length_invalid(backend):
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=bytes.fromhex(
                "ff033f010000000600010000000000000002937e1b9294d07c91b46d511851d698ad606a64950dd3a332fcfac9c0a3fb0bad352456000000000005616161616161010040e20100000000003fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3"
            ),
        )
    assert e.value.status == Errors.SW_WRONG_DATA_LENGTH


# Check the behaviour of the device when the transfer uses an invalid vote sum.
def test_sign_transfer_vote_sum_invalid(firmware, backend, navigator, test_name):
    client = SolarCommandSender(backend)

    rapdu = client.get_public_key(path=PATH_MAINNET)
    _, public_key = unpack_get_public_key_response(rapdu.data)

    vote_transaction = Vote(
        nonce=6,
        senderPkey=public_key,
        fee=10000000,
        memo="",
        votes=[["gym", 2001], ["cactus1549", 4000], ["sl33p", 4000]],
    )

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START + 1,
            p2=P2.P2_LAST,
            data=vote_transaction.serialise(),
        )
    assert e.value.status == Errors.SW_TX_PARSING_FAIL
