#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
//#include <string_view>

#include <cstdint>

#define FDQUEUE_MAX_FD 16
struct fdqueue {
    int fd[FDQUEUE_MAX_FD];
    int nfd;
};

struct Connection {
    static constexpr int BUFF_BLOCK_SZ { 4096 };
    static constexpr int BUFF_BLOCK_CT { 16 };
    static constexpr int BUFF_SZ       { BUFF_BLOCK_CT * BUFF_BLOCK_SZ };

    // TBD make id, start_time, from, client_fd, server_fd const and set in ctor
    int id;               // unique serial number
    uint64_t start_time;  // timestamp of connection creation (seconds since Unix Epoch)
    std::string from;     // allocated string describing client address (x.x.x.x:port) for AF_INET or socket file path/"unknown(local)" for AF_UNIX
    int client_fd;        // socket accept(2)ed from x client
    int server_fd;        // socket connect(2)ed to x server
    uint8_t clientbuffer[BUFF_SZ];
    unint32_t clientcount;              // current amount of bytes used in client_buffer
    unint32_t clientignore;
    /*struct */fdqueue clientfdq;
    uint8_t serverbuffer[BUFF_SZ];
    uint32_t servercount;               // current amount of bytes used in server_buffer
    uint32_t serverignore;
    /*struct */fdqueue serverfdq;
//    const bool bigendian;
//    enum client_state { c_start = 0, c_normal, c_amlost } client_state;
//    enum server_state { s_start=0, s_normal, s_amlost } server_state;
//    struct expectedreply* expectedreplies;
//    uint64_t seq;
//    struct usedextension* usedextensions;
//    struct unknownextension* waiting;
//    struct unknownextension* unknownextensions;
};


#endif  // CONNECTION_HPP
