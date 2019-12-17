#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "server_config.h"
#include "question.c"

/*--------------------------------*/
void room_init(); /* Khởi tạo 1 phòng */
void str_trim(); /* Xóa các kí tự kể từ index đến kết thúc */
Client get_client(); /* lấy 1 client theo connfd */
void new_client(); /* Tạo thêm 1 thằng client để server xử lý */
int join_room(); /* Kiểm tra xem có được phép join room không, phòng đang chơi hoặc full --> 0*/
char* get_params(); /* Trả về tham số của message. VD: "./newclient son" sẽ trả về "son" */
void send_msg_room(int connfd, int room_id, char *msg); /* Gửi msg cho các thằng client khác cùng phòng */
void send_all_client(int room_id, int connfd, char *msg); /*  */
/*-----------------------------------------*/
Question q;
void room_init() {
	for (int i = 0; i < ROOM_MAX; i++) {
		room_arr[i].id	= i + 1;
		room_arr[i].client_num = 0;
		room_arr[i].status = ROOM_PENDING;
	}
}

void str_trim(char *str, int index) {
	for (int i=strlen(str)-1; i>=index; i--) {
		str[i] = '\0';
	}
}

Client get_client(int connfd) {
	for (int i =0; i < client_num; i++) {
		if (client_arr[i].connfd == connfd)
			return client_arr[i];
	}
}

Client get_client_same_room(int connfd) {
	for (int i = 0; i < client_num; i++) {
		if ((client_arr[i].room_id == get_client(connfd).room_id) && (client_arr[i].connfd != connfd)) 
			return client_arr[i];
	}
}

int num_char_in_string(char c, char *str) {
	int count = 0;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == c) count++;
	}

	return count;
}


void new_client(int connfd, char *buff) {
	client_arr[client_num].connfd = connfd;
	strcpy(client_arr[client_num++].name, buff);
}

int join_room(int connfd, int room_id) {

	if (room_arr[room_id-1].client_num == ROOM_SIZE)
		return 0;
	if (room_arr[room_id-1].status == ROOM_RUNNING)
		return -1;

	for(int i = 0; i < client_num; i++)
		if (client_arr[i].connfd == connfd)	{
			client_arr[i].room_id = room_id;
		}
		
	room_arr[room_id-1].client_num++;
	return 1;	
}



void send_msg_room(int connfd, int room_id, char *msg) {
	for (int i = 0; i < client_num; i++) {
		if (client_arr[i].connfd != connfd && client_arr[i].room_id == room_id) {
			send(client_arr[i].connfd, msg, strlen(msg), 0);
		}
	}
}

void send_all_client(int room_id, int connfd, char *msg) {
	for (int i = 0; i < client_num; i++) {
		if (client_arr[i].connfd != connfd && client_arr[i].room_id != room_id)
			send(client_arr[i].connfd, msg, strlen(msg), 0);
	}
}

char *get_params(char command[]) {
	int i = 0, j;
	while (command[i] != ' ') {
		i++;
	}
	i++;

	char * params = malloc(LENGTH_MSG);
	for (j = 0; i+j < strlen(command); j++) {
		*(params + j) = command[i+j];
	}
	*(params + j) = '\0';
	return params;
}

char get_char_msg(char command[]) {
	int i = 0, j;
	while (command[i] != ' ') i++;
	i++;
	char c;
	c = command[i];
	return c; 
}


