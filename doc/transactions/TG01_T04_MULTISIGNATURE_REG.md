# TypeGroup 1 | Type 4: MultiSignature Registration

## JSON

```json
{
	"version": 3,
	"network": 63,
	"typeGroup": 1,
	"type": 4,
	"nonce": "3",
	"senderPublicKey": "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192",
	"fee": "2000000000",
	"amount": "0",
	"asset": {
		"multiSignature": {
			"publicKeys": [
				"034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192",
				"03df0a1eb42d99b5de395cead145ba1ec2ea837be308c7ce3a4e8018b7efc7fdb8",
				"03860d76b1df09659ac282cea3da5bd84fc45729f348a4a8e5f802186be72dc17f"
			],
			"min": 2
		}
	}
}
```

## Serialised

```shell
ff033f0100000004000300000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed1920094357700000000000203034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19203df0a1eb42d99b5de395cead145ba1ec2ea837be308c7ce3a4e8018b7efc7fdb803860d76b1df09659ac282cea3da5bd84fc45729f348a4a8e5f802186be72dc17f
```

## Deserialised

| Key                  |   Pos.    | Size<br>_(bytes)_ | Value<br> _(hex)_                                                      |
| :------------------- | :-------: | :---------------: | :--------------------------------------------------------------------- |
| **Header:**          |  **[0]**  |       **1**       | `0xff`                                                                 |
| **Version:**         |  **[1]**  |       **1**       | `0x03`                                                                 |
| **Network:**         |  **[2]**  |       **1**       | `0x3f`                                                                 |
| **Typegroup:**       |  **[3]**  |       **4**       | `0x01000000`                                                           |
| **Type:**            |  **[7]**  |       **2**       | `0x0400`                                                               |
| **Nonce:**           |  **[9]**  |       **8**       | `0x0300000000000000`                                                   |
| **SenderPublicKey:** | **[17]**  |      **33**       | `0x034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192` |
| **Fee:**             | **[50]**  |       **8**       | `0x0094357700000000`                                                   |
| **Memo Length:**     | **[58]**  |       **1**       | `0x00`                                                                 |
| **Key Min:**         | **[59]**  |       **1**       | `0x02`                                                                 |
| **Key Count:**       | **[60]**  |       **1**       | `0x03`                                                                 |
| **Key 1:**           | **[61]**  |      **33**       | `0x034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192` |
| **Key 2:**           | **[94]**  |      **33**       | `0x03df0a1eb42d99b5de395cead145ba1ec2ea837be308c7ce3a4e8018b7efc7fdb8` |
| **Key 3:**           | **[127]** |      **33**       | `0x03860d76b1df09659ac282cea3da5bd84fc45729f348a4a8e5f802186be72dc17f` |

## APDU Session

```shell
=> e0c2008015058000002c80000d05800000000000000000000000
<= 9000
=> e0c20100a0ff033f0100000004000300000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed1920094357700000000000203034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed19203df0a1eb42d99b5de395cead145ba1ec2ea837be308c7ce3a4e8018b7efc7fdb803860d76b1df09659ac282cea3da5bd84fc45729f348a4a8e5f802186be72dc17f
<= <signature(64)>, 0x9000
```
