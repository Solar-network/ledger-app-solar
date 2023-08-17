# Solar App Commands

## Overview

| Command Name     |  INS   | Description                                                         |
|------------------|:------:|---------------------------------------------------------------------|
| `GET_APP_NAME`   | `0xa1` | Get the ASCII encoded application name.                             |
| `GET_VERSION`    | `0xa2` | Get the application's version as `[MAJOR(1), MINOR(1), PATCH(1)]`.  |
| `GET_PUBLIC_KEY` | `0xb1` | Get the user's PublicKey given a BIP32 path.                        |
| `GET_ADDRESS`    | `0xb2` | Get the user's Solar Address given a BIP32 path and a network.      |
| `SIGN_MESSAGE`   | `0xc1` | Sign a message given a BIP32 path and an ASCII text message.        |
| `SIGN_TX`        | `0xc2` | Sign a transaction given a BIP32 path and a serialised transaction. |

## GET_APP_NAME

The `GET_APP_NAME` instruction will return the app name, "`Solar`" as an ASCII-encoded value.

### Command

|  CLA   |  INS   |   P1   |   P2   |   Lc   |   CData    |
|:------:|:------:|:------:|:------:|:------:|:----------:|
| `0xe0` | `0xa1` | `0x00` | `0x00` | `0x00` | _not used_ |

> Note that `CData` is not used and should remain empty.

### Parameters

For the `GET_APP_NAME` instruction, both P1 and P2 values are not used and should be set as `0x00`.

| Parameter | Value  | Description |
|:---------:|:------:|:-----------:|
|    P1     | `0x00` | _not used_  |
|    P2     | `0x00` | _not used_  |

### Response

| Response length |    SW    |  RData  |
|:---------------:|:--------:|:-------:|
|   5 _(bytes)_   | `0x9000` | `Solar` |

### Example Session

```shell
=> e0a1000000
<= 536f6c61729000
   # "Solar", 0x9000
```

## GET_VERSION

The `GET_VERSION` instruction will return the Solar App's version as a byte array of `[MAJOR(1), MINOR(1), PATCH(1)]`.

### Command

|  CLA   |  INS   |   P1   |   P2   |   Lc   |   CData    |
|:------:|:------:|:------:|:------:|:------:|:----------:|
| `0xe0` | `0xa2` | `0x00` | `0x00` | `0x00` | _not used_ |

> Note that `CData` is not used and should remain empty.

### Parameters

For the `GET_VERSION` instruction, both P1 and P2 values are not used and should be set as `0x00`.

| Parameter | Value  | Description |
|:---------:|:------:|:-----------:|
|    P1     | `0x00` | _not used_  |
|    P2     | `0x00` | _not used_  |

### Response

| Response length |    SW    |              RData               |
|:---------------:|:--------:|:--------------------------------:|
|   3 _(bytes)_   | `0x9000` | `[MAJOR(1), MINOR(1), PATCH(1)]` |

### Example Session

```shell
=> e0a2000000
<= 0101039000
   # "1 1 3", 0x9000
```

## GET_PUBLIC_KEY

The `GET_PUBLIC_KEY` instruction will return the user's PublicKey given a derivation path (e.g., `"44'/3333'/0'/0/0"`). A chain code used for external key derivation may additionally be requested using P2 as described below.

### Command

|  CLA   |  INS   |        P1         |        P2         |   Lc   |                              CData                              |
|:------:|:------:|:-----------------:|:-----------------:|:------:|:---------------------------------------------------------------:|
| `0xe0` | `0xb1` | `0x00`<br/>`0x01` | `0x00`<br/>`0x01` | 1 + 4n | `[len(bip32_path)(1), bip32_path{1}(4), ..., bip32_path{n}(4)]` |

### Parameters

For the `GET_PUBLIC_KEY` instruction, the P1 value is used to signify whether or not to display the user's PublicKey on the Ledger device's screen and prompt the user for approval.

| P1                                                                                                                                                                                                                                                        |
|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead><tr><th><b>Value</b></th><th><b>Description</b></th></tr></thead><tbody><tr><td>`0x00`</td><td>Don't display or ask for user confirmation.</td></tr><tr><td>`0x01`</td><td>Display and ask for user confirmation.</td></tr></tbody></table> |

