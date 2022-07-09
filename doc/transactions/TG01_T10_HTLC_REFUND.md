# TypeGroup 1 | Type 10: HTLC Refund

## JSON

```json
{
	"version": 3,
	"network": 63,
	"typeGroup": 1,
	"type": 10,
	"nonce": "8",
	"senderPublicKey": "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192",
	"fee": "0",
	"amount": "0",
	"asset": {
		"refund": {
			"lockTransactionId": "3aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f"
		}
	}
}
```

## Serialised

```shell
ff033f010000000a000800000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed1920000000000000000003aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f
```

## Deserialised

| Key                  |   Pos.   | Size<br>_(bytes)_ | Value<br> _(hex)_                                                      |
| :------------------- | :------: | :---------------: | :--------------------------------------------------------------------- |
| **Header:**          | **[0]**  |       **1**       | `0xff`                                                                 |
| **Version:**         | **[1]**  |       **1**       | `0x03`                                                                 |
| **Network:**         | **[2]**  |       **1**       | `0x3f`                                                                 |
| **TypeGroup:**       | **[3]**  |       **4**       | `0x01000000`                                                           |
| **Type:**            | **[7]**  |       **2**       | `0x0a00`                                                               |
| **Nonce:**           | **[9]**  |       **8**       | `0x0800000000000000`                                                   |
| **SenderPublicKey:** | **[17]** |      **33**       | `0x034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192` |
| **Fee:**             | **[50]** |       **8**       | `0x0000000000000000`                                                   |
| **Memo Length:**     | **[58]** |       **1**       | `0x00`                                                                 |
| **Lock Id:**         | **[59]** |      **32**       | `0x3aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f`   |

## APDU Session

```shell
=> e0c2008015058000002c80000d05800000000000000000000000
<= 9000
=> e0c201005bff033f010000000a000800000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed1920000000000000000003aade2b98391ba7230252530cdd5124183a9f4e582660666ae873da48173ea5f
<= <signature(64)>, 0x9000
```
