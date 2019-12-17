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
		/*Hiển thị cho tất cả thằng client cập nhật về  phòng khi có 1 thằng client khác vào phòng thành công*/
		if (strstr(msg, "refresh_list_room")) {
			if (label_room != NULL) {
				puts("change");
				data = get_data(msg);
				choose_room_clear();
				choose_room_screen(data);
			}
		}
		/*Hiển thị cho thằng cùng phòng để update lại phòng khi có đủ thằng vào phòng*/
		if (strstr(msg, "refresh_friend_room")) {
			//puts(msg);
			data = get_data(msg);
			refresh_friend_room(data);
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

		if (strstr(msg, "new_character_block")) {
			data = get_data(msg);
			convert_question_and_grade(data);
			
			g_signal_handler_disconnect(entry_msg, handler_id);
			redisplay_answer_grade();

		}

		/*gửi đến thằng client cùng phòng nếu lượt chơi của nó < max_turn*/
		if (strstr(msg, "new_character_unblock")) {
			data = get_data(msg);
			puts(data);
			convert_question_and_grade(data);
			handler_id = g_signal_connect(entry_msg, "activate", G_CALLBACK(send_character), NULL);
			redisplay_answer_grade();

		}

		if (strstr(msg, "new_question_1")) {
			data = get_data(msg);
			display_question(data);
			handler_id = g_signal_connect(entry_msg, "activate", G_CALLBACK(send_character), NULL);

		}

		if (strstr(msg, "new_question_2")) {
			data = get_data(msg);
			display_question(data);
			
			g_signal_handler_disconnect(entry_msg, handler_id);
		}

		if (strstr(msg, "signal_guess")) {
			
			data = get_data(msg);
			convert_question_and_grade(data);
			
			g_signal_handler_disconnect(entry_msg, handler_id);
			
			redisplay_answer_grade();
			send_guess();
		}

		if (strstr(msg, "guess_the_answer")) {		
			input_answer();
		}

		if (strstr(msg, "finish")) {
			data = get_data(msg);
			convert_question_and_grade(data);
	
			g_signal_handler_disconnect(entry_answer, handler_id);
			redisplay_answer_grade();
		}

		
		/*...refresh_answer_room......*/
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

	my_client.connfd = client_sock;
	
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

