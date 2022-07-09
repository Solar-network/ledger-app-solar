import struct
from typing import Tuple

from client.client_interface import ClientInterface
from speculos.client import ApduException

from client.cmd_builder import CommandBuilder, InsType
from client.exception import DeviceException
from client.transaction import Transaction


class Command:
    def __init__(self, transport: ClientInterface, debug: bool = False) -> None:
        self.transport = transport
        self.builder = CommandBuilder(debug=debug)
        self.debug = debug

    def get_app_and_version(self) -> Tuple[str, str]:
        try:
            response = self.transport.apdu_exchange_raw(
                self.builder.get_app_and_version()
            )  # type: int, bytes
        except ApduException as error:
            raise DeviceException(error_code=error.sw, ins=0x01)

        # response = format_id (1) ||
        #            app_name_len (1) ||
        #            app_name (var) ||
        #            version_len (1) ||
        #            version (var) ||
        offset: int = 0

        format_id: int = response[offset]
        offset += 1
        app_name_len: int = response[offset]
        offset += 1
        app_name: str = response[offset : offset + app_name_len].decode("ascii")
        offset += app_name_len
        version_len: int = response[offset]
        offset += 1
        version: str = response[offset : offset + version_len].decode("ascii")
        offset += version_len

        return app_name, version

    def get_version(self) -> Tuple[int, int, int]:
        try:
            response = self.transport.apdu_exchange_raw(
                self.builder.get_version()
            )  # type: int, bytes
        except ApduException as error:
            raise DeviceException(error_code=error.sw, ins=InsType.INS_GET_VERSION)

        # response = MAJOR (1) || MINOR (1) || PATCH (1)
        assert len(response) == 3

        major, minor, patch = struct.unpack("BBB", response)  # type: int, int, int

        return major, minor, patch

    def get_app_name(self) -> str:
        try:
            response = self.transport.apdu_exchange_raw(
                self.builder.get_app_name()
            )  # type: int, bytes
        except ApduException as error:
            raise DeviceException(error_code=error.sw, ins=InsType.INS_GET_APP_NAME)

        return response.decode("ascii")

    def get_public_key(
        self,
        bip32_path: str,
        display: int = False,
        chaincode: int = False,
        n_screens: int = 0,
    ) -> Tuple[bytes, bytes]:
        try:
            response = self.transport.apdu_exchange_raw_with_buttons(
                self.builder.get_public_key(
                    bip32_path=bip32_path, display=display, chaincode=chaincode
                ),
                n_screens,
            )  # type: int, bytes
        except ApduException as error:
            raise DeviceException(error_code=error.sw, ins=InsType.INS_GET_PUBLIC_KEY)

        # response = pub_key_len (1) ||
        #            pub_key (var) ||
        #            chain_code_len (1) ||
        #            chain_code (var)
        offset: int = 0

        pub_key_len: int = response[offset]
        offset += 1
        pub_key: bytes = response[offset : offset + pub_key_len]
        offset += pub_key_len

        chain_code_len: int = 0
        chain_code: bytes = {}

        if chaincode:
            chain_code_len = response[offset]
            offset += 1
            chain_code = response[offset : offset + chain_code_len]
            offset += chain_code_len

        assert len(response) == 1 + pub_key_len + (
            (1 + chain_code_len) if chaincode else 0
        )

        return pub_key, chain_code

    def get_address(
        self,
        bip32_path: str,
        display: int = False,
        network: int = 0,
        n_screens: int = 0,
    ) -> Tuple[bytes, bytes]:
        try:
            response = self.transport.apdu_exchange_raw_with_buttons(
                self.builder.get_address(
                    bip32_path=bip32_path, display=display, network=network
                ),
                n_screens,
            )  # type: int, bytes
        except ApduException as error:
            raise DeviceException(error_code=error.sw, ins=InsType.INS_GET_ADDRESS)

        # response = address_len(1), address_string(34)
        offset: int = 0

        address_len: int = response[offset]
        offset += 1
        address: bytes = response[offset : offset + address_len]
        offset += address_len

        assert len(response) == 1 + address_len

        return address

    def sign_tx(
        self,
        bip32_path: str,
        transaction: Transaction,
        n_screens: int = 0,
    ) -> Tuple[int, bytes]:
        response: bytes = b""

        for is_last, chunk in self.builder.sign_tx(
            bip32_path=bip32_path, transaction=transaction
        ):
            try:
                response = self.transport.apdu_exchange_raw_with_buttons(
                    chunk,
                    n_screens if is_last else 0,
                )  # type: int, bytes
            except ApduException as error:
                raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_TX)

        # response = schnorr_sig_len (1) ||
        #            schnorr_sig (var) ||
        #            v (1)

        schnorr_sig: bytes = response

        assert len(response) == 64

        return schnorr_sig

    def sign_message(
        self, bip32_path: str, message: str, n_screens: int = 0
    ) -> Tuple[int, bytes]:
        response: bytes = b""

        for is_last, chunk in self.builder.sign_message(
            bip32_path=bip32_path, message=message
        ):
            try:
                response = self.transport.apdu_exchange_raw_with_buttons(
                    chunk,
                    n_screens if is_last else 0,
                )  # type: int, bytes
            except ApduException as error:
                raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_MESSAGE)

        # response = schnorr_sig_len (1) ||
        #            schnorr_sig (var) ||
        #            v (1)

        schnorr_sig: bytes = response

        assert len(response) == 64

        return schnorr_sig
