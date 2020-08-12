# Auth API Flavours

Access Auth API comes in two flavours:
- [NIST FIPS PUB 800-131A](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-131Ar2.pdf): Rivest-Shamir-Adleman (**RSA**) 2048.
- [NIST SP 800-186](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186-draft.pdf): Elliptic Curve Digital Signature Algorithm (**ECDSA**) with secp160r1.

Both flavours share the following cryptographic primitives:
- [NIST FIPS PUB 198-1](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.198-1.pdf): Keyed-Hash Message Authentication Code (**HMAC**) [**SHA-256**](https://www.cs.princeton.edu/~appel/papers/verif-sha.pdf).
- [NIST FIPS PUB 197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf): Advanced Encryption Standard 256 (**AES256**).
