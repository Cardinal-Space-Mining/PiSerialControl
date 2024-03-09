#include "../include/SerialMotorCtrl.h"

// C library headers
#include <cstdio>
#include <cstring>
#include <cstdlib>

// Linux headers
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()

#include <sys/file.h> //flock()

constexpr const char XON = 0x13;
constexpr const char XOFF = 0x11;

// See https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
MotorSerialConnection::MotorSerialConnection(const char *port)
{
open_again:
    serial_fd = open(port, O_RDWR);

    // Check for errors
    if (serial_fd < 0)
    {
        if (errno == EINTR)
        {
            goto open_again;
        }
        std::printf("Error %i from open: %s\n", errno, strerror(errno));
        std::exit(EXIT_FAILURE);
    }

flock_again:
    if (flock(serial_fd, LOCK_EX | LOCK_NB) == -1)
    {
        if (errno == EINTR)
        {
            goto flock_again;
        }
        std::printf("Error %i from flock: %s\n", errno, strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    // Create new termios struct, we call it 'tty' for convention
    // No need for "= {0}" at the end as we'll immediately write the existing
    // config to this struct
    struct termios tty;

    // Read in existing settings, and handle any error
    // NOTE: This is important! POSIX states that the struct passed to tcsetattr()
    // must have been initialized with a call to tcgetattr() overwise behaviour
    // is undefined
    if (tcgetattr(serial_fd, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity

    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication

    tty.c_cflag |= CS8; // 8 bits per byte

    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control for systems with only 2 wires

    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON; // Disable Canonical mode (data only processed on recipt of a new line)

    tty.c_lflag &= ~ECHO; // Disable echo

    tty.c_lflag &= ~ECHOE; // Disable erasure

    tty.c_lflag &= ~ECHONL; // Disable new-line echo

    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)

    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 1; // Wait for up to .1s (1 deciseconds), returning as soon as any data is received.

    tty.c_cc[VMIN] = 0; // Use blocking reads

    cfsetspeed(&tty, B230400); // Set the baudrate to 230400

    // Save tty settings, also checking for error
    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

MotorSerialConnection::~MotorSerialConnection()
{
again:
    int ret_val = close(serial_fd);

    if (ret_val != 0)
    {
        if (errno == EAGAIN)
        {
            goto again;
        }
        else
        {
            printf("Error %i from close: %s\n", errno, strerror(errno));
            std::exit(EXIT_FAILURE);
        }
    }
}

void read_bytes(int fd, void *buff, size_t num_bytes)
{
again:
    ssize_t r_value = read(fd, buff, num_bytes);

    // Handle Errors with read
    if (r_value < 0)
    {
        if (errno == EINTR || errno == EAGAIN)
        {
            goto again;
        }
        printf("Error %i from read: %s\n", errno, strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    // Handle not reading the required number of bytes
    if (r_value < (ssize_t)num_bytes)
    {
        num_bytes = num_bytes - r_value;
        buff = (void*) ((size_t)buff + (size_t)num_bytes);
        goto again;
    }
}

void write_bytes(int fd, const void *buff, size_t num_bytes)
{
again:
    ssize_t r_value = write(fd, buff, num_bytes);

    // Handle Errors with read
    if (r_value < 0)
    {
        if (errno == EINTR || errno == EAGAIN)
        {
            goto again;
        }
        printf("Error %i from read: %s\n", errno, strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    // Handle not reading the required number of bytes
    if (r_value < (ssize_t)num_bytes)
    {
        num_bytes = num_bytes - r_value;
        buff = (const void*) ((size_t)buff + (size_t)num_bytes);
        goto again;
    }
}

void MotorSerialConnection::send_command(const MotorDataStruct &data)
{
    // Write XON to start comms
    write_bytes(serial_fd, &XON, sizeof(XON));

    // Wait for XON to start our sending
    char first_byte = 0;
    while (first_byte != XON)
    {
        read_bytes(serial_fd, &first_byte, 1);
    }

    SerialMsg msg;
    msg.type = SerialMsgType::MotorMessage;
    msg.mds = data;
    write_bytes(serial_fd, &msg, sizeof(msg));

    SerialResponse resp;
    read_bytes(serial_fd, &resp, sizeof(resp));
    
    char last_byte = 0;
    while (last_byte != XOFF)
    {
        read_bytes(serial_fd, &last_byte, 1);
    }
}