For the `GET_PUBLIC_KEY` instruction, the P2 value is used to signify whether or not the HD chain code should be included in the response. _(see ['BIP-0032: Extended Keys'](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki#extended-keys) for more about the chain code)._

| P2                                                                                                                                                                                                                           |
|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead><tr><th><b>Value</b></th><th><b>Description</b></th></tr></thead><tbody><tr><td>`0x00`</td><td>Don't request the chain code.</td></tr><tr><td>`0x01`</td><td>Request the chain code.</td></tr></tbody></table> |

### Response

No Chain Code:

| Response length |    SW    |                 RData                  |
|:---------------:|:--------:|:--------------------------------------:|
|  34 _(bytes)_   | `0x9000` | `[len(public_key)(1), public_key(33)]` |

With Chain Code:

| Response length |    SW    |                                   RData                                    |
|:---------------:|:--------:|:--------------------------------------------------------------------------:|
|  67 _(bytes)_   | `0x9000` | `[len(public_key)(1), public_key(33), len(chain_code)(1), chain_code(32)]` |

### Example Session

```shell
=> e0b1000015058000002c80000d05800000000000000000000000
<= 21033cf83b4df94857a7a782f910e70dd7a777ae6e53f51f31932a8073cdf9caff039000
   # 33, 0x033cf83b4df94857a7a782f910e70dd7a777ae6e53f51f31932a8073cdf9caff03, 0x9000
```

## GET_ADDRESS

The `GET_ADDRESS` instruction will return the user's Address given a derivation path (e.g., `"44'/3333'/0'/0/0"`).

### Command

| CLA    | INS    | P1                | P2                                    | Lc     | CData                                                           |
|--------|--------|-------------------|---------------------------------------|--------|-----------------------------------------------------------------|
| `0xe0` | `0xb2` | `0x00`<br/>`0x01` | `0x3F` (mainnet)<br/>`0x1E` (testnet) | 1 + 4n | `[len(bip32_path)(1), bip32_path{1}(4), ..., bip32_path{n}(4)]` |

### Parameters

For the `GET_ADDRESS` instruction, the P1 value is used to signify whether or not to display the user's PublicKey on the Ledger device's screen and prompt the user for approval.

| P1                                                                                                                                                                                                                                                        |
|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead><tr><th><b>Value</b></th><th><b>Description</b></th></tr></thead><tbody><tr><td>`0x00`</td><td>Don't display or ask for user confirmation.</td></tr><tr><td>`0x01`</td><td>Display and ask for user confirmation.</td></tr></tbody></table> |

For the `GET_ADDRESS` instruction, the P2 value is used to specify which network the Address should be created for.

| P2                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead> <tr> <th><b>Value</b></th> <th><b>Network</b></th> <th><b>Prefix</b></th> <th><b>Example Address</b></th> </tr> </thead><tbody> <tr> <td><code>0x3F</code></td> <td><code>Mainnet</code></td> <td><code>S</code></td> <td><code>SNAgA2XCRZDKfm5Vu9h4KR1bZw5xn9EiC3</code></td> </tr><tr> <td><code>0x1E</code></td> <td><code>Testnet</code></td> <td><code>D</code></td> <td><code>D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib</code></td> </tr> </tbody></table> |

### Response

| Response length |    SW    |                  RData                   |
|:---------------:|:--------:|:----------------------------------------:|
|  35 _(bytes)_   | `0x9000` | `[len(address)(1), base_58_address(34)]` |

### Example Session

```shell
=> e0b2013f15058000002c80000d05800000000000000000000000  # (with user approval)
<= 5351424b71716969735933415a59436a63585358334d657133385a6e47644755754b9000
   # "SQBKqqiisY3AZYCjcXSX3Meq38ZnGdGUuK", 0x9000
```

## SIGN_MESSAGE

The `SIGN_MESSAGE` instruction is used to sign an ASCII text message for a given derivation path (e.g., `"44'/3333'/0'/0/0"`).

First Chunk:

|  CLA   |  INS   |   P1   |   P2   |   Lc   |                              CData                              |
|:------:|:------:|:------:|:------:|:------:|:---------------------------------------------------------------:|
| `0xe0` | `0xc1` | `0x00` | `0x80` | 1 + 4n | `[len(bip32_path)(1), bip32_path{1}(4), ..., bip32_path{n}(4)]` |

> Note that the first chunk will only contain path information in `CData`.

_n<sup>th</sup>_ Chunk:

|  CLA   |  INS   |    P1    |               P2                |           Lc           |         CData          |
|:------:|:------:|:--------:|:-------------------------------:|:----------------------:|:----------------------:|
| `0xe0` | `0xc1` | _`n...`_ | `0x80` (more)<br/>`0x00` (last) | variable (_`1...255`_) | `[payload_data_chunk]` |

### Parameters

For the `SIGN_MESSAGE` instruction, the P1 value is used to represent the chunk's index (i.e., the _n<sup>th</sup>_ chunk).

> There is currently a limit of 8 chunks max.

| P1                                                                                                                                                                                                           |
|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead><tr><th><b>Value</b></th><th><b>Description</b></th></tr></thead><tbody><tr><td>`0x00`</td><td>The first chunk</td></tr><tr><td>`...`</td><td>_n<sup>th</sup>_ chunk</td></tr></tbody></table> |

For the `SIGN_MESSAGE` instruction, the P2 value is used to signify whether there are more chunks to be received or if the current chunk is the final chunk of the request session.

| P2                                                                                                                                                                                                                                                      |
|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead><tr><th><b>Value</b></th><th><b>Description</b></th></tr></thead><tbody><tr><td>`0x80`</td><td>More chunks should be expected.</td></tr><tr><td>`0x00`</td><td>This is the final chunk that should be expected.</td></tr></tbody></table> |

### Response

| Response length |    SW    |           RData           |
|:---------------:|:--------:|:-------------------------:|
|  64 _(bytes)_   | `0x9000` | `[schnorr_signature(64)]` |

### Example Session

```shell
=> e0c1008015058000002c80000d05800000000000000000000000
<= 9000
=> e0c101000d48656C6C6F2C20576F726C6421  # ("Hello, World!")
<= <signature(64)>, 0x9000
```

## SIGN_TX

The `SIGN_TX` instruction is used to sign a Solar transaction for a given derivation path (e.g., `"44'/3333'/0'/0/0"`).

### Command

First Chunk:

|  CLA   |  INS   |   P1   |   P2   |   Lc   |                              CData                              |
|:------:|:------:|:------:|:------:|:------:|:---------------------------------------------------------------:|
| `0xe0` | `0xc2` | `0x00` | `0x80` | 1 + 4n | `[len(bip32_path)(1), bip32_path{1}(4), ..., bip32_path{n}(4)]` |

> Note that the first chunk will only contain path information in `CData`.

_n<sup>th</sup>_ Chunk:

|  CLA   |  INS   |    P1    |               P2                |           Lc           |         CData          |
|:------:|:------:|:--------:|:-------------------------------:|:----------------------:|:----------------------:|
| `0xe0` | `0xc2` | _`n...`_ | `0x80` (more)<br/>`0x00` (last) | variable (_`1...255`_) | `[payload_data_chunk]` |

### Parameters

For the `SIGN_TX` instruction, the P1 value is used to represent the chunk's index (i.e., the _n<sup>th</sup>_ chunk).

> There is currently a limit of 8 chunks max.

| P1                                                                                                                                                                                                           |
|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead><tr><th><b>Value</b></th><th><b>Description</b></th></tr></thead><tbody><tr><td>`0x00`</td><td>The first chunk</td></tr><tr><td>`...`</td><td>_n<sup>th</sup>_ chunk</td></tr></tbody></table> |

For the `SIGN_TX` instruction, the P2 value is used to signify whether there are more chunks to be received or if the current chunk is the final chunk of the request session.

| P2                                                                                                                                                                                                                                                      |
|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| <table><thead><tr><th><b>Value</b></th><th><b>Description</b></th></tr></thead><tbody><tr><td>`0x80`</td><td>More chunks should be expected.</td></tr><tr><td>`0x00`</td><td>This is the final chunk that should be expected.</td></tr></tbody></table> |

### Response

| Response length |    SW    |       RData       |
|:---------------:|:--------:|:-----------------:|
|  64 _(bytes)_   | `0x9000` | `[signature(64)]` |

### Example Session

```shell
=> e0c2008015058000002c80000d05800000000000000000000000
<= 9000
=> e0c2010057ff033f0200000002000a00000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19200e1f5050000000000030367796d88130a63616374757331353439c40905736c333370c409
<= <signature(64)>, 0x9000
```

## Status Words

|    SW    | SW name                      | Description                                      |
|:--------:|:-----------------------------|:-------------------------------------------------|
| `0x6985` | `SW_DENY`                    | Rejected by user                                 |
| `0x6A86` | `SW_WRONG_P1P2`              | Either `P1` or `P2` is incorrect                 |
| `0x6A87` | `SW_WRONG_DATA_LENGTH`       | `Lc` or minimum APDU length is incorrect         |
| `0x6D00` | `SW_INS_NOT_SUPPORTED`       | No command exists with `INS`                     |
| `0x6E00` | `SW_CLA_NOT_SUPPORTED`       | Bad `CLA` used for this application              |
| `0xB000` | `SW_WRONG_RESPONSE_LENGTH`   | Wrong response length (buffer size problem)      |
| `0xB001` | `SW_DISPLAY_BIP32_PATH_FAIL` | BIP32 path conversion to string failed           |
| `0xB002` | `SW_DISPLAY_PUBLICKEY_FAIL`  | PublicKey conversion to string failed            |
| `0xB003` | `SW_DISPLAY_AMOUNT_FAIL`     | Amount conversion to string failed               |
| `0xB004` | `SW_WRONG_TX_LENGTH`         | Wrong raw transaction length                     |
| `0xB005` | `SW_TX_PARSING_FAIL`         | Failed to parse raw transaction                  |
| `0xB006` | `SW_TX_HASH_FAIL`            | Failed to compute hash digest of raw transaction |
| `0xB007` | `SW_BAD_STATE`               | Security issue with bad state                    |
| `0xB008` | `SW_SIGNATURE_FAIL`          | Signature of raw transaction failed              |
| `0xB009` | `SW_OUT_OF_ORDER_REQ`        | Requests are not in order                        |
| `0xB010` | `SW_ENCODE_ADDRESS_FAIL`     | PublicKey to Address conversion failed           |
| `0xB011` | `SW_DISPLAY_ADDRESS_FAIL`    | Address conversion to string failed              |
| `0x9000` | `OK`                         | Success                                          |
