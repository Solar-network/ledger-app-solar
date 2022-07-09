from typing import Union

from client.utils import UINT8_MAX
from client.transaction import Transaction, TransactionError


class HtlcClaim(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        hashType: int,
        lockId: Union[str, bytes],
        unlockSecret: Union[str, bytes],
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            1, 9, nonce, senderPkey, fee, memo, network, version, startingByte
        )

        self.hashType: int = hashType

        self.lockId: bytes = (
            bytes.fromhex(lockId) if isinstance(lockId, str) else lockId
        )
        self.unlockSecret: bytes = (
            bytes.fromhex(unlockSecret)
            if isinstance(unlockSecret, str)
            else unlockSecret
        )

        if not (0 <= self.hashType <= UINT8_MAX):
            raise TransactionError(f"Bad amount: '{self.amount}'!")

        if len(self.lockId) != 32:
            raise TransactionError(f"Bad lockId: '{self.lockId}'!")

        if len(self.unlockSecret) > UINT8_MAX:
            raise TransactionError(f"Bad unlockSecret: '{self.unlockSecret}'!")

    def serialise(self) -> bytes:
        return super().serialise() + b"".join(
            [
                self.hashType.to_bytes(1, byteorder="little"),
                self.lockId,
                len(self.unlockSecret).to_bytes(1, byteorder="little"),
                self.unlockSecret,
            ]
        )
