# Plugin API

While Access Core SDK is platform agnostic, Plugins add flexibility for different use cases.

Plugins handle hardware (sensors and actuators) abstractions while respecting the specifications imposed by their correspondent Access Policy API modules.

## PAP Plugins
Policies need to be stored on the device in a non-volatile manner.

Embedded Systems can have different forms of permanent storage. It can be eMMC, SSD, SATA, USB, or many other examples. This variability implies the need for a modular approach for Policy Storage. Also, (RT) Operating Systems impose different ways to handle non-volatile memory, as with [file abstractions in POSIX](/portability/os/posix/posix.h) for example.

PAP Plugins are used by PAP to read and write Policies from non-volatile memory.

## PEP Plugins
PEP Plugins are used by PEP to enforce actions in the physical world. They are the main interface for **actuators** that need to behave in accordance to the Access being granted or denied.

For example, imagine a door that is controlled by a relay attached to the board's GPIO. The GPIO needs to set the relay such that the door locks and unlocks depending on the decision for grant or denial of access.

## PIP Plugins
PIP Plugins are used by PIP to gather information about the external environment. They are the main interface for **sensors** that provide information that help determine whether access will be granted or denied.

For example, a Wallet-based PIP Plugin is used to verify whether an IOTA transaction was indeed performed. Another example is of a GPS-based PIP Plugin that checks device location.

## Plugin API

Access Core SDK provides a Plugin API with the logical abstractions to handle `plugin_t` objects.
```
typedef struct {
  plugin_cb destroy;
  plugin_cb *callbacks;
  size_t callbacks_num;
  void *plugin_data;
} plugin_t;
```

The type `plugin_cb` is represents a callback function. It receives a reference to the plugin and some input arguments.
```
typedef int (*plugin_cb)(plugin_t *plugin, void *data);
```

So that means each `plugin_t` object has one `destroy` callback (`plugin_cb`), and a pointer array to other functional (also `plugin_cb`) callbacks. There is a `callbacks_num` counter to keep track of the pointer array size. Finally, there is a `void` pointer called `plugin_data`, where any kind of additional plugin information is stored.

xxx

<!--
ToDo: continue
-->