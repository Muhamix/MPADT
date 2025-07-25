#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined (__linux__)
#include <sys/utsname.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#include "../include/lexer.h"
#include "../include/parser.h"
int ends_with(const char *str, const char *suff) {
    if (!str || !suff) return 0;
    size_t len = strlen(str);
    size_t slen = strlen(suff);
    if (slen > len) return 0;
    return strcmp(str + len - slen, suff) == 0;
}
const char *detect_arch() {
    #if defined(__linux__)
        struct utsname buf;
        uname(&buf);
        if (strcmp(buf.machine, "x86_64") == 0) return "x86";
        if (strcmp(buf.machine, "aarch64") == 0) return "arm";
        return NULL;
    #elif defined(__APPLE__)
        char arch[32];
        size_t size = sizeof(arch);
        if (sysctlbyname("hw.optional.arm64", &arch, &size, NULL, 0) == 0) {
            int isarm = 0;
            sysctlbyname("hw.optional.arm64", &arch, &size, NULL, 0);
            return isarm ? "arm" : "x86";
        }
        return NULL;
    #elif defined(_WIN32) || defined(_WIN64)
        SYSTEM_INFO sysinfo;
        GetNativeSystemInfo(&sysinfo);
        switch (sysinfo.wProcessorArchitecture) {
            case PROCESSOR_ARCHITECTURE_INTEL:
            case PROCESSOR_ARCHITECTURE_AMD64:
                return "x86";
            case PROCESSOR_ARCHITECTURE_ARM:
            case PROCESSOR_ARCHITECTURE_ARM64:
                return "arm";
            default:
                return NULL;
    }
    #endif
    return NULL;
}
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file.mpadt>\n", argv[0]);
        return 1;
    }
    if (!ends_with(argv[1], ".mpadt")) {
        printf("Unknown file type: please make sure the extention is .mpadt\n");
        exit(1);
    }
    char path[256];
    char *last_slash = strrchr(argv[1], '/');
    if (last_slash != NULL) {
        size_t len = last_slash - argv[1] + 1;
        strncpy(path, argv[1], len);
        path[len] = '\0';
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) { perror("fopen"); return 1; }
    fseek(f, 0, SEEK_END); long fsize = ftell(f); rewind(f);
    char *source = malloc(fsize + 1);
    fread(source, 1, fsize, f); source[fsize] = '\0'; fclose(f);
    init_lexer(source);
    parse_program();
    int count;
    ArchRule *rules = get_arch_rules(&count);
    const char *host_arch = detect_arch();
    if (!host_arch) {
        fprintf(stderr, "Unknown architecture\n");
        return 1;
    }
    int matched = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(rules[i].arch_name, host_arch) == 0) {
            matched = 1;
            rules[i].files.file = strcat(path, rules[i].files.file);
            if (access(rules[i].files.file, F_OK) != 0) {
                fprintf(stderr, "ERROR: File not found: %s\n", rules[i].files.file);
                return 1;
            }
            char cmd[512];
            if (strcmp(host_arch, "x86") == 0) snprintf(cmd, sizeof(cmd), "as --32 -o out.o %s && ld -m elf_i386 -o out out.o && ./out", rules[i].files.file);
            else if (strcmp(host_arch, "arm") == 0) snprintf(cmd, sizeof(cmd), "aarch64-linux-gnu-as -o out.o %s && aarch64-linux-gnu-ld -o out out.o && qemu-aarch64 ./out", rules[i].files.file);
            else {
                fprintf(stderr, "Unsupported architecture: %s\n", host_arch);
                return 1;
            }
            int res = system(cmd);
            if (res != 0) fprintf(stderr, "Command failed: %s\n", cmd);
            else {remove("out");remove("out.o");}
            break;
        }
    }
    if (!matched) fprintf(stderr, "No rule found for architecture: %s\n", host_arch);
    free(source);
    return 0;
}
