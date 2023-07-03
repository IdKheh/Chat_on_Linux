#if !defined(__BOARD_H)
#define __BOARD_H
struct Message {
    long mtype;
    char mtext[1024];
};
#endif