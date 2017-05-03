import socket
import generateScoreData
import readData
from time import sleep

TCP_IP = '10.0.1.2'
TCP_PORT = 5005
BUFFER_SIZE = 1024

# data = readData.read_data('../Data/gameData.csv')

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

while 1:
    generateScoreData.generate_score_data()
    sleep(5)
    data = readData.read_data('../Data/gameData.csv')
    conn, addr = s.accept()
    print("Connection address: ", addr)
    conn.send(data)
    print('sent data: ', data)
    sleep(50.0/1000.0)
    conn.close()
