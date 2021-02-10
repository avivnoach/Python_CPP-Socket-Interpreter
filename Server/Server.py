
import socket
import sys
from io import StringIO
import contextlib
from _thread import *
import threading
from RestrictedPython import compile_restricted
from RestrictedPython import safe_globals

MAX_MSG_LEN = 100
print_lock = threading.Lock()


@contextlib.contextmanager
def stdoutIO(stdout=None):
    """
    function used to get the output of the exec function, because it's running on "internal" interpreter
    :param stdout: contains the location of the new stream
    """
    old = sys.stdout
    if stdout is None:
        stdout = StringIO()
    sys.stdout = stdout
    yield stdout
    sys.stdout = old


# thread function
def threaded(c):
    """
    function handles each client separately, so using threads
    :param c: socket object usable to send and receive data on the connection
    :return: none
    """
    c.send("server connected".encode())
    while True:

        # data received from client
        try:
            data = c.recv(1024)
            if not data:
                print('Disconnecting from client...')

                # lock released on exit
                print_lock.release()
                break

            with stdoutIO() as s:
                byte_code = compile_restricted(data, safe_globals, '<inline>', 'exec')


            # send back reversed string to client
            c.send(s.getvalue().encode())
        except ConnectionResetError:
            print("Client lost connection...")
            print_lock.release()
            break
        except Exception as e:
            print(e.args)

    # connection closed
    c.close()


def Main():
    host = ""

    port = 2357
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((host, port))
    print("socket bound to port", port)

    # put the socket into listening mode
    s.listen()
    print("socket is listening")

    try:
        loc = {}
        source_code = """
    open('requirements.txt', 'w')"""
        byte_code = compile_restricted(source_code, '<inline>', 'exec')
        exec(byte_code, safe_globals, loc)
    except Exception as e:
        print()

    # a forever loop until client wants to exit
    while True:
        # establish connection with client
        c, addr = s.accept()

        # lock acquired by client
        print_lock.acquire()
        print('Connected to :', addr[0], ':', addr[1])

        # Start a new thread and return its identifier
        start_new_thread(threaded, (c,))
    s.close()


if __name__ == '__main__':
    Main()
