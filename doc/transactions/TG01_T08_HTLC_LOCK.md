# TypeGroup 1 | Type 8: HTLC Lock

## JSON

```json
{
	"version": 3,
	"network": 63,
	"typeGroup": 1,
	"type": 8,
	"nonce": "6",
	"senderPublicKey": "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192",
	"fee": "10000000",
	"amount": "1",
	"recipientId": "SNAgA2XCRZDKfm5Vu9h4KR1bZw5xn9EiC3",
	"asset": {
		"lock": {
			"secretHash": "9c1a3815d49e0c9f78b872bfb017e825ea2db708158b70815526a830c85912b4",
			"expiration": {
				"type": 1,
				"value": 78740307
			}
		}
	}
}
```

## Serialised

```shell
ff033f0100000008000600000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed1928096980000000000000100000000000000029c1a3815d49e0c9f78b872bfb017e825ea2db708158b70815526a830c85912b401537bb1043f0995750207ecaf0ccf251c1265b92ad84f553662
```

## Deserialised

| Key                     |   Pos.    | Size<br>_(bytes)_ | Value<br> _(hex)_                                                      |
| :---------------------- | :-------: | :---------------: | :--------------------------------------------------------------------- |
| **Header:**             |  **[0]**  |       **1**       | `0xff`                                                                 |
| **Version:**            |  **[1]**  |       **1**       | `0x03`                                                                 |
| **Network:**            |  **[2]**  |       **1**       | `0x3f`                                                                 |
| **Typegroup:**          |  **[3]**  |       **4**       | `0x01000000`                                                           |
| **Type:**               |  **[7]**  |       **2**       | `0x0800`                                                               |
| **Nonce:**              |  **[9]**  |       **8**       | `0x0600000000000000`                                                   |
| **SenderPublicKey:**    | **[17]**  |      **33**       | `0x034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192` |
| **Fee:**                | **[50]**  |       **8**       | `0x8096980000000000`                                                   |
| **Memo Length:**        | **[58]**  |       **1**       | `0x00`                                                                 |
| **Amount:**             | **[59]**  |       **8**       | `0x0100000000000000`                                                   |
| **Secret Hash Length:** | **[67]**  |       **8**       | `0x20`                                                                 |
| **Secret Hash:**        | **[68]**  |      **32**       | `0x9c1a3815d49e0c9f78b872bfb017e825ea2db708158b70815526a830c85912b4`   |
| **Expiration Type:**    | **[100]** |       **1**       | `0x01`                                                                 |
| **Expiration Value:**   | **[101]** |       **4**       | `0x537bb104`                                                           |
| **Recipient:**          | **[105]** |      **21**       | `0x3f0995750207ecaf0ccf251c1265b92ad84f553662`                         |

## APDU Session

```shell
=> e0c2008015058000002c80000d05800000000000000000000000
<= 9000
=>
e0c201007eff033f0100000008000600000000000000034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed1928096980000000000000100000000000000209c1a3815d49e0c9f78b872bfb017e825ea2db708158b70815526a830c85912b401537bb1043f0995750207ecaf0ccf251c1265b92ad84f553662
<= <signature(64)>, 0x9000
```
