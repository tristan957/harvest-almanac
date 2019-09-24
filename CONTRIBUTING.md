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

This project includes support for flatpak and snap in addition to a regular
application.

Unfortunately the version of GTK in the `core18` snap is incompatible with
`libhandy`. A workaround could be using the GTK wrap supplied in this repo. It
would take time to figure out if versions of packages in `core18` work with
GTK 3.24. This is doable if someone commits to it.

#### Host

```text
ninja -C build
```

#### flatpak

```text
flatpak-builder --verbose --sandbox --force-clean --repo=.flatpak-repo --ccache --user --install-deps-from=flathub --install .flatpak  dist/flatpak/io.partin.tristan.HarvestAlmanac.json
```

### Running

#### Host

```text
./build/src/harvest-almanac
```

#### flatpak

```text
flatpak run io.partin.tristan.HarvestAlmanac
```
