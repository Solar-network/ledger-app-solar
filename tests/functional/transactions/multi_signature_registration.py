from typing import Union, List

from client.utils import write_varint, UINT8_MAX
from client.transaction import Transaction, TransactionError


class Multisignature(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        min: int,
        pkeys: List[Union[bytes, str]],
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            1, 4, nonce, senderPkey, fee, memo, network, version, startingByte
        )
        self.min: int = min
        self.pkeys = []
        for key in pkeys:
            if len(key) != 66:
                raise TransactionError(f"Bad pkeys: '{key}'!")
            self.pkeys.append(bytes.fromhex(key) if isinstance(key, str) else key)

        if not (0 <= self.min <= UINT8_MAX):
            raise TransactionError(f"Bad min: '{self.min}'!")

        if len(self.pkeys) < 1 or len(self.pkeys) > 16:
            raise TransactionError(f"Bad pkeys: '{self.pkeys}'!")

    def serialise(self) -> bytes:
        return super().serialise() + b"".join(
            [
                self.min.to_bytes(1, byteorder="little"),
                write_varint(len(self.pkeys)),
                *self.pkeys,
            ]
        )
