# Transfering data by time interval
Final project for streaming algorithm


## Building and Testing

### Step 1
```shell
mkdir build
cd build
```

### Step 2
```shell
cmake ..
```
Or if you want to build in release mode:
```shell
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Step 3
```shell
make
```

### Step 4

receiver needs to bind the port first

#### On sender
```shell
./trans recvip port data
```

#### On receiver
```shell
./recv port
```
