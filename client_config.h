#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8888

#define LENGTH_MSG 512
#define BUFF_SIZE 512

#define ROOM_SIZE 2
#define ROOM_NUM 4
#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 600

/* Message */
#define NEW_CLIENT_SUCESS "new_client_success"
#define NEW_CLIENT_ERROR "new_client_error"
#define LEFT_ROOM_MODAL "Bạn chắc chắn muốn rời phòng chơi"
#define ELIMINATED "Bạn đã bị loại"
#define START_GAME "Các người chơi đã sẵn sàng. Bắt đầu nào! :)"


#define TRUE 1
#define FALSE 0 
#define MODAL_YES -8
#define MODAL_NO -9

#define MAX_TURN 3

typedef struct Client {
    int connfd;
    int room_id;
    char name[100];
    int turn;
    int grade;
} Client;

typedef struct Room {
    int id;
    int client_num;
} Room;



int client_sock = 0, running_client = 0;
struct Queue *responses;
Room room_arr[ROOM_NUM];
Client client_arr[ROOM_SIZE];
int running = FALSE;
int in_choose_room = FALSE;
int in_waiting_friend = FALSE;
char client_name[LENGTH_MSG];
int room_id;
Question question;
int handler_id;
char character_entered;
char client_name_temp[LENGTH_MSG];
int grade_temp;
char answer_guess[LENGTH_MSG];

// GUI
GdkPixbuf *image = NULL;
GdkPixbufAnimation * anim = NULL;
GtkWidget *widget = NULL;
GdkPixmap *background = NULL;
GtkStyle *style = NULL;

GtkWidget *window, *box, *table;        
GtkWidget *label_name, *entry_name;
GtkWidget *label_room = NULL, *button_room[ROOM_NUM];
GtkWidget *btn_back, *msg_box = NULL, *scroll_window, *entry_msg, *label_client[ROOM_SIZE];
GtkWidget *label_start, *label_question, *label_answer;
GtkWidget *label_enter_char, *label_test, *label_grade[ROOM_SIZE], *entry_answer, *label_enter_answer;
