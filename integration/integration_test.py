import socket
import os
import sys
import time
from subprocess import check_output

_CURRENT_PATH = os.path.join(os.path.realpath(__file__))
_SRC_PATH = os.path.join(os.path.dirname(os.path.dirname(_CURRENT_PATH)), "src")

def run_shell_command(cmd, shell=True):
    """
    run shell command
    """
    result = ""
    try:
        result = check_output([cmd], shell=shell, stderr=None)
    except:
        pass
    if result:
        return result.decode('utf-8')
    return result

def run_udp_server():
    """
    go to the src folder and compile, then run udp_server
    return 0 if running server succesfully, otherwise -1
    """
    result = run_shell_command("netstat -anp | grep 127.0.0.1:53")
    if result:
        print("There is already processing running, please kill it before run this test")
        return -1
    # remove old binary
    run_shell_command("cd {0}; make all".format(_SRC_PATH))
    os.system("{0}/udp_server &".format(_SRC_PATH))
    time.sleep(1) # shortcut to make sure server is running, should be another better way to check
    result = run_shell_command("netstat -anp | grep 127.0.0.1:53")
    if result:
        print("run udp server success")
    return 0

def run_udp_client():
    """
    go to the src folder and run udp_client
    return 0 if running server succesfully, otherwise -1
    """
    # remove old binary
    result = run_shell_command("{0}/udp_client".format(_SRC_PATH))
    if not "Client is to close" in result:
        printf("Failed to run client")
        return -1
    return 0

def send_udp_packet():
    """
    Send a udp packet to 127.0.0.1:53
    """
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto("1234567", ("127.0.0.1", 53))

def test_udp_server():
    """
    run udp server, then send a message to this server, check if server is closed after that
    """
    if run_udp_server() != 0:
        print("Failed to run udp server, test case failed")
        sys.exit(-1)
    
    send_udp_packet()

    result = run_shell_command("netstat -anp | grep 127.0.0.1:53")

    if result:
        print("udp server is still running, test case failed")
        sys.exit(-1)

def test_udp_server_and_client():
    """
    run udp server, then run udp client, check if server is closed after client running
    """
    if run_udp_server() != 0:
        print("Failed to run udp server, test case failed")
        sys.exit(-1)

    if run_udp_client() != 0:
        print("Failed to run udp client, test case failed")
        sys.exit(-1) 

    result = run_shell_command("netstat -anp | grep 127.0.0.1:53")
    if result:
        print("Udp server is still running, test case failed")
        sys.exit(-1)

def main():
    test_udp_server()
    print("Test case passed: test_udp_server")

    test_udp_server_and_client()
    print("Test case passed: test_udp_server_and_client")

if __name__ == "__main__":
    main()
