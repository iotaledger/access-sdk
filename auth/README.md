# Auth API

Access Auth API comes in two flavours:
- RSA flavour
- ECDSA flavour

Both flavours share the following cryptographic primitives:
- Keyed-Hash Message Authentication Code (**HMAC**) **SHA-256** [[NIST FIPS PUB 198-1](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.198-1.pdf)]
- Advanced Encryption Standard 256 (**AES256**) [[NIST FIPS PUB 197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf)]

## RSA Flavour
- key exchange: [NIST FIPS PUB 800-56A](https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-56ar.pdf) Diffie-Hellman (**DH**).
- signing/verification: [NIST FIPS PUB 800-131A](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-131Ar2.pdf): Rivest-Shamir-Adleman (**RSA**) 2048 bits.
- pros:
  - OpenSSL friendly.
  - RSA is still the leading DSA for most certificate applications.
  - deterministic keypair derived from IOTA seed (legacy or BIP39, IOTA 2.0) (via [dOpenSSL](https://github.com/bernardoaraujor/dopenssl)).
- cons:
  - hungry for CPU+RAM (not IoT friendly).
  - [RSA known vulnerabilities](https://crypto.stanford.edu/~dabo/pubs/papers/RSA-survey.pdf), especially [Shor](https://eprint.iacr.org/2017/351.pdf).
- usage scenarios:
  - Wide Area Network (**WAN**) over **SSL** / **TCP** / **IPv4** / **802.3** stack on **SoC HW-based MMU with 512Mb RAM** (Embedded Linux arm64).
  - World Wide Web (**WWW**) over **SSL** / **TCP** / **IPv4** / **802.3** stack on **VPS** (Linux x86-64).

## ECDSA Flavour
- key exchange: Elliptic Curve Diffie-Hellman (**ECDH**) with **Curve25519** [[NIST SP 800-186](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186-draft.pdf)]
- signing/verification: Elliptic Curve Digital Signature Algorithm (**ECDSA**) with secp160r1 [[NIST SP 800-186](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186-draft.pdf)].
- pros:
  - not hungry for CPU+RAM (IoT friendly).
- cons:
  - ECDSA is less mature and standardized than RSA.
  - Unknown vulnerabilities != No vulnerabilities.
  - No deterministic keypairs derived from IOTA seeds.
- usage scenarios:
  - Bluetooth Low Energy (**BLE**) Network over **SSL** / **TCP** / **IPv6** / **6LoWPAN** stack on **uC SW-based MMU with 256kb RAM** (FreeRTOS armv7-m).
  - Local Area Network (**LAN**) over **SSL** / **TCP** / **IPv4** / **802.11** stack on **uC SW-based MMU with 520 KiB SRAM** (ZephyrOS esp32).