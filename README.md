# ipc

IP calculator

## Prerequisites

Check for the following programs:

* `gcc`
* `make`
* `curl`
* `unzip`

## Receiving the project

```bash
curl -L -o ipc.zip https://github.com/zverorx/ipc/archive/refs/tags/v1.zip
```

```bash
unzip ipc.zip
```

## Compilation and installation

Go to the source directory, and run the following commands:

```bash
make && sudo make install
```

To find out more about Makefile features, run 

```bash
make help
```

## Usage

The command takes a string in IP/BITMASK format as a parameter.

If the compilation and installation stage was successful, you can run in the terminal:

```bash
ipc 192.168.1.1/24
```

The result of such a command will be:

```
               DEC                 BIN                                     HEX        
IP             192.168.001.001     11000000.10101000.00000001.00000001     c0.a8.01.01
Bitmask        24
Netmask        255.255.255.000     11111111.11111111.11111111.00000000     ff.ff.ff.00     
Wildcard       000.000.000.255     00000000.00000000.00000000.11111111     00.00.00.ff     
Network        192.168.001.000     11000000.10101000.00000001.00000000     c0.a8.01.00     
Broadcast      192.168.001.255     11000000.10101000.00000001.11111111     c0.a8.01.ff     
Hostmin        192.168.001.001     11000000.10101000.00000001.00000001     c0.a8.01.01     
Hostmax        192.168.001.254     11000000.10101000.00000001.11111110     c0.a8.01.fe     
Hosts          254
```

## License

This project is licensed under the GPLv3. See the LICENSE file for more details.

## Feedback

To contact the developer, you can email zveror1806@gmail.com.