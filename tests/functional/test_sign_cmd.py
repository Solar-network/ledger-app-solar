from btclib.ecc import ssa
import pytest
from client.exception import *

from speculos.client import ApduException
from client.cmd_builder import InsType

from client.transaction import Transaction
from transactions.multi_signature_registration import Multisignature
from transactions.ipfs import Ipfs
from transactions.transfer import Transfer
from transactions.htlc_lock import HtlcLock
from transactions.htlc_refund import HtlcRefund
from transactions.htlc_claim import HtlcClaim
from transactions.burn import Burn
from transactions.vote import Vote


@pytest.fixture(autouse=True)
def context(cmd):
    bip32_path: str = "44'/3333'/0'/0/0"

    pub_key, chain_code = cmd.get_public_key(
        bip32_path=bip32_path, display=0, chaincode=0
    )

    context = {"pkey": pub_key, "chain_code": chain_code, "bip32": bip32_path}
    yield context


def test_transfer_tx(cmd, context):

    tx = Transfer(
        1,
        context["pkey"],
        5645365,
        "This is a test memo.",
        ["3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
        [123456],
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=7)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True

    tx = Transfer(
        1,
        context["pkey"],
        5645365,
        "",
        [
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
            "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
        ],
        [
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
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=60)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_transfer_limit(cmd, context):

    tx = Transfer(
        1,
        context["pkey"],
        5645365,
        "",
        ["3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3" for _ in range(41)],
        [1 for _ in range(41)],
    )
    cmd.sign_tx(bip32_path=context["bip32"], transaction=tx)


def test_burn_tx(cmd, context):

    tx = Burn(3, context["pkey"], 0, "", 250000000000)
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=3)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


def test_htlc_lock_tx(cmd, context):
    tx = HtlcLock(
        4,
        context["pkey"],
        645365,
        "",
        123456789,
        "9c1a3815d49e0c9f78b872bfb017e825ea2db708158b70815526a830c85912b4",
        1,
        1234,
        "3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3",
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=11)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


def test_htlc_claim_tx(cmd, context):

    tx = HtlcClaim(
        5,
        context["pkey"],
        645365,
        "",
        0,
        "cfb37ec05433841cd8d25c3e8d353dc519a6dcb583edd75085b671bff4075b40",
        "c27f1ce845d8c29eebc9006be932b604fd06755521b1a8b0be4204c65377151a",
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=10)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


def test_htlc_refund_tx(cmd, context):

    tx = HtlcRefund(
        6,
        context["pkey"],
        645365,
        "",
        "cfb37ec05433841cd8d25c3e8d353dc519a6dcb583edd75085b671bff4075b40",
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=6)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


def test_ipfs_tx(cmd, context):

    tx = Ipfs(
        7,
        context["pkey"],
        645365,
        "",
        "122040e8c8cc86493d35f01b603190499ad4046cf2d097f5d34de034cebbba904804",
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=5)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


def test_multisignature_tx(cmd, context):

    tx = Multisignature(
        8,
        context["pkey"],
        645365,
        "",
        2,
        [
            "03e5ea82082ea33c9956ec03109118c7d588f82eb7a0c3c1794de67103bb0c85f9",
            "022f69d427310705c3e02ebb5519bcd545f41a2eed543660fafe0e7191930eb747",
            "03d39fb4797d0c428bebed6d80203e2273a9fdbcaafe0b29761ef3183c2151e211",
        ],
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=15)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


def test_vote_tx(cmd, context):

    tx = Vote(
        2,
        context["pkey"],
        10000000,
        "",
        [["gym", 2000], ["cactus1549", 4000], ["sl33p", 4000]],
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=8)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True

    tx = Vote(
        2,
        context["pkey"],
        10000000,
        "",
        [],
    )
    schnorr_sig = cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=2)
    assert ssa.verify(tx.serialise(), context["pkey"][1:], schnorr_sig) is True


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def vote_sum_equal_100(cmd, context):
    try:
        tx = Vote(
            2,
            context["pkey"],
            10000000,
            "",
            [["gym", 2001], ["cactus1549", 4000], ["sl33p", 4000]],
        )
        cmd.sign_tx(bip32_path=context["bip32"], transaction=tx, n_screens=2)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_accept_v3_only(cmd, context):
    try:
        tx = Transfer(
            1,
            context["pkey"],
            5645365,
            "",
            ["3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
            [123456],
            version=2,
        )
        cmd.sign_tx(bip32_path=context["bip32"], transaction=tx)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_network_byte(cmd, context):
    try:
        tx = Transfer(
            1,
            context["pkey"],
            5645365,
            "",
            ["3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
            [123456],
            network=62,
        )
        cmd.sign_tx(bip32_path=context["bip32"], transaction=tx)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_starting_byte(cmd, context):
    try:
        tx = Transfer(
            1,
            context["pkey"],
            5645365,
            "",
            ["3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
            [123456],
            startingByte=0xFE,
        )
        cmd.sign_tx(bip32_path=context["bip32"], transaction=tx)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_memo_encoding(cmd, context):
    try:
        tx = Transfer(
            1,
            context["pkey"],
            5645365,
            "eco\nuter",
            ["3fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3"],
            [123456],
        )
        cmd.sign_tx(bip32_path=context["bip32"], transaction=tx)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_wrong_typegroup(cmd, context):
    try:
        tx = Transaction(3, 0, 13, context["pkey"], 2000000, "")
        cmd.sign_tx(bip32_path=context["bip32"], transaction=tx)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_wrong_type(cmd, context):
    try:
        tx = Transaction(1, 20, 13, context["pkey"], 2000000, "")
        cmd.sign_tx(bip32_path=context["bip32"], transaction=tx)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_wrong_memo_length(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC2,
            p1=0x00,
            p2=0x80,
            cdata=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC2,
            p1=0x01,
            p2=0x00,
            cdata=bytes.fromhex(
                "ff033f010000000600010000000000000002937e1b9294d07c91b46d511851d698ad606a64950dd3a332fcfac9c0a3fb0bad352456000000000005616161616161010040e20100000000003fc91327b917bf2b464e9b8f1acf0588f4cb6e7bb3"
            ),
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)
