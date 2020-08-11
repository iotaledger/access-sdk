# Access Core SDK

The Access Core Software Development Kit provides the  components to be used as  building pieces for the Access Server and Client Implementations, and is meant to be used as the starting point to write new IOTA Access applications.

It is composed of the following pieces:
- Access Core API
    * Policy Administration API
    * Policy Decision API
    * Policy Enforcement API
    * Policy Information API
- Access Authentication API
    * OpenSSL Implementation
    * TinyAuth Implementation
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
1. Render `$SDK/doxygen/html/auth_8h.html` on a web-browser.
