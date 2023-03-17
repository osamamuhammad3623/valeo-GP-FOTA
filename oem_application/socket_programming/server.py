import socket
import threading
import ast
from network_definitions import *


def client_handle(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected")
    
    connected = 1
    while connected:
        # receive data length first
        msg_length = conn.recv(MSG_LENGTH).decode('utf-8')
        if msg_length : # not none
            msg_length = int(msg_length)
            # wait until receiving "msg_length" bytes
            data = conn.recv(msg_length).decode('utf-8')
            if data == DISCONN_MSG:
                connected=0
            print("[Metadata RECEIVED]")
            print(data)
            # use  ast.literal_eval(string_dicitonary) to convert from str to dictionary
            print("[SIGNING]")

            # the signing process starts here!
           



            # send the certificates back to the client
            print("[SENDING CERTIFICATES]")
            output_msg = "I'm here!".encode("utf-8")
            output_msg += b' ' * (MSG_LENGTH - len(output_msg))
            conn.send(output_msg)
    
    conn.close()


def start():
    print("[SERVER IS RUNNING]")
    server.listen()
    print("[LISTENING]")
    while True:
        conn, addr = server.accept()

        # create a separate thread to handle the new client while listning
        thread = threading.Thread(target=client_handle, args=(conn,addr))
        thread.start()


# get server IP and create the address (server + port)
SERVER = socket.gethostbyname(socket.gethostname())
ADDRESS = (SERVER, PORT)
# init server
server=  socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDRESS)

start()