<h1 align="center">
  <img src="access.png">
</h1>
<h2 align="center">
Access-control framework on the IOTA Distributed Ledger.
</h2>

<p align="center">
  <a href="https://discord.iota.org/" style="text-decoration:none;"><img src="https://img.shields.io/badge/Discord-9cf.svg?logo=discord" alt="Discord"></a>
    <a href="https://iota.stackexchange.com/" style="text-decoration:none;"><img src="https://img.shields.io/badge/StackExchange-9cf.svg?logo=stackexchange" alt="StackExchange"></a>
    <a href="https://github.com/iotaledger/access-sdk/blob/master/LICENSE" style="text-decoration:none;"><img src="https://img.shields.io/github/license/iotaledger/access-sdk.svg" alt="Apache 2.0 license"></a>
    <img src="https://github.com/iotaledger/access-sdk/workflows/C/C++%20CI/badge.svg" alt="C/C++ CI">
</p>

**IOTA Access** is a lightweight access-control framework tailored for resource-constrained settings, such as embedded devices and the infrastructure in which they are used.

The framework is also expanded with relevant concepts, such as obligations and the delegation of access-control policies, to particularly address the needs of reliable and secure human-machine interactions in commercial settings of the IoT and mobility space.

IOTA Access uses access control policies. Attributes are described in combination with binary operations that result in the `[grant, deny, conflict, undefined]` set of logical outcomes for access control of some physical device.

## Purpose
Existing access and permissions solutions revolve around having a connection to a centralized system. The permissions and privilege structures are stored in a server or cloud and managed by a centralized entity. The device granting access needs to have a maintained connection to check these access permissions, and it typically has to be stationary and is usually a highly restricted process.

In automotive and smart mobility contexts, none of this works reliably. Vehicles do not have a consistent connection all the time. Constantly checking in with a centralized structure requires high use of bandwidth and poses slew of single points of failure along the data pipeline. With the use of IOTA Access, we look to solve this. IOTA Access decentralizes access and permission structures and is working towards allowing them to be embedded into the devices and the individual processors on those devices directly. This is done with a new policy-based management structure that focuses on device and human interactions in a direct peer to peer context. Policies can be embedded into devices and devices they come in contact with. These policies can be directly managed by the device owner and can allow for a slew of improvements in security and usability.

For example, a vehicle with IOTA Access enabled, could connect to a parking entry station with IOTA Access enabled, and that entry station could directly allow the vehicle to enter and park based on the policy language if it has been approved. Or if there is a payment requirement, the wallet integration built into IOTA Access can allow for direct M2M payments between that vehicle and the parking entry station. No human interaction directly required. The station gives access to the vehicle. The vehicle pays for that access in a predetermined fashion, which could be as granular as by the second charge rates. When the vehicle leaves, the payment stops, and the transaction is concluded. This can work for EV charging, tolling, parking, fast food, usage-based road tax, mobility as a service use cases, or even delivery services. A user want’s a package dropped off in the trunk of their car while they are at work instead of at their house, Access could enable it. A user wants to rent out an autonomous vehicle in the future to make money while they work, then Access could enable that. A homeowner wants a smart lock to directly and securely manage access and payment for that access to a home they are renting out as an AirBnB. Access could enable that. All of these direct, frictionless use cases and more rely on access and permissions systems. And most cannot be done securely or reliably with centralized systems.

