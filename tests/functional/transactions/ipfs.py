from typing import Union

from application_client.solar_utils import UINT64_MAX
from application_client.solar_transaction import Transaction, TransactionError


class Ipfs(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        ipfs: Union[str, bytes],
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            1, 5, nonce, senderPkey, fee, memo, network, version, startingByte
        )
        self.ipfs: bytes = bytes.fromhex(ipfs) if isinstance(ipfs, str) else ipfs

        if len(self.ipfs) != self.ipfs[1] + 2:
            raise TransactionError(f"Bad ipfs: '{self.ipfs}'!")

    def serialise(self) -> bytes:
        return super().serialise() + b"".join([self.ipfs])
