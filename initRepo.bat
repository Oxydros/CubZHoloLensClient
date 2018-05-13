
cd libs\LibNetwork

copy scripts\linux\ProtoCMakeLists.txt lib_src\proto\CMakeLists.txt

cmake -G "Visual Studio 15"
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
devenv LibNetworks.sln /build Release

mkdir ..\bin\
mkdir ..\include\
mkdir ..\include\protocol

move lib\Release\cubznetworks.lib ..\bin\
move lib\Release\proto.lib ..\bin\

move lib_src\proto\TCPMessage.pb.h ..\include\protocol\
move lib_src\proto\UDPMessage.pb.h ..\include\protocol\
move lib_src\proto\TypeDescription.pb.h ..\include\protocol\
move lib_src\proto\ChatMessage.pb.h ..\include\protocol\

xcopy /e /v lib_src\include ..\include\libnetwork\