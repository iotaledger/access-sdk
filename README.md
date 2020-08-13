# Access Core SDK

The Access Core Software Development Kit provides the  components to be used as  building pieces for the Access Server and Client Implementations, and is meant to be used as the starting point to write new IOTA Access applications.

It is composed of the following pieces:
- Access Policy API
    * Policy Creation API
        * JSON Flavour
        * Protobuf Flavour
    * Policy Administration API
    * Policy Decision API
    * Policy Enforcement API
    * Policy Information API
- Access Authentication API
    * RSA Flavour
    * ECDSA Flavour
- Access Plugins API
    * Plugin API
    * Plugin Manager
- Access Utils
    * Cryptographic primitives
    * Miscelaneous

# Documentation
You can find a precompiled Doxygen html book at [`doxygen/html`](https://github.com/iotaledger/access-sdk/tree/master/doxygen/html).

In order to read it:
1. Clone `https://github.com/iotaledger/access-sdk` to `$SDK` directory
1. Render `$SDK/doxygen/html/index.html` on a web-browser.
