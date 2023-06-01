from typing import Union, List

from application_client.solar_utils import UINT16_MAX
from application_client.solar_transaction import Transaction, TransactionError


class Vote(Transaction):
    def __init__(
        self,
        nonce: int,
        senderPkey: Union[str, bytes],
        fee: int,
        memo: str,
        votes: List[List[Union[int, str]]],
        network: int = 63,
        version: int = 3,
        startingByte: int = 0xFF,
    ) -> None:
        super().__init__(
            2, 2, nonce, senderPkey, fee, memo, network, version, startingByte
        )
        self.votes = []
        total_percentage = 0
        for vote in votes:
            username = vote[0]
            percentage = vote[1]
            if len(username) < 1 or len(username) > 20:
                raise TransactionError(f"Bad username: '{username}'!")
            if not (0 <= percentage <= UINT16_MAX):
                raise TransactionError(f"Bad percentage: '{percentage}'!")
            total_percentage += percentage
            self.votes.append(vote)
        # if total_percentage != 10000:
        #    raise TransactionError(
        #        f"Total of percentage is'{total_percentage} != 10000'!"
        #    )
        if len(self.votes) > 53:
            raise TransactionError(f"Wrong total number of votes: '{len(self.votes)}'!")

    def serialise(self) -> bytes:
        asset = b""
        for vote in self.votes:
            asset += len(vote[0]).to_bytes(1, byteorder="little")
            asset += vote[0].encode("ascii")
            asset += vote[1].to_bytes(2, byteorder="little")
        return super().serialise() + b"".join(
            [len(self.votes).to_bytes(1, byteorder="little"), asset]
        )
