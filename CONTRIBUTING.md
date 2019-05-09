# Contributing

Harvest Almanac is composed of two components:

* `libharvest-glib`
* `harvest-almanac`

## libharvest-glib

`libharvest-glib` is an API wrapper around the Harvest API V2. It makes use of
the GObject type system around its entities.

## harvest-almanac

`harvest-almanac` is the GUI application written in GTK to interact with
Harvest. It uses `libharvest-glib` to do this.

## Development

### Setup

This project has only been tested with `gcc`. The build system for the
project is Meson. Make sure that is installed on your system.

```text
CC=gcc meson build
```

### Dependencies

This project has been configured to use Meson's WrapDB if development libraries
are not found on the host system. If you want to use WrapDB-provided
dependencies, simply run the command

```text
meson configure -Dwrap_mode=forcefallback build
```

### Building

```text
ninja -C build
```
