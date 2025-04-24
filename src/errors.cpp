#include <system_error>
#include <string>
#include <array>
#include <string_view>
#include <unordered_map>

#include <cassert>

#include <errno.h>
#include <netdb.h>        // gai_strerror
#include <string.h>       // strerror

#include "errors.hpp"


namespace errors {

namespace system {

namespace detail {

static constexpr int MAX_ERRNO { EHWPOISON };
static constexpr std::array<
    std::string_view, MAX_ERRNO + 1 > errno_names {
    "(errno not set)",  // 0
    "EPERM",            // 1 /* Operation not permitted */
    "ENOENT",           // 2 /* No such file or directory */
    "ESRCH",            // 3 /* No such process */
    "EINTR",            // 4 /* Interrupted system call */
    "EIO",              // 5 /* I/O error */
    "ENXIO",            // 6 /* No such device or address */
    "E2BIG",            // 7 /* Argument list too long */
    "ENOEXEC",          // 8 /* Exec format error */
    "EBADF",            // 9 /* Bad file number */
    "ECHILD",           // 10 /* No child processes */
    "EAGAIN"            // 11 /* Try again */
    " (EWOULDBLOCK)",   //    /* Operation would block */
    "ENOMEM",           // 12 /* Out of memory */
    "EACCES",           // 13 /* Permission denied */
    "EFAULT",           // 14 /* Bad address */
    "ENOTBLK",          // 15 /* Block device required */
    "EBUSY",            // 16 /* Device or resource busy */
    "EEXIST",           // 17 /* File exists */
    "EXDEV",            // 18 /* Cross-device link */
    "ENODEV",           // 19 /* No such device */
    "ENOTDIR",          // 20 /* Not a directory */
    "EISDIR",           // 21 /* Is a directory */
    "EINVAL",           // 22 /* Invalid argument */
    "ENFILE",           // 23 /* File table overflow */
    "EMFILE",           // 24 /* Too many open files */
    "ENOTTY",           // 25 /* Not a typewriter */
    "ETXTBSY",          // 26 /* Text file busy */
    "EFBIG",            // 27 /* File too large */
    "ENOSPC",           // 28 /* No space left on device */
    "ESPIPE",           // 29 /* Illegal seek */
    "EROFS",            // 30 /* Read-only file system */
    "EMLINK",           // 31 /* Too many links */
    "EPIPE",            // 32 /* Broken pipe */
    "EDOM",             // 33 /* Math argument out of domain of func */
    "ERANGE",           // 34 /* Math result not representable */
    "EDEADLK"           // 35 /* Resource deadlock would occur */
    " (EDEADLOCK)",
    "ENAMETOOLONG",     // 36 /* File name too long */
    "ENOLCK",           // 37 /* No record locks available */
    "ENOSYS",           // 38 /* Invalid system call number */
    "ENOTEMPTY",        // 39 /* Directory not empty */
    "ELOOP",            // 40 /* Too many symbolic links encountered */
    "(undefined)",      // 41
    "ENOMSG",           // 42 /* No message of desired type */
    "EIDRM",            // 43 /* Identifier removed */
    "ECHRNG",           // 44 /* Channel number out of range */
    "EL2NSYNC",         // 45 /* Level 2 not synchronized */
    "EL3HLT",           // 46 /* Level 3 halted */
    "EL3RST",           // 47 /* Level 3 reset */
    "ELNRNG",           // 48 /* Link number out of range */
    "EUNATCH",          // 49 /* Protocol driver not attached */
    "ENOCSI",           // 50 /* No CSI structure available */
    "EL2HLT",           // 51 /* Level 2 halted */
    "EBADE",            // 52 /* Invalid exchange */
    "EBADR",            // 53 /* Invalid request descriptor */
    "EXFULL",           // 54 /* Exchange full */
    "ENOANO",           // 55 /* No anode */
    "EBADRQC",          // 56 /* Invalid request code */
    "EBADSLT",          // 57 /* Invalid slot */
    "(undefined)",      // 58
    "EBFONT",           // 59 /* Bad font file format */
    "ENOSTR",           // 60 /* Device not a stream */
    "ENODATA",          // 61 /* No data available */
    "ETIME",            // 62 /* Timer expired */
    "ENOSR",            // 63 /* Out of streams resources */
    "ENONET",           // 64 /* Machine is not on the network */
    "ENOPKG",           // 65 /* Package not installed */
    "EREMOTE",          // 66 /* Object is remote */
    "ENOLINK",          // 67 /* Link has been severed */
    "EADV",             // 68 /* Advertise error */
    "ESRMNT",           // 69 /* Srmount error */
    "ECOMM",            // 70 /* Communication error on send */
    "EPROTO",           // 71 /* Protocol error */
    "EMULTIHOP",        // 72 /* Multihop attempted */
    "EDOTDOT",          // 73 /* RFS specific error */
    "EBADMSG",          // 74 /* Not a data message */
    "EOVERFLOW",        // 75 /* Value too large for defined data type */
    "ENOTUNIQ",         // 76 /* Name not unique on network */
    "EBADFD",           // 77 /* File descriptor in bad state */
    "EREMCHG",          // 78 /* Remote address changed */
    "ELIBACC",          // 79 /* Can not access a needed shared library */
    "ELIBBAD",          // 80 /* Accessing a corrupted shared library */
    "ELIBSCN",          // 81 /* .lib section in a.out corrupted */
    "ELIBMAX",          // 82 /* Attempting to link in too many shared libraries */
    "ELIBEXEC",         // 83 /* Cannot exec a shared library directly */
    "EILSEQ",           // 84 /* Illegal byte sequence */
    "ERESTART",         // 85 /* Interrupted system call should be restarted */
    "ESTRPIPE",         // 86 /* Streams pipe error */
    "EUSERS",           // 87 /* Too many users */
    "ENOTSOCK",         // 88 /* Socket operation on non-socket */
    "EDESTADDRREQ",     // 89 /* Destination address required */
    "EMSGSIZE",         // 90 /* Message too long */
    "EPROTOTYPE",       // 91 /* Protocol wrong type for socket */
    "ENOPROTOOPT",      // 92 /* Protocol not available */
    "EPROTONOSUPPORT",  // 93 /* Protocol not supported */
    "ESOCKTNOSUPPORT",  // 94 /* Socket type not supported */
    "EOPNOTSUPP",       // 95 /* Operation not supported on transport endpoint */
    "EPFNOSUPPORT",     // 96 /* Protocol family not supported */
    "EAFNOSUPPORT",     // 97 /* Address family not supported by protocol */
    "EADDRINUSE",       // 98 /* Address already in use */
    "EADDRNOTAVAIL",    // 99 /* Cannot assign requested address */
    "ENETDOWN",         // 100 /* Network is down */
    "ENETUNREACH",      // 101 /* Network is unreachable */
    "ENETRESET",        // 102 /* Network dropped connection because of reset */
    "ECONNABORTED",     // 103 /* Software caused connection abort */
    "ECONNRESET",       // 104 /* Connection reset by peer */
    "ENOBUFS",          // 105 /* No buffer space available */
    "EISCONN",          // 106 /* Transport endpoint is already connected */
    "ENOTCONN",         // 107 /* Transport endpoint is not connected */
    "ESHUTDOWN",        // 108 /* Cannot send after transport endpoint shutdown */
    "ETOOMANYREFS",     // 109 /* Too many references: cannot splice */
    "ETIMEDOUT",        // 110 /* Connection timed out */
    "ECONNREFUSED",     // 111 /* Connection refused */
    "EHOSTDOWN",        // 112 /* Host is down */
    "EHOSTUNREACH",     // 113 /* No route to host */
    "EALREADY",         // 114 /* Operation already in progress */
    "EINPROGRESS",      // 115 /* Operation now in progress */
    "ESTALE",           // 116 /* Stale file handle */
    "EUCLEAN",          // 117 /* Structure needs cleaning */
    "ENOTNAM",          // 118 /* Not a XENIX named type file */
    "ENAVAIL",          // 119 /* No XENIX semaphores available */
    "EISNAM",           // 120 /* Is a named type file */
    "EREMOTEIO",        // 121 /* Remote I/O error */
    "EDQUOT",           // 122 /* Quota exceeded */
    "ENOMEDIUM",        // 123 /* No medium found */
    "EMEDIUMTYPE",      // 124 /* Wrong medium type */
    "ECANCELED",        // 125 /* Operation Canceled */
    "ENOKEY",           // 126 /* Required key not available */
    "EKEYEXPIRED",      // 127 /* Key has expired */
    "EKEYREVOKED",      // 128 /* Key has been revoked */
    "EKEYREJECTED",     // 129 /* Key was rejected by service */
    "EOWNERDEAD",       // 130 /* Owner died */
    "ENOTRECOVERABLE",  // 131 /* State not recoverable */
    "ERFKILL",          // 132 /* Operation not possible due to RF-kill */
    "EHWPOISON"         // 133 /* Memory page has hardware error */
};

}  // namespace detail

const std::system_error
exception( const std::string& msg ) {
    assert( errno > -1 );
    assert( errno <= detail::MAX_ERRNO );
    return std::system_error(
        errno, std::generic_category(),
        msg + ( msg.empty() ? "" : ": " ) +
        "errno " + detail::errno_names[errno].data()
        );
}

std::string
message( const std::string& msg ) {
    return msg + ( msg.empty() ? "" : ": " ) +
        "errno " + detail::errno_names[errno].data() +
        ": " + strerror(errno);
}

}  // namespace errors::system

namespace getaddrinfo {

namespace detail {

const char*
Category::name() const noexcept {
    return "getaddrinfo";
}

std::string
Category::message( int i ) const {
    return gai_strerror( i );
}

static const std::unordered_map<
    int, std::string_view> gai_error_names {
    { -1,   "EAI_BADFLAGS" },     // /* Invalid value for `ai_flags' field.  */
    { -2,   "EAI_NONAME" },       // /* NAME or SERVICE is unknown.  */
    { -3,   "EAI_AGAIN" },        // /* Temporary failure in name resolution.  */
    { -4,   "EAI_FAIL" },         // /* Non-recoverable failure in name res.  */
    { -5,   "EAI_NODATA" },       // /* No address associated with NAME.  */
    { -6,   "EAI_FAMILY" },       // /* `ai_family' not supported.  */
    { -7,   "EAI_SOCKTYPE" },     // /* `ai_socktype' not supported.  */
    { -8,   "EAI_SERVICE" },      // /* SERVICE not supported for `ai_socktype'.  */
    { -9,   "EAI_ADDRFAMILY" },   // /* Address family for NAME not supported.  */
    { -10,  "EAI_MEMORY" },       // /* Memory allocation failure.  */
    { -11,  "EAI_SYSTEM" },       // /* System error returned in `errno'.  */
    { -12,  "EAI_OVERFLOW" },     // /* Argument buffer overflow.  */
    { -100, "EAI_INPROGRESS" },   // /* Processing request in progress.  */
    { -101, "EAI_CANCELED" },     // /* Request canceled.  */
    { -102, "EAI_NOTCANCELED" },  // /* Request not canceled.  */
    { -103, "EAI_ALLDONE" },      // /* All requests done.  */
    { -104, "EAI_INTR" },         // /* Interrupted by a signal.  */
    { -105, "EAI_IDN_ENCODE" }    // /* IDN encoding failed.  */
};

}  // namepace errors::getaddrinfo::detail

const std::error_category& category() {
    static detail::Category instance;
    return instance;
}

const std::system_error
exception( const int gai_ret, const std::string& msg ) {
    assert( gai_ret < 0 );
    return std::system_error(
        errorCode( gai_ret ),
        msg + ( msg.empty() ? "" : ": " ) +
        "retval " + detail::gai_error_names.at( gai_ret ).data()
        );
}

std::string
message( const int gai_ret, const std::string& msg ) {
    return msg + ( msg.empty() ? "" : ": " ) +
        "retval " + detail::gai_error_names.at( gai_ret ).data() +
        ": " + gai_strerror( gai_ret );
}

}  // namespace errors::getaddrinfo

}  // namespace errors
