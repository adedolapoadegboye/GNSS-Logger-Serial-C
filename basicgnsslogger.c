#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

#define MAX_PORT_LENGTH 256
#define MAX_PORTS 20
#define MAX_TEST_DURATION 86400

// Function to scan available serial ports dynamically
int get_available_ports(char ports[MAX_PORTS][MAX_PORT_LENGTH])
{
    struct dirent *entry;
    DIR *dp = opendir("/dev");
    int count = 0;

    if (dp == NULL)
    {
        perror("Error opening /dev directory");
        return 0;
    }

    while ((entry = readdir(dp)) != NULL && count < MAX_PORTS)
    {
        if (strncmp(entry->d_name, "ttyUSB", 6) == 0 || strncmp(entry->d_name, "ttyS", 4) == 0 || strncmp(entry->d_name, "serial", 6) == 0)
        {
            snprintf(ports[count], MAX_PORT_LENGTH, "/dev/%s", entry->d_name);
            count++;
        }
    }

    closedir(dp);
    return count;
}

// Function to display available options and get user selection
void get_user_input(char *port, int *baud_rate, int *duration)
{
    char ports[MAX_PORTS][MAX_PORT_LENGTH];
    int num_ports = get_available_ports(ports);
    int baud_rates[] = {9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
    int port_choice, baud_choice;

    // Select port dynamically
    if (num_ports == 0)
    {
        printf("No available serial ports found.\n");
        exit(1);
    }

    printf("Available Serial Ports:\n");
    for (int i = 0; i < num_ports; i++)
    {
        printf("%d: %s\n", i + 1, ports[i]);
    }
    do
    {
        printf("Select a serial port (1-%d): ", num_ports);
        scanf("%d", &port_choice);
    } while (port_choice < 1 || port_choice > num_ports);
    strcpy(port, ports[port_choice - 1]);

    // Select baud rate
    printf("Available Baud Rates:\n");
    for (int i = 0; i < sizeof(baud_rates) / sizeof(baud_rates[0]); i++)
    {
        printf("%d: %d\n", i + 1, baud_rates[i]);
    }
    do
    {
        printf("Select a baud rate (1-%lu): ", sizeof(baud_rates) / sizeof(baud_rates[0]));
        scanf("%d", &baud_choice);
    } while (baud_choice < 1 || baud_choice > (sizeof(baud_rates) / sizeof(baud_rates[0])));
    *baud_rate = baud_rates[baud_choice - 1];

    // Select test duration
    do
    {
        printf("Enter test duration (seconds, min: 1, max: 86400): ");
        scanf("%d", duration);
    } while (*duration < 1 || *duration > MAX_TEST_DURATION);
}

// Function to set up the serial port
int setup_serial(const char *port, int baud_rate)
{
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        perror("Error opening serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    options.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

// Function to generate a timestamped log filename
void get_log_filename(char *filename, size_t size)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(filename, size, "logs/gnss_log_%Y%m%d_%H%M%S.txt", tm_info);
}

// Function to log GNSS data for a specified duration
void log_gnss_data(int fd, const char *filename, int duration)
{
    FILE *logfile = fopen(filename, "w");
    if (!logfile)
    {
        perror("Error opening log file");
        return;
    }

    printf("Logging GNSS data to %s for %d seconds...\n", filename, duration);

    char buffer[256];
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < duration)
    {
        int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';
            fprintf(logfile, "%s", buffer);
            printf("%s", buffer); // Print to console (optional)
        }
        usleep(100000); // Small delay (100ms)
    }

    fclose(logfile);
    printf("Logging complete. Data saved to %s\n", filename);
}

int main()
{
    // Ensure the logs directory exists
    system("mkdir -p logs");

    char selected_port[MAX_PORT_LENGTH];
    int selected_baud_rate;
    int test_duration;

    // Get user input for serial port, baud rate, and duration
    get_user_input(selected_port, &selected_baud_rate, &test_duration);

    // Open serial port
    int serial_fd = setup_serial(selected_port, selected_baud_rate);
    if (serial_fd == -1)
    {
        return 1;
    }

    // Generate log filename
    char log_filename[512];
    get_log_filename(log_filename, sizeof(log_filename));

    // Start logging GNSS data
    log_gnss_data(serial_fd, log_filename, test_duration);

    // Close serial port
    close(serial_fd);
    return 0;
}
