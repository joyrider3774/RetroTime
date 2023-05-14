#!/bin/sh

NAME="Retrotime"
PROJ="retrotime"
ARCH="x86_64"
DATA="retrotimefs"
SDL_LIB_DIR="/home/joyrider3774/dev/x86_64/sdl2/lib"

rm -rf "$PROJ.AppDir"
mkdir -p "$PROJ.AppDir/usr/bin"
mkdir -p "$PROJ.AppDir/usr/lib"
cp "$PROJ" "$PROJ.AppDir/usr/bin"
cp "$DATA/data/io.github.joyrider3774.retrotime.svg" "$PROJ.AppDir/$PROJ.svg"

cp -R "$DATA" "$PROJ.AppDir/usr"
cd "$PROJ.AppDir"
cp "$SDL_LIB_DIR"/* usr/lib
rm -f usr/lib/*.a
#remove some blacklisted libs as per https://github.com/AppImageCommunity/pkg2appimage/blob/master/excludelist
#definatly need to remove libasound as it causes issues on systems not running pulse
rm -f usr/lib/libasound*
#if i removed these below even though they are blacklisted the game would not run on my "clean ubuntu 16.04 vm" so i still include them
#rm -f usr/lib/libfreetype*
#rm -f usr/lib/libharfbuzz*
#rm -f usr/lib/libz*

strip usr/bin/* usr/lib/*

echo "[Desktop Entry]
Type=Application
Name=$NAME
Exec=$PROJ
Categories=Game;
Icon=$PROJ" > "$PROJ.desktop"

wget -O AppRun https://github.com/probonopd/AppImageKit/releases/download/12/AppRun-x86_64
chmod +x AppRun

cd ..
wget -N https://github.com/probonopd/AppImageKit/releases/download/12/appimagetool-x86_64.AppImage
chmod +x appimagetool-x86_64.AppImage
ARCH=x86_64 ./appimagetool-x86_64.AppImage "$PROJ.AppDir" "$PROJ-$ARCH.AppImage"

