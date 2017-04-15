import getpass
import paramiko
import os
import socket

server_address = "c-lnx000.engr.uiowa.edu"
host = "c-lnx000.engr.uiowa.edu"
server_port = 22

server_root_adr = "/user/j/jwryan/WINDOWS_H_DRIVE/public_html/Embedded/project/"

"""
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
"""
username = "jwryan"
password = "jr4ia3908"

print "Uploading file"
transport = paramiko.Transport(host, server_port)
transport.connect(username=username, password=password)
sftp = paramiko.SFTPClient.from_transport(transport)
sftp.put("/Users/jasonryan/Desktop/test.txt", os.path.join(server_root_adr, "test.txt"))
sftp.close()
print "\nDone"
