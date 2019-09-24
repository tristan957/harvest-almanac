#!/bin/sh

GLADE_CATALOG_SEARCH_PATH="subprojects/libhandy-v0.0.11/glade/:${GLADE_CATALOG_SEARCH_PATH}" \
	GLADE_MODULE_SEARCH_PATH="build/subprojects/libhandy-v0.0.11/glade:${GLADE_MODULE_SEARCH_PATH}" \
	glade
