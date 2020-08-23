# Auth API

### EdDSA Flavour
- **key exchange**: [NIST SP 800-186](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186-draft.pdf) Elliptic Curve Diffie-Hellman (ECDH) X25519.
- **signing/verification**: [NIST FIPS PUB 186-5](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5-draft.pdf) Ed25519.
- **MAC**: [RFC7539](https://tools.ietf.org/html/rfc7539) Poly1305.
- **encryption**: [XSalsa20](https://cr.yp.to/snuffle/xsalsa-20081128.pdf) stream cipher.
- **base lib**: [Sodium v1.0.18](https://github.com/jedisct1/libsodium/tree/1.0.18-RELEASE)
- **pros**:
  - IoT friendly.
  - IOTA 1.5 compatible.
  - cryptographically robust.
  - EdDSA is younger than RSA.
- **cons**:
  - unknown vulnerabilities != no vulnerabilities.
- **usage scenarios**: Edge Infrastructure and [POSIX](https://standards.ieee.org/project/1003_1.html) compliant RTOSs.

### RSA Flavour
- **key exchange**: [NIST FIPS PUB 800-56A](https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-56ar.pdf) Diffie-Hellman (DH).
- **signing/verification**: [NIST FIPS PUB 800-131A](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-131Ar2.pdf) Rivest-Shamir-Adleman (RSA) 2048+ bits.
- **HMAC**: [NIST FIPS PUB 198-1](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.198-1.pdf) SHA-256
- **encryption**: [NIST FIPS PUB 197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf) AES256
- **base lib**: [OpenSSL v1.0.2](https://github.com/openssl/openssl/releases/tag/OpenSSL_1_0_2u)
- **pros**:
  - OpenSSL friendly.
  - RSA is still the leading DSA for most certificate applications.
- **cons**:
  - hungry for CPU+RAM (not IoT friendly).
  - [RSA known vulnerabilities](https://crypto.stanford.edu/~dabo/pubs/papers/RSA-survey.pdf), especially [Shor](https://eprint.iacr.org/2017/351.pdf).
- usage scenarios: Cloud Infrastructure and Embedded Linux.