void *echo(void *arg){
	int bytes_sent, bytes_received;
	char buff[BUFF_SIZE + 1];
	char msg[LENGTH_MSG];
	int connfd = *((int *) arg);
		
	pthread_detach(pthread_self());
	// Game start
	
	int room_id;	
	while (1) {
		memset(buff, 0, strlen(buff));
		while((bytes_received = recv(connfd, buff, BUFF_SIZE, 0)) > 0) {
			buff[bytes_received] = '\0';
			memset(msg, 0, strlen(msg));

			if (strstr(buff, "./new_client")) {
				new_client(connfd, get_params(buff));
				printf("You got a connection from %s - Port: %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port)); /* prints client's IP */
				printf("Client name: %s \n", buff);
				sprintf(msg, "new_client_success: ");

				for (int i = 0; i < ROOM_MAX; i++) {
					sprintf(msg + strlen(msg), "%d-%d#", room_arr[i].id, room_arr[i].client_num);	
				}
				/*gửi thông tin tất cả phòng cho thằng client để nó hiển thị theo định dạng 1-0#2-0#2-0#4-0#*/
				send(connfd, msg, strlen(msg), 0);	
			}

			if (strstr(buff, "./join_room")) {
				room_id =  atoi(get_params(buff));
				int check = join_room(connfd, room_id);
				printf("room: %d - check: %d\n", room_id, check);
				
				switch (check) {
					case -1:
						sprintf(msg, "join_room_error: %s", ROOM_STARTED_NOTIFY);
						send(connfd, msg, strlen(msg), 0);
						break;
					case 0:
						sprintf(msg, "join_room_error: %s", ROOM_FULL_NOTIFY);
						send(connfd, msg, strlen(msg), 0);
						break;
					case 1:
						sprintf(msg, "join_room_success: ");
						int room_player = 0;
						char temp[LENGTH_MSG];
						sprintf(temp, "refresh_friend_room: ");

						for (int i = 0; i < client_num; i++) {
							if (client_arr[i].room_id == room_id) {
								sprintf(msg+strlen(msg), "%s#", client_arr[i].name);
								sprintf(temp+strlen(temp), "%s#", client_arr[i].name);
								room_player++;
							}	
						}
						send(connfd, msg, strlen(msg), 0);
						send_msg_room(connfd, room_id, temp);
						// Start game in room

						

						if (room_player == ROOM_SIZE) {
							room_arr[room_id-1].status = ROOM_RUNNING;
						}
						char temp2[LENGTH_MSG];	
						sprintf(temp2, "refresh_list_room: ");
						for (int i = 0; i < ROOM_MAX; i++) {
							sprintf(temp2 + strlen(temp2), "%d-%d#", room_arr[i].id, room_arr[i].client_num);	
						}
						puts(temp2);
						send_all_client(room_id, connfd, temp2);
						break;
					default:
						break;
				}
			}

			if (strstr(buff, "./new_character")) {
				puts(buff);
				char c = get_char_msg(buff);
				int count = num_char_in_string(c, q.answer);

				for (int i = 0; i < strlen(q.answer); i++) {
					if (q.answer[i] == c) q.hide_answer[i] = c;
				}
				
				for (int i = 0; i < client_num; i++) {
					if (client_arr[i].connfd == connfd) {
						client_arr[i].turn++;
						client_arr[i].grade += (10*count);
						break;
					}
				}

				Client clnt = get_client(connfd);

				printf("Name: %s ", clnt.name);
				printf("Turn: %d ", clnt.turn);	
				printf("Grade: %d\n", clnt.grade);
				sprintf(msg, "new_character_block: %s", q.hide_answer);
				puts(msg);
				send(connfd, msg, strlen(msg), 0);
				
				Client client2 = get_client_same_room(connfd);

				if (client2.turn < MAX_TURN) {
					char temp[LENGTH_MSG];
					sprintf(temp, "new_character_unblock: %s", q.hide_answer);
					send_msg_room(connfd, room_id, temp);
				}
			}

			if (strstr(buff, "./get_question")) {
				q = load_question();
				char temp[LENGTH_MSG];
				char temp2[LENGTH_MSG];
				sprintf(temp, "new_question_1: %s#%s#", q.question, q.hide_answer);
				sprintf(temp2, "new_question_2: %s#%s#", q.question, q.hide_answer);

				send(connfd, temp2, strlen(temp2), 0);
				send_msg_room(connfd, room_id, temp);
			}


			if (strstr(buff, "./exit")) {
				
			}
		}
	}
	
}

int main(int argc, char *argv[]) { 
	int listenfd, connfd;
	pthread_t tid;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT); 
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   
	
	if(bind(listenfd,(struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listenfd, CLIENT_MAX) == -1){  
		perror("\nError: ");
		return 0;
	}
	printf("Start Server on: %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
	int client_size = sizeof(struct sockaddr_in);
	room_init();
	while(1){		
		if ((connfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t*)&client_size)) ==- 1)
			perror("\nError: ");		
		pthread_create(&tid, NULL, &echo, &connfd);	
	}
	
	close(listenfd);
	return 0;
}