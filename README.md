# Access Core SDK

The Access Core Software Development Kit provides the  components to be used as  building pieces for the Access Server and Client Implementations, and is meant to be used as the starting point to write new IOTA Access applications.

It is composed of the following pieces:
- [Access Policy API](/policy/README.md)
    * Policy Encoding API
        * JSON Flavour
        * Protobuf Flavour
    * Policy Administration API
    * Policy Decision API
    * Policy Enforcement API
    * Policy Information API
- [Access Authentication API](/auth/README.md)
    * RSA Flavour
    * EdDSA Flavour
- [Access Plugin API](/auth/README.md)
    * Plugin API
    * Plugin Manager
- Access Utils
    * Cryptographic primitives
    * Miscelaneous

# Documentation
You can find a precompiled Doxygen html book at [`doxygen/html`](https://github.com/iotaledger/access-sdk/tree/master/doxygen/html).

In order to read it:
1. Clone `https://github.com/iotaledger/access-sdk` to `$SDK` directory
2. Render `$SDK/doxygen/html/index.html` on a web-browser.
