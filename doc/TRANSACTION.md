# Solar Transactions

## Overview

[The Solar Network](https://github.com/Solar-network/core) serialises transactions into raw byte streams which are then digested via SHA256 to produce a signing message hash. In the case of the Solar Ledger App, serialised transactions are first sent via APDU as the `CData`, then deserialised for validation and user approval before construction and signing of the message hash. The sections below provide a brief overview of how transactions and their associated values are constructed; for more in-depth information, please visit [Solar's Documentation](https://docs.solar.org).

## Amount units

The Solar Network currency is the SXP token which has a precision of 8 decimals. Solar transactions make use of raw currency values obtained by multiplying the SXP amount by `10^8` as exampled in the table below:

|                                                                                                                       Unit                                                                                                                        |
| :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
| <table><tr> <th>SXP</th> <th>10^8</th> </tr><tr> <td>`1`</td> <td>`100000000`</td> </tr><tr> <td>`25`</td> <td>`2500000000`</td> </tr><tr> <td>`50`</td> <td>`50000000000`</td> </tr><tr> <td>`10000`</td> <td>`1000000000000`</td> </tr></table> |

> `1 SXP => 1.00000000 SXP => 100000000` for raw transactions.

## Address format

Solar addresses are [Base58Check](https://en.bitcoin.it/wiki/Base58Check_encoding) encodings of a [RIPEMD-160](https://en.bitcoin.it/wiki/RIPEMD-160) hash of a Solar PublicKey prepended by a network/version byte.

### Networks

The network/version byte is used to set the leading character of the address and helps to both visually identify and ensure the correct network is being used.

| Network | Dec  |  Hex   | Prefix | Address                              |
| :-----: | :--: | :----: | :----: | ------------------------------------ |
| Mainnet | `63` | `0x3F` |  `S`   | `SNAgA2XCRZDKfm5Vu9h4KR1bZw5xn9EiC3` |
| Testnet | `30` | `0x1E` |  `D`   | `D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib` |

### Example Construction

<div align="center">

|                              PublicKey                               |
| :------------------------------------------------------------------: |
| `034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192` |

|            RIPEMD160(PublicKey)            |
| :----------------------------------------: |
| `0995750207ecaf0ccf251c1265b92ad84f553662` |

|    Base58Check(`0x3F`, RIPEMD160)    |    Base58Check(`0x1E`, RIPEMD160)    |
| :----------------------------------: | :----------------------------------: |
| `SNAgA2XCRZDKfm5Vu9h4KR1bZw5xn9EiC3` | `D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib` |

</div>

## Transaction Structures

- TypeGroup 1
  - [Type 5: IPFS](./transactions/TG01_T05_IPFS.md)
  - [Type 6: Transfer](./transactions/TG01_T06_TRANSFER.md)
- TypeGroup 2
  - [Type 0: Burn](./transactions/TG02_T00_BURN.md)
  - [Type 2: Vote](./transactions/TG02_T02_VOTE.md)

### Signature

[BIP-0340 Schnorr](https://github.com/bitcoin/bips/blob/master/bip-0340.mediawiki) is used to sign transactions on the [SECP-256k1](https://www.secg.org/sec2-v2.pdf#subsubsection.2.4.1) curve.
A Solar transaction is signed by first constructing a SHA256 hash of a serialised transaction (`sha256(serialised_tx)`); this hash is subsequently signed producing a 64-byte Schnorr signature.

### Fee

Solar transaction fees are dynamic and can be found via the network's API: [https://sxp.mainnet.sh/api/node/fees](https://sxp.mainnet.sh/api/node/fees)

## Links

- Solar Documentation - [https://docs.solar.org/](https://docs.solar.org)
- Solar Whitepaper - [https://docs.solar.org/assets/documents/whitepaper-february-2022.pdf](https://docs.solar.org/assets/documents/whitepaper-february-2022.pdf)
