import serial.tools.list_ports

# List all available serial ports
ports = list(serial.tools.list_ports.comports())

# Print information for each port
for port in ports:
    print(port.device + ' - ' + port.description)
