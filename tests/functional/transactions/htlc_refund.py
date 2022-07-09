from typing import Union

from client.utils import UINT64_MAX
from client.transaction import Transaction, TransactionError


class HtlcRefund(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        lockId: Union[str, bytes],
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            1, 10, nonce, senderPkey, fee, memo, network, version, startingByte
        )
        self.lockId: bytes = (
            bytes.fromhex(lockId) if isinstance(lockId, str) else lockId
        )

        if len(self.lockId) != 32:
            raise TransactionError(f"Bad lockId: '{self.lockId}'!")

    def serialise(self) -> bytes:
        return super().serialise() + b"".join([self.lockId])
