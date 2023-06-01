from typing import Union

from application_client.solar_utils import UINT64_MAX
from application_client.solar_transaction import Transaction, TransactionError


class Burn(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        amount: int,
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            2, 0, nonce, senderPkey, fee, memo, network, version, startingByte
        )
        self.amount: int = amount

        if not (0 <= self.amount <= UINT64_MAX):
            raise TransactionError(f"Bad amount: '{self.amount}'!")

    def serialise(self) -> bytes:
        return super().serialise() + b"".join(
            [
                self.amount.to_bytes(8, byteorder="little"),
            ]
        )
