# genericGlm
A OpenGL abstraction for my projects
Allow using a text-only shader (to keep text in simple color, without light).

22.9.22 for configure:
     added --with-gles using GL ES 3 e.g. useful on Raspi's (requires same use on programs)

Requires genericImg so build&install it first.

To build use any (lin)ux:
<pre>
autoreconf -fis
./configure ...
make
</pre>
For Raspi e.g.:
<pre>
  ./configure --prefix=/usr --with-gles
</pre>
For windows (get msys2 https://www.msys2.org/) the files shoud adapt use e.g.<br>
(use "msys2 mingw64" window/shell see tooltip)<br>
<pre>
  ./configure --prefix=/mingw64
</pre>
For Debian you may need the following steps (prefix # execute with sudo/root prefix > as user):
<pre>
# apt install git build-essential automake libtool
# apt install libgtkmm-3.0-dev libglm-dev libglu1-mesa-dev
> git clone https://github.com/github-pfeifer-syscon-de/genericGlm
> cd genericGlm
> autoreconf -fis
> configure --prefix=/usr
> make
# cd .../genericGlm
# make install
</pre>
If you run into trouble with the used c++20 change configure.ac AX_CXX_COMPILE_STDCXX([20]... to ...[17]...

## Navigation

The programs using this lib come with some navigation options:

Without a selection the option work on the whole scene, otherwise for the selected item:

Left click -> select item (only shapes will be selectable (not text)) (some geometries e.g. trees will move dependent entries as well, and some additional infos might be displayed when selected e.g. a process name)

Left double click -> show options like "reset"

Mouse scroll -> change distance

Drag right mouse buttton -> rotate

Drag middle mouse button -> offset

