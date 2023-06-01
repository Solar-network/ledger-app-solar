from typing import Union

from application_client.solar_utils import write_varint, UINT64_MAX, UINT32_MAX, UINT16_MAX, UINT8_MAX


class TransactionError(Exception):
    pass


class Transaction:
    def __init__(
        self,
        typeGroup: int,
        type: int,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        self.nonce: int = nonce
        self.typeGroup: int = typeGroup
        self.type: int = type
        self.senderPkey: bytes = (
            bytes.fromhex(senderPkey) if isinstance(senderPkey, str) else senderPkey
        )
        self.fee: int = fee
        self.memo: bytes = memo.encode("ascii")
        self.network: int = network
        self.version: int = version
        self.startingByte: int = startingByte

        if not (0 <= self.network <= UINT8_MAX):
            raise TransactionError(f"Bad network: '{self.network}'!")

        if not (0 <= self.version <= UINT8_MAX):
            raise TransactionError(f"Bad version: '{self.version}'!")

        if not (0 <= self.typeGroup <= UINT32_MAX):
            raise TransactionError(f"Bad typeGroup: '{self.typeGroup}'!")

        if not (0 <= self.type <= UINT16_MAX):
            raise TransactionError(f"Bad type: '{self.type}'!")

        if not (0 <= self.nonce <= UINT64_MAX):
            raise TransactionError(f"Bad nonce: '{self.nonce}'!")

        if not (0 <= self.fee <= UINT64_MAX):
            raise TransactionError(f"Bad fee: '{self.fee}'!")

        if len(self.senderPkey) != 33:
            raise TransactionError(f"Bad senderPkey: '{self.senderPkey}'!")

        if len(self.memo) > 255:
            raise TransactionError(f"Bad memo: '{self.memo}'!")

    def serialise(self) -> bytes:
        return b"".join(
            [
                self.startingByte.to_bytes(1, byteorder="little"),
                self.version.to_bytes(1, byteorder="little"),
                self.network.to_bytes(1, byteorder="little"),
                self.typeGroup.to_bytes(4, byteorder="little"),
                self.type.to_bytes(2, byteorder="little"),
                self.nonce.to_bytes(8, byteorder="little"),
                self.senderPkey,
                self.fee.to_bytes(8, byteorder="little"),
                write_varint(len(self.memo)),
                self.memo,
            ]
        )
