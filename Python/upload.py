import getpass
import paramiko
import os

host = "c-lnx000.engr.uiowa.edu"
server_port = 22

server_root_adr = "/user/j/jwryan/WINDOWS_H_DRIVE/public_html/Embedded/project/"


# Getting the users Eningeering username and ID for transferring the files later in the script
username = raw_input("\nEnter engineering-ID: ")
password = getpass.getpass("Enter engineering password: ")
confirm_pass = getpass.getpass("Re-enter engineering password: ")
# While the passwords don't match, keep asking. hopefully the person types it correctly
while password != confirm_pass:
    print "Error: Passwords do not match."
    password = getpass.getpass("Enter engineering password: ")
    confirm_pass = getpass.getpass("Re-enter engineering password: ")
print ""

print "Uploading file"
transport = paramiko.Transport(host, server_port)
transport.connect(username=username, password=password)
sftp = paramiko.SFTPClient.from_transport(transport)
sftp.put("/Users/jasonryan/PycharmProjects/EmbeddedSp17_Project/Data/master_scoreboard.json", os.path.join(server_root_adr, "master_scoreboard.json"))
sftp.close()
print "\nDone"
