cmd_Release/obj.target/hello.node := flock ./Release/linker.lock g++ -shared -pthread -rdynamic  -Wl,-soname=hello.node -o Release/obj.target/hello.node -Wl,--start-group Release/obj.target/hello/hello.o -Wl,--end-group /usr/lib/libopencv_core.so /usr/lib/libopencv_highgui.so /usr/lib/libopencv_imgproc.so