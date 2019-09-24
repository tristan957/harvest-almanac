#!/bin/sh

set -e

icon_cache_folder="$MESON_INSTALL_PREFIX/share/icons/hicolor"

if [ -z $DESTDIR ]; then
	echo 'Compiling GSchema'
	glib-compile-schemas "$MESON_INSTALL_PREFIX/share/glib-2.0/schemas"

	if [ -f "$icon_cache_folder/.icon-theme.cache" ]; then
		echo 'Updating icon cache'
		gtk-update-icon-cache -qtf $icon_cache_folder
	fi

	echo 'Updating desktop database'
	update-desktop-database -q "$MESON_INSTALL_PREFIX/share/applications"
fi
