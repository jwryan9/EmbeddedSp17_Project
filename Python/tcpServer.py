import socket
import generateScoreData
import readData
import datetime;
from time import sleep

LOCAL_IP = '127.0.0.1'
TCP_IP = '10.0.1.3'
TCP_PORT = 5005
BUFFER_SIZE = 1024

# data = readData.read_data('../Data/gameData.csv')

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

# Infinitely generate data and send it when TCP connection made
while 1:
    generateScoreData.generate_score_data()
    sleep(1)
    data = readData.read_data('../Data/gameData.csv')
    conn, addr = s.accept()
    print("Connection address: ", addr)
    conn.send(data)
    print('sent data: ', data)
    print('datetime transmited: ', datetime.datetime.now())
    sleep(50.0/1000.0)
    conn.close()
