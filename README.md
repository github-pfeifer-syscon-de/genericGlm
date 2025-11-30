# genericGlm
A OpenGL abstraction for my projects
Allow using a text-only shader (to keep text in simple color, without light).

22.9.22 for configure:
     added --with-gles using GL ES 3 e.g. useful on Raspi's (requires same use on programs)

## Minimum requirements

- requires genericImg so read&build&install it first.
- the project can be downloaded with
<pre>
git clone https://github.com/github-pfeifer-syscon-de/genericGlm
</pre>

## Debian

For Debian you may need the following setup steps
(run with sudo or as root):
<pre>
apt-get install libglm-dev libglu1-mesa-dev
</pre>

## Any Linux

To build use from project dir:
<pre>
autoreconf -fis
./configure --prefix=/usr
make
</pre>

## Raspi
for Raspi configure with:
<pre>
  ./configure --prefix=/usr --with-gles
</pre>

## Windows

For windows (get msys2 https://www.msys2.org/) the files shoud adapt:
<pre>
pacman -S ${MINGW_PACKAGE_PREFIX}-glm
autoreconf -fis
./configure --prefix=${MINGW_PREFIX}
make
</pre>

## Any system

The lib requires to be installed before use so (on linux run as root):
<pre>
make install
</pre>

## Navigation

The programs using this lib come with some navigation options:

Without a selection the option work on the whole scene, otherwise for the selected item:

Left click -> select item (only shapes will be selectable (not text)) (some geometries e.g. trees will move dependent entries as well, and some additional infos might be displayed when selected e.g. a process name)

Left double click -> show options like "reset"

Mouse scroll -> change distance

Drag right mouse buttton -> rotate

Drag middle mouse button -> offset

