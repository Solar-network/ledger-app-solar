from pathlib import Path
from hashlib import sha256

# from sha3 import keccak_256

from btclib.ecc import ssa

from ecdsa.curves import SECP256k1

# from ecdsa.keys import VerifyingKey
# from ecdsa.util import sigdecode_der


ROOT_SCREENSHOT_PATH = Path(__file__).parent.resolve()


# Check if a signature of a given message is valid
def check_signature_validity(
    public_key: bytes, signature: bytes, message: bytes
) -> bool:
    ssa.verify(message1.encode("ascii"), context["pkey"][1:], schnorr_sig) is True

    pk: VerifyingKey = VerifyingKey.from_string(
        public_key, curve=SECP256k1, hashfunc=sha256
    )
    return pk.verify(
        signature=signature, data=message, hashfunc=sha256, sigdecode=sigdecode_der
    )
