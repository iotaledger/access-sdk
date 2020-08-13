# Policy API

Access Policy API is used to encapsulate the [Belnap Logic](https://www.doc.ic.ac.uk/research/technicalreports/2011/DTR11-6.pdf).

## Encoding
Policy Encoding comes in two flavours:
 - JSON
 - protobuf-c

### JSON
JavaScript Object Notation (JSON) is a popular data-interchange format. It is easy for humans to read and write.
- pros: human readable.
- cons: resource hungry (not IoT friendly).

### protobuf-c
Protocol Buffers are a language-neutral, platform-neutral extensible mechanism for serializing structured data. It allows for lightweight binary representation of data.
`protobuf-c` is a C implementation of the [Google Protocol Buffers](https://developers.google.com/protocol-buffers/).
- pros: light (IoT friendly).
- cons: not human readable.

## Administration
 
Policy Administration API provides the abstractions for the Policy Administration Point (PAP). The PAP is used for managing local policy updates and matching policies to incoming requests. The API expects callback functions to be registered as PAP Plugins.

## Decision

Policy Decision API provides the abstractions for Policy Decision Point (PDP). The PDP is responsible for calculating the output for access requests. The API only consumed internally by other Policy API modules.

## Enforcement

Policy Enforcement API provides the abstractions for Policy Enforcement Point (PEP). The PEP routes the access request to the PDP for decision making, and acts on the received decision as appropriate within the system context. The API expects callback functions to be registered as PEP Plugins.
 
## Information

Policy Information API provides the abstractions for Policy Information Point. PIP provides attribute values used by the PDP. Attribute values are information collected from the outside world, such as IOTA transactions, sensor data or network traffic. The API expects callback functions to be registered as PIP Plugins.
  