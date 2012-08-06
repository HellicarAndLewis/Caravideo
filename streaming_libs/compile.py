#!/usr/bin/python
import os
import os.path
import commands
import sys

bd = os.getcwd() +"/build/"

def set_environ():
    os.environ["PKG_CONFIG"] = bd + "bin/pkg-config"
    os.environ["PKG_CONFIG_PATH"] = bd +"bin/pkgconfig"
    os.environ["PATH"] = bd +"/bin:" +os.environ["PATH"]
    os.environ["CXX"] = "i686-apple-darwin10-llvm-g++-4.2"
    os.environ["CC"] = "i686-apple-darwin11-llvm-gcc-4.2"
    os.environ["LDFLAGS"] = "-L" +bd +"lib/"
    os.environ["CFLAGS"] = "-Os -m32 -I" +bd +"include/"
    os.environ["CXXFLAGS"] = os.environ["CFLAGS"]
    os.environ["LIBFFI_LIBS"] = "-lffi"
    os.environ["LIBFFI_CFLAGS"] = "-I" +bd +"lib/libffi-3.0.11/include/"

def configure(directory, options):
    if not os.path.exists(directory):
        print directory," does not exist"
    os.system("cd " +directory +" && ./configure --prefix=" +bd +" " +options +" && make && make install")


set_environ()


lame_options_base = ""
libav_options_base = "--arch=x86_32 --enable-cross-compile --disable-hwaccels --enable-libx264 --enable-gpl --enable-libmp3lame --enable-debug=3 --disable-optimizations --disable-avserver --target-os=darwin cc='gcc -m32'"
x264_options_base = "--exec-prefix=" +bd +" --prefix=" +bd +" --enable-static --enable-debug "

lame_options = ""
libav_options = ""
x264_options = ""

def compile():
    set_environ()

    # yasm
    configure("yasm", "")
    
    # lame
    configure("lame", lame_options)

    # flvmeta
    if os.path.exists("flvmeta"):
        os.system("cd flvmeta && mkdir compiled && cd compiled && cmake -DCMAKE_INSTALL_PREFIX=" +bd +" ../  && make && make install")
    else:
        print "Cannot install FLVMeta. Not found"

    # x264
    configure("x264", x264_options)
    
    # libav
    configure("libav", libav_options)



# Compile static version
build = "shared"
build = ""
if build != "shared":
    lame_options = lame_options_base +" --disable-shared"
    libav_options = libav_options_base +" --disable-shared"
    x264_options = "--exec-prefix=" +bd +" --prefix=" +bd +" --enable-static --enable-debug "
    bd = os.getcwd() +"/build/"
    compile()
else:
    # Compile dynamic lib
    bd = os.getcwd() +"/build_shared/"
    lame_options = lame_options_base
    libav_options = libav_options_base +" --enable-shared"
    x264_options = "--exec-prefix=" +bd +" --prefix=" +bd +" --enable-static --enable-debug  --enable-shared"
    compile()
