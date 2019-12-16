#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>

#include <gtk/gtk.h>
#include "question.c"
#include "queue.c"
#include "client_config.h"
#include "handle_string.c"
#include "dialog.c"
#include "gui.c"
#include "send_request.c"

void recv_msg() {
	char *receive_message = malloc(LENGTH_MSG);
	memset(receive_message, 0, strlen(receive_message)+1);
	int receive = recv(client_sock, receive_message, LENGTH_MSG, 0);
	
	if (receive > 0) {
		receive_message[receive] = '\0';
		puts(receive_message);
		enQueue(responses, receive_message);
	} else if (receive == 0) {
		// break;
	} else { 
		// -1 
	}
}

gboolean timer_exe(gpointer p, int test) {
    char msg[1024], *data;
	struct QNode * response = deQueue(responses);
    if (response != NULL) {
		strcpy(msg, response->key);
		if (strstr(msg, "new_client_success") || strstr(msg, "choose_room_again")) {
			data = get_data(msg);
			choose_room_screen(data);
	    }
	    if (strstr(msg, "join_room_success")) {	
			data = get_data(msg);
			wait_friend_screen(data);
		}
		if (strstr(msg, "join_room_error")) {
			data = get_data(msg);
			show_error(data);
		}
		if (strstr(msg, "refresh_list_room")) {
			if (label_room != NULL) {
				puts("change");
				data = get_data(msg);
				choose_room_clear();
				choose_room_screen(data);
			}
		}
		if (strstr(msg, "turn_now")){
			data = get_data(msg);
			my_turn = atoi(data);
			printf("my_turn %d\n",my_turn );
			play_game();
		}
		if (strstr(msg, "left_room")) {
			data = get_data(msg);
			if (strstr(msg, "left_room_running")) {
				puts("thoat");
				running_client--;
			}
			show_info(data);
			append_message(data);
		}
		if (strstr(msg, "refresh_friend_room")) {
			puts(msg);
			data = get_data(msg);
			refresh_friend_room(data);
		}
		if (strstr(msg, "new_message_success")) {
			data = get_data(msg);
			append_message(data);
			my_turn = 1;
			turn_on_text_box();
			// printf("my_turn lan 2 %d\n", my_turn);	
		}
    }

    return TRUE;
}


int main(int argc, char *argv[]) {
    responses = createQueue(); 

	if (!g_thread_supported ()){ g_thread_init(NULL); }
	// initialize GDK thread support
	gdk_threads_init();
	gdk_threads_enter();
	g_timeout_add(100, (GSourceFunc)timer_exe, NULL);
	gtk_init(&argc, &argv);

	struct sockaddr_in server_socket;
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(SERVER_PORT);
    server_socket.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    printf("server IP = %s ", inet_ntoa(server_socket.sin_addr));

    if (connect(client_sock, (struct sockaddr *)&server_socket, sizeof(server_socket)) < 0)
        printf("Error in connecting to server\n");
    else
        printf("connected to server\n");

    // Signal driven I/O mode and NONBlOCK mode so that recv will not block
    if (fcntl(client_sock, F_SETFL, O_NONBLOCK | O_ASYNC))
        printf("Error in setting socket to async, nonblock mode");

    signal(SIGIO, recv_msg); // assign SIGIO to the handler
	
    // set this process to be the process owner for SIGIO signal
    if (fcntl(client_sock, F_SETOWN, getpid()) < 0)
        printf("Error in setting own to socket");

    game_init(); 
	enter_name_screen();
	gtk_main();
    gdk_threads_leave();
	close(client_sock);
    return 0;
}

