#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to fetch and display kernel information
void get_kernel_info() {
    printf("---------------------- Kernel Information --------------------------\n\n");
    system("cat /proc/version_signature | awk '{printf \"Kernel Version: %s\\n\", $0}'");
}

// Function to fetch and display memory information
void get_memory_info() {
    printf("\n---------------------- Memory Status -------------------------------\n");

    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    char label[64];
    unsigned long value;
    char unit[8];

    while (fscanf(file, "%63s %lu %7s", label, &value, unit) == 3) {
        if (strcmp(label, "MemTotal:") == 0) {
            printf("Total Memory: %.2f GB\n", value / (1024.0 * 1024.0));
        } else if (strcmp(label, "MemFree:") == 0) {
            printf("Free Memory: %.2f GB\n", value / (1024.0 * 1024.0));
        } else if (strcmp(label, "MemAvailable:") == 0) {
            printf("Available Memory: %.2f GB\n", value / (1024.0 * 1024.0));
        }
    }

    fclose(file);
}

int main() {
    printf("===================================================================\n");
    printf("                          System Information                       \n");
    printf("===================================================================\n\n");

    get_kernel_info();
    get_memory_info();

    printf("\n===================================================================\n");

    return 0;
}
