#define PORT 8888
#define CLIENT_MAX 20   /* Number of allowed connections */

#define LENGTH_MSG 1024 /*độ dài của một message*/
#define BUFF_SIZE 1024 /*độ dài của một buffer*/

#define ROOM_SIZE 2 /*1 room tối đa 2 người*/
#define ROOM_MAX 4 /*tối đa 4 room*/
#define MAX_TURN 3 /*tối đa 3 lượt chơi*/

#define NEW_CLIENT_SUCESS "new_client_success"
#define NEW_CLIENT_ERROR "new_client_error"
#define ROOM_FULL_NOTIFY "Phòng chơi đã đầy. Hãy chọn phòng khác"
#define ROOM_STARTED_NOTIFY "Phòng chơi đã bắt đầu. Hãy chọn phòng khác"


#define ROOM_PENDING 0
#define ROOM_RUNNING 1

typedef struct Client {
    int connfd;
    int room_id;
    int turn;
    char name[100];
    int grade;
} Client;

typedef struct Room {
    int id;
    int client_num;
    int status;
    int turn_now;
    //Question question;
} Room;

typedef struct Answer {
    int q_num;
    int q_option;
} Answer;


Room room_arr[ROOM_MAX];
char boolean[2][10] = {"false", "true"};
Client client_arr[CLIENT_MAX];
int client_num = 0;
struct sockaddr_in server, client;
