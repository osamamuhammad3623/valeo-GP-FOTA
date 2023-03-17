import socket
from network_definitions import *


def talk_to_server(data):
    data = str(data) # convert data to string to be able to encode it
    encoded_data = data.encode("utf-8")
    msg_length = str(len(encoded_data)).encode('utf-8')
    msg_length += b' ' * (MSG_LENGTH - len(msg_length)) # fill the message with padding

    client.send(msg_length)
    client.send(encoded_data)

    # wait for server response
    print(client.recv(MSG_LENGTH).decode('utf-8'))



ADDRESS = (SERVER_ADDRESS, PORT)
client=  socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDRESS)

talk_to_server("Are you listening?")