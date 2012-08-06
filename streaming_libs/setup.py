#!/usr/bin/python
import os.path
import os
import commands
import sys

if os.access("libav", os.R_OK) and not os.path.exists("libav/avconv.c"):
	os.system("rm -r libav")

if not os.path.exists("libav"):
	os.system("git clone git://git.libav.org/libav.git && cd libav && git reset --hard 65d94f63ca38b46a9d3719cb7d986d2daef90416");

if os.path.exists("lame")  and not os.path.isfile("lame/INSTALL"):
	os.system("rm -r lame")

if not os.path.exists("lame"):
	os.system("cp sources/lame.tar.gz .")
	os.system("tar -zxvf lame.tar.gz && mv lame-* lame && rm lame.tar.gz")

if os.path.exists("flvmeta") and not os.path.isfile("flvmeta/INSTALL"):
	os.system("rm -r flvmeta")

if not os.path.exists("flvmeta"):
	os.system("cp sources/flvmeta.tar.gz .")
	os.system("tar -zxvf flvmeta.tar.gz && mv flvmeta-* flvmeta && rm flvmeta.tar.gz")

if os.path.exists("yasm") and not os.path.isfile("yasm/INSTALL"):
	os.system("rm -r yasm")

if not os.path.exists("yasm"):
	os.system("cp sources/yasm.tar.gz . && tar -zxvf yasm.tar.gz && mv yasm-* yasm")
	os.system("rm yasm.tar.gz")

if os.path.exists("x264") and not os.path.isfile("x264/x264.c"):
	os.system("rm -r x264")

if not os.path.exists("x264"): 
	os.system("cp sources/x264.bz2 . && bunzip2 x264.bz2 && mv x264 x264.tar && tar -xvf x264.tar && mv x264-* x264")
	os.system("rm x264.tar")
