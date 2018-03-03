A UDP port knocking program

Enviornment:<br />
Ubuntu 16.04, gcc version 5.4.0, python3.6.1, libssl-dev (apt-get install libssl-dev)<br />

File Structure:<br />
    README.md:         introduction of this project<br />
    src:               folder contains the source file in C<br />
    integration:       the integration test code in python<br />

1. how to compile and run unit test<br />
    go to src folder<br />
        1.1 make test : it will compile the test code and run unit test<br />
        1.2 make server : compile the udp server code<br />
        1.3 make client : compile the udp client code<br />
        1.4 make clean  : remove the binary files after compilation<br />

2. how to run server and client<br />
    go to src folder and compile server and client<br />
        2.1 run server : make sure you have root permission, because it listens on 53 port<br />
        ```
        make server; sudo ./udp_server
        ``` <br />
        2.2 run client: <br />
        ```
        make client; ./udp_client 1234567   (1234567 is password)
        ```

3. how to run integration test <br />
    go to integration folder <br />
        run integration_test.py with sudo <br />
        ```
        sudo python integration_test.py 
        ```

4. about firewall compatibility<br />
    usually, the firewall filter the ip packets based on ip address and port<br />
    in this case, the firewall should be between server and client. and the ip<br />
    and port 53 should not be dropped by firewall. <br />
    Perhaps some firewall can inspect packet content to discard invalid packets<br />
    in this case, firewall might check the udp packet to server port 53. In  order<br />
    to bypass firewall, the client can send a "real" dns message to server with <br />
    real dns header and content, but include encrypted content inside queries<br />