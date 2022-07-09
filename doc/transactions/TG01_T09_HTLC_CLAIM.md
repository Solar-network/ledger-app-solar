# TypeGroup 1 | Type 9: HTLC Claim

## JSON

```json
{
	"version": 3,
	"network": 63,
	"typeGroup": 1,
	"type": 9,
	"nonce": "7",
	"senderPublicKey": "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192",
	"fee": "0",
	"asset": {
		"claim": {
			"lockTransactionId": "3aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f",
			"unlockSecret": "c27f1ce845d8c29eebc9006be932b604fd06755521b1a8b0be4204c65377151a"
		}
	}
}
```

## Serialised

```shell
ff033f0100000009000700000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192000000000000000000003aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f20c27f1ce845d8c29eebc9006be932b604fd06755521b1a8b0be4204c65377151a
```

## Deserialised

| Key                       |   Pos.   | Size<br>_(bytes)_ | Value<br> _(hex)_                                                      |
| :------------------------ | :------: | :---------------: | :--------------------------------------------------------------------- |
| **Header:**               | **[0]**  |       **1**       | `0xff`                                                                 |
| **Version:**              | **[1]**  |       **1**       | `0x03`                                                                 |
| **Network:**              | **[2]**  |       **1**       | `0x3f`                                                                 |
| **TypeGroup:**            | **[3]**  |       **4**       | `0x01000000`                                                           |
| **Type:**                 | **[7]**  |       **2**       | `0x0900`                                                               |
| **Nonce:**                | **[9]**  |       **8**       | `0x0700000000000000`                                                   |
| **SenderPublicKey:**      | **[17]** |      **33**       | `0x034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192` |
| **Fee:**                  | **[50]** |       **8**       | `0x0000000000000000`                                                   |
| **Memo Length:**          | **[58]** |       **1**       | `0x00`                                                                 |
| **Hash Type:**            | **[59]** |       **1**       | `0x00`                                                                 |
| **Lock Id:**              | **[60]** |      **32**       | `0x3aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f`   |
| **Unlock Secret Length:** | **[92]** |       **1**       | `0x20`                                                                 |
| **Unlock Secret:**        | **[93]** |      **32**       | `0xc27f1ce845d8c29eebc9006be932b604fd06755521b1a8b0be4204c65377151a`   |

### Hash Types

| Hash Type | Value |
| --------- | :---: |
| SHA256    |   0   |
| SHA384    |   1   |
| SHA512    |   2   |
| SHA3256   |   3   |
| SHA3384   |   4   |
| SHA3512   |   5   |
| Keccak256 |   6   |
| Keccak384 |   7   |
| Keccak512 |   8   |

## APDU Session

```shell
=> e0c2008015058000002c80000d05800000000000000000000000
<= 9000
=> e0c201007dff033f0100000009000700000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192000000000000000000003aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f20c27f1ce845d8c29eebc9006be932b604fd06755521b1a8b0be4204c65377151a
<= <signature(64)>, 0x9000
```
