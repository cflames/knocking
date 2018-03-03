A UDP port knocking program

Enviornment:
Ubuntu 16.04, gcc version 5.4.0, libssl-dev (apt-get install libssl-dev)

File Structure:
    README.md:         introduction of this project
    src:               folder contains the source file in C
    integration_test:  the integration test code in python

1. how to compile and run unit test
    go to src folder
        1.1 make test : it will compile the test code and run unit test
        1.2 make server : compile the udp server code
        1.3 make client : compile the udp client code
        1.4 make clean  : remove the binary files after compilation