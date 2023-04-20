# genericGlm
A OpenGL abstraction for my projects
Allow using a text-only shader (to keep text in simple color, without light).

22.9.22 for configure:
     added --with-gles using GL ES 3 e.g. useful on Raspi's (requires same use on programs)
     
To build use any (lin)ux:
~~~
autoreconf -fis
./configure ...
make
~~~
For Raspi e.g.:
~~~
  ./configure --prefix=/usr --with-gles  
~~~
For windows:
~~~
  pacman -S mingw-w64-x86_64-glm
  in configure.ac comment "dnl PKG_CHECK_MODULES(GLU, [glu])  
  in src/Makefile.am change "libgenericglm_la_LDFLAGS = -lglu32 -no-undefined"
  ./configure --prefix=/mingw64 
~~~    
    
