# echo-server
simple echo-server impl by c

clone enter project dir

``` shell
mkdir build
cd build
cmake ..
make

# next line to launch background process tcp server on port 9988
nohup ./echo-server-tcp 9988 &

# next line to launch udp-server on port 8877
./echo-server-udp 8877

```
