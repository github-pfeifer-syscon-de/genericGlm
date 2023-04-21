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
