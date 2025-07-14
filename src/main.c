#include <stdio.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#elif defined(__linux__)
#include <sys/utsname.h>
#endif
void detect_arch(char *arch, size_t size) {
#if defined(_WIN32) || defined(_WIN64)
    SYSTEM_INFO sysinfo;
    GetNativeSystemInfo(&sysinfo);

    switch (sysinfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            strncpy(arch, "x86_64", size);
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            strncpy(arch, "x86", size);
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            strncpy(arch, "arm64", size);
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            strncpy(arch, "arm", size);
            break;
        default:
            strncpy(arch, "unknown", size);
            break;
    }
#elif defined(__APPLE__)
    size_t len = size;
    sysctlbyname("hw.machine", arch, &len, NULL, 0);
#elif defined(__linux__)
    struct utsname uname_data;
    if (uname(&uname_data) == 0) strncpy(arch, uname_data.machine, size);
    else strncpy(arch, "unknown", size);
#else
    strncpy(arch, "unknown", size);
#endif
    arch[size - 1] = '\0'; // Ensure null-termination
}
int main() {
    char arch[64];
    detect_arch(arch, sizeof(arch));
    printf("Detected architecture: %s\n", arch);
    return 0;
}
