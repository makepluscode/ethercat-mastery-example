// Newlib stubs for stdio; _write is RTT (SEGGER_RTT_Syscalls_GCC.c).
#include <errno.h>
#include <sys/stat.h>

#include <cstring>

extern "C" {

int _read(int /*file*/, char* /*ptr*/, int /*len*/) {
    errno = EINVAL;
    return -1;
}

int _isatty(int file) {
    return ((file == 1) || (file == 2)) ? 1 : 0;
}

int _fstat(int file, struct stat* st) {
    if ((file < 0) || (st == nullptr)) {
        errno = EINVAL;
        return -1;
    }
    if ((file == 0) || (file == 1) || (file == 2)) {
        std::memset(st, 0, sizeof(*st));
        st->st_mode = S_IFCHR;
        return 0;
    }
    errno = EBADF;
    return -1;
}

} // extern "C"
