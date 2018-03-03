A UDP port knocking program

Enviornment:<br />
Ubuntu 16.04, gcc version 5.4.0, libssl-dev (apt-get install libssl-dev)<br />

File Structure:<br />
    README.md:         introduction of this project<br />
    src:               folder contains the source file in C<br />
    integration_test:  the integration test code in python<br />

1. how to compile and run unit test<br />
    go to src folder<br />
        1.1 make test : it will compile the test code and run unit test<br />
        1.2 make server : compile the udp server code<br />
        1.3 make client : compile the udp client code<br />
        1.4 make clean  : remove the binary files after compilation<br />

2. how to run server and client<br />
    go to src folder and compile server and client<br />
        2.1 run server : make sure you have root permission, because it listens on 53 port<br />
        make server<br />
        sudo ./udp_server<br />

        2.2 run client: <br />
        make client <br />
        sudo ./udp_client <br />