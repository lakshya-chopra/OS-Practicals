#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_kernel_version() {
    FILE *file = fopen("/proc/version", "r");
    if (file == NULL) {
        perror("Failed to open /proc/version");
        return;
    }
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Kernel Version: %s", buffer);
    }
    fclose(file);
}

void get_cpu_info() {
    FILE *file = fopen("/proc/cpuinfo", "r");
    if (file == NULL) {
        perror("Failed to open /proc/cpuinfo");
        return;
    }
    char buffer[256];
    char *cpu_model = NULL;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "model name", 10) == 0) {
            cpu_model = strchr(buffer, ':') + 2;  // Skip ": "
            break;
        }
    }

    if (cpu_model != NULL) {
        printf("CPU Model: %s", cpu_model);
    } else {
        printf("CPU Model information not found.\n");
    }

    fclose(file);
}

int main() {
    printf("Linux Kernel and CPU Information\n");
    printf("--------------------------------\n");

    // Get kernel version
    get_kernel_version();
    printf("\n");

    // Get CPU information
    get_cpu_info();

    return 0;
}
