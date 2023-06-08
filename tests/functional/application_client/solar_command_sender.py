from enum import IntEnum
from typing import Generator, List, Optional, Union
from contextlib import contextmanager

from application_client.solar_transaction import Transaction

# from application_client.solar_utils import bip32_path_from_string

from ragger.backend.interface import BackendInterface, RAPDU
from ragger.bip import pack_derivation_path


MAX_APDU_LEN: int = 255

CLA: int = 0xE0


class P1(IntEnum):
    # Parameter 1 for first APDU number.
    P1_START = 0x00
    # Parameter 1 for maximum APDU number.
    P1_MAX = 0x03
    # Parameter 1 for screen confirmation for GET_PUBLIC_KEY.
    P1_CONFIRM = 0x01


class P2(IntEnum):
    # Parameter 2 for last APDU to receive.
    P2_LAST = 0x00
    # Parameter 2 for more APDU to receive.
    P2_MORE = 0x80


class InsType(IntEnum):
    GET_APP_NAME = 0xA1
    GET_VERSION = 0xA2
    GET_PUBLIC_KEY = 0xB1
    GET_ADDRESS = 0xB2
    SIGN_MESSAGE = 0xC1
    SIGN_TX = 0xC2


class Errors(IntEnum):
    SW_DENY = 0x6985
    SW_WRONG_P1P2 = 0x6A86
    SW_WRONG_DATA_LENGTH = 0x6A87
    SW_INS_NOT_SUPPORTED = 0x6D00
    SW_CLA_NOT_SUPPORTED = 0x6E00
    SW_WRONG_RESPONSE_LENGTH = 0xB000
    SW_DISPLAY_BIP32_PATH_FAIL = 0xB001
    SW_DISPLAY_ADDRESS_FAIL = 0xB002
    SW_DISPLAY_AMOUNT_FAIL = 0xB003
    SW_WRONG_TX_LENGTH = 0xB004
    SW_TX_PARSING_FAIL = 0xB005
    SW_TX_HASH_FAIL = 0xB006
    SW_BAD_STATE = 0xB007
    SW_SIGNATURE_FAIL = 0xB008


def split_message(message: bytes, max_size: int) -> List[bytes]:
    return [message[x : x + max_size] for x in range(0, len(message), max_size)]


class SolarCommandSender:
    def __init__(self, backend: BackendInterface) -> None:
        self.backend = backend

    def serialise(
        self,
        cla: int,
        ins: Union[int, IntEnum],
        p1: int = 0,
        p2: int = 0,
        data: bytes = b"",
    ) -> bytes:

        ins = cast(int, ins.value) if isinstance(ins, IntEnum) else cast(int, ins)

        header: bytes = struct.pack(
            "BBBBB", cla, ins, p1, p2, len(data)
        )  # add Lc to APDU header

        if self.debug:
            logging.info("header: %s", header.hex())
            logging.info("data:  %s", data.hex())

        return header + data

    def get_app_and_version(self) -> RAPDU:
        return self.backend.exchange(
            cla=0xB0,  # specific CLA for BOLOS
            ins=0x01,  # specific INS for get_app_and_version
            p1=P1.P1_START,
            p2=P2.P2_LAST,
            data=b"",
        )

    def get_version(self) -> RAPDU:
        return self.backend.exchange(
            cla=CLA, ins=InsType.GET_VERSION, p1=P1.P1_START, p2=P2.P2_LAST, data=b""
        )

    def get_app_name(self) -> RAPDU:
        return self.backend.exchange(
            cla=CLA, ins=InsType.GET_APP_NAME, p1=P1.P1_START, p2=P2.P2_LAST, data=b""
        )

    # def get_public_key(self, path: str) -> RAPDU:
    def get_public_key(self, path: str, display: int = 0, chaincode: int = 0) -> RAPDU:
        return self.backend.exchange(
            cla=CLA,
            ins=InsType.GET_PUBLIC_KEY,
            p1=display,
            p2=chaincode,
            data=pack_derivation_path(path),
        )

    @contextmanager
    def get_public_key_with_confirmation(self, path: str, chaincode: int = 0) -> RAPDU:
        with self.backend.exchange_async(
            cla=CLA,
            ins=InsType.GET_PUBLIC_KEY,
            p1=P1.P1_CONFIRM,
            p2=chaincode,
            data=pack_derivation_path(path),
        ) as response:
            yield response

    def get_address(self, path: str, display: int = 0, network: int = 0) -> RAPDU:
        return self.backend.exchange(
            cla=CLA,
            ins=InsType.GET_ADDRESS,
            p1=display,
            p2=network,
            data=pack_derivation_path(path),
        )

    @contextmanager
    def get_address_with_confirmation(self, path: str, network: int = 0) -> RAPDU:
        with self.backend.exchange_async(
            cla=CLA,
            ins=InsType.GET_ADDRESS,
            p1=P1.P1_CONFIRM,
            p2=network,
            data=pack_derivation_path(path),
        ) as response:
            yield response

    @contextmanager
    def sign_message(self, path: str, message: str) -> RAPDU:
        msg: bytes = b"".join(
            [len(message).to_bytes(2, byteorder="little"), bytes(message, "ascii")]
        )

        self.backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_MESSAGE,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=pack_derivation_path(path),
        )
        chunks = split_message(msg, MAX_APDU_LEN)
        idx: int = P1.P1_START + 1

        for chunk in chunks[:-1]:
            self.backend.exchange(
                cla=CLA, ins=InsType.SIGN_MESSAGE, p1=idx, p2=P2.P2_MORE, data=chunk
            )
            idx += 1

        with self.backend.exchange_async(
            cla=CLA, ins=InsType.SIGN_MESSAGE, p1=idx, p2=P2.P2_LAST, data=chunks[-1]
        ) as response:
            yield response

    @contextmanager
    def sign_transaction(self, path: str, transaction: Transaction) -> RAPDU:
        tx: bytes = transaction.serialise()

        self.backend.exchange(
            cla=CLA,
            ins=InsType.SIGN_TX,
            p1=P1.P1_START,
            p2=P2.P2_MORE,
            data=pack_derivation_path(path),
        )
        chunks = split_message(tx, MAX_APDU_LEN)
        idx: int = P1.P1_START + 1

        for chunk in chunks[:-1]:
            self.backend.exchange(
                cla=CLA, ins=InsType.SIGN_TX, p1=idx, p2=P2.P2_MORE, data=chunk
            )
            idx += 1

        with self.backend.exchange_async(
            cla=CLA, ins=InsType.SIGN_TX, p1=idx, p2=P2.P2_LAST, data=chunks[-1]
        ) as response:
            yield response

    def get_async_response(self) -> Optional[RAPDU]:
        return self.backend.last_async_response
