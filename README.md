# genericGlm
A OpenGL abstraction for my projects
Allow using a text-only shader (to keep text in simple color, without light).

22.9.22 for configure:
     added --with-gles using GL ES 3 e.g. useful on Raspi's (requires same use on programs)

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
<pre>
  ./configure --prefix=/mingw64
</pre>
For Debian you may need the following steps (prefix # execute with sudo/root prefix > as user):
<pre>
# apt install git build-essential automake libtool
# apt install  libgtkmm-3.0-dev libglm-dev libglu1-mesa-dev
> git clone https://github.com/github-pfeifer-syscon-de/genericGlm.git
> cd genericGlm
> in configure.ac change AX_CXX_COMPILE_STDCXX([20]... to ...[17]...
> autoreconf -fis
> configure --prefix=/usr
> make
# cd .../genericGlm
# make install
</pre>