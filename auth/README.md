# Auth API

## EdDSA
- **key exchange**: [NIST SP 800-186](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186-draft.pdf) Elliptic Curve Diffie-Hellman (ECDH) X25519.
- **signing/verification**: [NIST FIPS PUB 186-5](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5-draft.pdf) Ed25519.
- **MAC**: [RFC7539](https://tools.ietf.org/html/rfc7539) Poly1305.
- **encryption**: [XSalsa20](https://cr.yp.to/snuffle/xsalsa-20081128.pdf) stream cipher.
- **base lib**: [Sodium v1.0.18](https://github.com/jedisct1/libsodium/tree/1.0.18-RELEASE)