## Disclaimer
IOTA Access is a Work-in-Progress, and the project should be seen as a [Minimal Viable Product (MVP)](https://www.agilealliance.org/glossary/mvp). It is by no means ready for production.

## Repositories
IOTA Access is divided into a few repositories:
- [github.com/iotaledger/access-sdk](http://github.com/iotaledger/access-sdk) contains the Access Core Software Development Kit (SDK). It consists of the Core SDK components used as building pieces for Access dApps (server and client).
- [github.com/iotaledger/access-server](https://github.com/iotaledger/access-server) contains the Access Server Reference Implementation (ASRI). It consists of the embedded software meant to be executed on the target device for which access will be delegated.
- [github.com/iotaledger/access-mobile-client](https://github.com/iotaledger/access-mobile-client.git) contains an Android-based Access Reference Implementation. It is meant to work as the User Interface both for creating policies and initiating access requests. It will eventually be replaced by the cross-platform implementation.
- [github.com/iotaledger/access-policy-store](https://github.com/iotaledger/access-policy-store) contains the Access Policy Store. It consists of interface servers for managing policies. It will eventually be rendered obsolete when a Permanode solution is rolled out.

## Documentation (v0.1.0-alpha)

The [docs](/docs/v0.1.0) directory contains markdown files for documentation over the `v0.1.0-alpha` release tag.
Please note that it does not necessarily reflect the current state of `master` or `develop` branches. In case you want to inspect the code, make sure you set the release tag.
 - [Introduction](/docs/v0.1.0/01-introduction.md)
 - [Engineering Specification](/docs/v0.1.0/02-engineering-specs.md)
 - [Architecture](/docs/v0.1.0/03-architecture.md)
 - [Plugin Specs](/docs/v0.1.0/04-plugin-specs.md)
 - [Authentication Specs](/docs/v0.1.0/05-auth-specs.md)
 - [Policy Specs](/docs/v0.1.0/06-policy-specs.md)
 - [Access Server Reference Implementation](/docs/v0.1.0/07-asri.md)
 - [Access Policy Updater](/docs/v0.1.0/08-apu.md)
 - [Getting Started](/docs/v0.1.0/09-getting-started.md)

## SDK Doxygen (develop)
You can find a precompiled Doxygen html book of the Access SDK at [`doxygen/html`](https://github.com/iotaledger/access-sdk/tree/master/doxygen/html).
It reflects the state of the `develop` branch.

In order to read it:
1. Clone `https://github.com/iotaledger/access-sdk` to `$SDK` directory
2. Render `$SDK/doxygen/html/index.html` on a web-browser.

## Contributing  

Pull Requests are welcome.  
This project uses clang-format to format C/C++ code. Before you make any changes please install the format script via running `./git_hooks/pre_commit_install.sh`.  

## XAIN FROST
IOTA Access is based on [XAIN](https://www.xain.io/)'s **FROST** project, which is the byproduct of [Leif-Nissen Lundbeak](https://www.researchgate.net/profile/Leif_Nissen_Lundbaek)'s 2019 PhD Thesis at Imperial College London.

Here's a list of resources on XAIN FROST:

* [Paper] **Access Control via Belnap Logic:Intuitive, Expressive, and Analyzable Policy Composition**: https://www.doc.ic.ac.uk/research/technicalreports/2011/DTR11-6.pdf
* [Conference Paper] **Owner-centric sharing of physical resources, data, and data-driven insights in digital ecosystems**: https://spiral.imperial.ac.uk:8443/handle/10044/1/77522
* [ReadTheDocs] **FROST — Xain Documentation**: https://xain-documentation.readthedocs.io/en/latest/Frost/
* [Video Presentation] **Michael Huth (CTO) presents FROST at Birmingham**: https://www.youtube.com/watch?v=2mHQrmGt7CA
* [Medium Article] **A technical overview of XAIN Embedded Extension**: https://medium.com/xain/a-technical-overview-of-xain-embedded-905678f5b236
* [Medium Article] **Overview of the XAIN Pilot for Porsche Vehicles**: https://medium.com/xain/part-1-technical-overview-of-the-porsche-xain-vehicle-network-f70bb117be16
* [Medium Article] **The Porsche-XAIN Vehicle Blockchain Network: A Technical Overview**: https://medium.com/next-level-german-engineering/the-porsche-xain-vehicle-blockchain-network-a-technical-overview-e1f48c40e73d
* [Marketing Video] **XAIN and Porsche: Results of the pilot project**: https://www.youtube.com/watch?v=KvyF78RTj18
* [Presentation Slides] **Access Control via Belnap Logic: Intuitive, Expressive, and Analyzable Policy Composition**: http://www.doc.ic.ac.uk/~mrh/talks/BelnapTalk.pdf

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
- [Access Plugin API](/auth/README.md)
    * Plugin API
    * Plugin Manager
- Access Utils
    * Cryptographic primitives
    * Miscelaneous
