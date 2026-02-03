# ipc

IP calculator

## Prerequisites

Check for the following programs:

* `gcc`
* `make`

## Receiving the project

### Download the archive:

```bash
curl -L -o ipc.zip https://github.com/zverorx/ipc/archive/refs/tags/v2.zip
```

```bash
unzip ipc.zip
```

### Clone a repository:

```bash
git clone https://github.com/zverorx/ipc.git
```

## Compilation

Go to the source directory, and run:

```bash
make
```

To find out more about Makefile features, run:

```bash
make help
```

## Usage

```
ipc <-a> <ip/bitmask>
```

```
ipc <-s> <ip/bitmask> <--equal> <count>
```

```
ipc <-s> <ip/bitmask> <--part> <uint, ...>
```

### For example

#### Analysis

```bash
$ ./ipc -a 192.168.1.1/24

               DEC                 BIN                                     HEX        
Addr           192.168.001.001     11000000.10101000.00000001.00000001     c0.a8.01.01
Bitmask        24
Netmask        255.255.255.000     11111111.11111111.11111111.00000000     ff.ff.ff.00     
Wildcard       000.000.000.255     00000000.00000000.00000000.11111111     00.00.00.ff     
Network        192.168.001.000     11000000.10101000.00000001.00000000     c0.a8.01.00     
Broadcast      192.168.001.255     11000000.10101000.00000001.11111111     c0.a8.01.ff     
Hostmin        192.168.001.001     11000000.10101000.00000001.00000001     c0.a8.01.01     
Hostmax        192.168.001.254     11000000.10101000.00000001.11111110     c0.a8.01.fe     
Hosts          254
```

#### Splitting into equal subnets

```bash
$ ./ipc -s 192.168.1.1/24 --equal 5

     MIN                 MAX                 MASK       
0    192.168.001.000     192.168.001.031     27
1    192.168.001.032     192.168.001.063     27
2    192.168.001.064     192.168.001.095     27
3    192.168.001.096     192.168.001.127     27
4    192.168.001.128     192.168.001.159     27
```

#### Split into custom-sized subnets

```bash
$ ./ipc -s 192.168.1.1/24 --part 2 6 16 10

     MIN                 MAX                 MASK       
0    192.168.001.000     192.168.001.015     28
1    192.168.001.016     192.168.001.031     28
2    192.168.001.032     192.168.001.039     29
3    192.168.001.040     192.168.001.041     31
```

## License

This project is licensed under the GPLv3. See the LICENSE file for more details.

## Feedback

To contact the developer, you can email zveror1806@gmail.com.
