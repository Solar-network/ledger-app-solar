from typing import Union, List

from application_client.solar_utils import write_varint, UINT64_MAX
from application_client.solar_transaction import Transaction, TransactionError


class Transfer(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        addresses: List[Union[bytes, str]],
        amounts: List[int],
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            1, 6, nonce, senderPkey, fee, memo, network, version, startingByte
        )
        self.addresses = []
        self.amounts = []
        for address in addresses:
            if len(address) != 42:
                raise TransactionError(f"Bad address: '{address}'!")
            self.addresses.append(
                bytes.fromhex(address) if isinstance(address, str) else address
            )

        for amount in amounts:
            if not (0 <= amount <= UINT64_MAX):
                raise TransactionError(f"Bad amount: '{amount}'!")
            self.amounts.append(amount)

        if len(self.addresses) != len(self.amounts) or len(self.amounts) < 1:
            raise TransactionError(f"Wrong number of recipients!")

    def serialise(self) -> bytes:
        asset = b""
        for i in range(len(self.addresses)):
            asset += self.amounts[i].to_bytes(8, byteorder="little")
            asset += self.addresses[i]
        return super().serialise() + b"".join(
            [len(self.addresses).to_bytes(2, byteorder="little"), asset]
        )
