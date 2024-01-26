import socket

target_host = "127.0.0.1"
target_port = 12345  # Asegúrate de cambiar esto al puerto que esté escuchando el servidor UDP

try:
    # create a socket object
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # send some data (convert string to bytes)
    message = "AAABBBCCC"
    client.sendto(message.encode('utf-8'), (target_host, target_port))

    # receive some data
    data, addr = client.recvfrom(4096)

    print("[*] Received data from {}: {}".format(addr, data.decode('utf-8')))

except Exception as e:
    print("[!] Error: {}".format(e))

finally:
    # close the socket
    client.close()
