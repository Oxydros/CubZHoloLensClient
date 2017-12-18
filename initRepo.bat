
cd libs\LibNetwork

copy scripts\linux\ProtoCmakeLists.txt lib_src\proto\CMakeLists.txt

cmake -G "Visual Studio 15"
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
devenv LibNetworks.sln /build Release
move lib\Release\cubznetworks.lib ..\bin
move lib\Release\proto.lib ..\bin
move lib_src\proto\TCPMessage.pb.h ..\include