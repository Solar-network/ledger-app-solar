from typing import Union

from client.utils import UINT64_MAX, UINT32_MAX, UINT8_MAX
from client.transaction import Transaction, TransactionError


class HtlcLock(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        amount: int,
        secretHash: Union[str, bytes],
        expirationType: int,
        expirationValue: int,
        recipientId: Union[str, bytes],
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            1, 8, nonce, senderPkey, fee, memo, network, version, startingByte
        )
        self.amount: int = amount
        self.secretHash: bytes = (
            bytes.fromhex(secretHash) if isinstance(secretHash, str) else secretHash
        )
        self.expirationType: int = expirationType
        self.expirationValue: int = expirationValue
        self.recipientId: bytes = (
            bytes.fromhex(recipientId) if isinstance(recipientId, str) else recipientId
        )

        if not (0 <= self.amount <= UINT64_MAX):
            raise TransactionError(f"Bad amount: '{self.amount}'!")

        if len(self.secretHash) > UINT8_MAX:
            raise TransactionError(f"Bad secretHash: '{self.secretHash}'!")

        if not (0 <= self.expirationType <= UINT8_MAX):
            raise TransactionError(f"Bad expirationType: '{self.expirationType}'!")

        if not (0 <= self.expirationValue <= UINT32_MAX):
            raise TransactionError(f"Bad expirationValue: '{self.expirationValue}'!")

        if len(self.recipientId) != 21:
            raise TransactionError(f"Bad recipientId: '{self.recipientId}'!")

    def serialise(self) -> bytes:
        return super().serialise() + b"".join(
            [
                self.amount.to_bytes(8, byteorder="little"),
                len(self.secretHash).to_bytes(1, byteorder="little"),
                self.secretHash,
                self.expirationType.to_bytes(1, byteorder="little"),
                self.expirationValue.to_bytes(4, byteorder="little"),
                self.recipientId,
            ]
        )
