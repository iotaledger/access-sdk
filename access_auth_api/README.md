# Auth API Flavours

Access Auth API comes in two flavours:
- RSA flavour
  - [NIST FIPS PUB 800-56A](https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-56ar.pdf) Diffie-Hellman (**DH**) key exchange.
  - [NIST FIPS PUB 800-131A](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-131Ar2.pdf): Rivest-Shamir-Adleman (**RSA**) 2048 signature scheme.
- ECDSA flavour
  - [NIST SP 800-186](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186-draft.pdf): Elliptic Curve Diffie-Hellman (**ECDH**) key exchange.
  - [NIST SP 800-186](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186-draft.pdf): Elliptic Curve Digital Signature Algorithm (**ECDSA**) with secp160r1.

Both flavours share the following cryptographic primitives:
- [NIST FIPS PUB 198-1](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.198-1.pdf): Keyed-Hash Message Authentication Code (**HMAC**) [**SHA-256**](https://www.cs.princeton.edu/~appel/papers/verif-sha.pdf).
- [NIST FIPS PUB 197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf): Advanced Encryption Standard 256 (**AES256**).

## RSA Flavour
- key exchange: **DH**
- signing/verification: **RSA**
- pros:
  - OpenSSL compatible.
  - keypair generated deterministically from IOTA seeds.
- cons:
  - RAM hungry.
- scenarios:
  - **SSL**/**TCP**/**IPv4**/**802.3** Wide Area Network (**WAN**) on **SoC HW-based MMU with 512Mb RAM** (Embedded Linux arm64).
  - **SSL**/**TCP**/**IPv4**/**802.3** World Wide Web (**WWW**) on **VPS** (Linux x86-64).



## ECDSA Flavour
- key exchange: **ECDH** with **Curve25519**
- signing/verification: **ECDSA** with **secp160r1**.
- pros:
  - RAM-light.
- cons:
  - Not Quantum robust.
-scenarios:
  - **SSL**/**TCP**/**IPv6**/**6LoWPAN** Bluetooth Low Energy (**BLE**) Network on **uC SW-based MMU with 256kb RAM** (FreeRTOS armv7-m).
  - **SSL**/**TCP**/**IPv4**/**802.11** Local Area Network (**LAN**) on **uC SW-based MMU with 520 KiB SRAM** (ZephyrOS esp32).
