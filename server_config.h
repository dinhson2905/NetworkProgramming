#define PORT 8888
#define CLIENT_MAX 20   /* Number of allowed connections */

#define LENGTH_MSG 1024
#define BUFF_SIZE 1024

#define ROOM_SIZE 2
#define ROOM_MAX 4
 
#define NEW_CLIENT_SUCESS "new_client_success"
#define NEW_CLIENT_ERROR "new_client_error"

#define ROOM_PENDING 0
#define ROOM_RUNNING 1

typedef struct Client {
    int connfd;
    int room_id;
    char name[100];
} Client;

typedef struct Room {
    int id;
    int client_num;
    int status;
} Room;

typedef struct Answer {
    int q_num;
    int q_option;
} Answer;

#define ROOM_FULL_NOTIFY "Phòng chơi đã đầy. Hãy chọn phòng khác"
#define ROOM_STARTED_NOTIFY "Phòng chơi đã bắt đầu. Hãy chọn phòng khác"