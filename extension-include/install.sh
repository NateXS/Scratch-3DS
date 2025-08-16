#!/usr/bin/env sh

# TODO: Support Windows
if [ "$1" == "linux" ] || [ "$1" == "macos" ]; then
	INSTALL_PATH=/usr/local/include/
elif [ "$1" == "wiiu" ]; then
	INSTALL_PATH=$DEVKITPRO/portlibs/wiiu/include/
elif [ "$1" == "3ds" ]; then
	INSTALL_PATH=$DEVKITPRO/portlibs/3ds/include/
elif [ "$1" == "wii" ]; then
	INSTALL_PATH=$DEVKITPRO/portlibs/wii/include/
elif [ "$1" == "gamecube" ]; then
	INSTALL_PATH=$DEVKITPRO/portlibs/wii/include/
else
	echo "Unsupported platform: $1"
	exit 1
fi

echo Installing to $INSTALL_PATH
sudo cp scratch-3ds.hpp $INSTALL_PATH
