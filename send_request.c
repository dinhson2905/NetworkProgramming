void send_name(GtkWidget *widget, gpointer *data) {
	const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(widget));
	strcpy(client_name, send_buffer);
	if (!check_valid_username(send_buffer)) {
		show_error("Tên đăng nhập không hợp lệ!");
		return;
	} 
	char new_client_cmd[LENGTH_MSG];
	sprintf(new_client_cmd, "./new_client %s", send_buffer);
	int n = send(client_sock, new_client_cmd, strlen(new_client_cmd),0);
}

void send_room(GtkWidget *widget, gpointer *data) {
	char join_room_cmd[LENGTH_MSG];
	room_id = *((int*) data);
	sprintf(join_room_cmd, "./join_room %d", room_id);
	send(client_sock, join_room_cmd, strlen(join_room_cmd), 0);
}

void send_character() {
    char message[LENGTH_MSG] = {};
    const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(entry_msg));
	if (strlen(send_buffer) != 1) {
		show_info("Chỉ được nhập 1 ký tự");
		return;
	}

	if (check_char_in_string(send_buffer[0], question.hide_answer)) {
		show_info("Ký tự này đã được chọn");
		return;
	}
	
	sprintf(message, "./new_character %s", send_buffer);
    send(client_sock, message, strlen(message), 0);
	puts(message);
	gtk_entry_set_text(GTK_ENTRY(entry_msg), "");	
}

/*Gửi yêu cầu cho server trả về question*/
void send_question() {
	char get_question_cmd[LENGTH_MSG];
	sprintf(get_question_cmd, "./get_question");
	send(client_sock, get_question_cmd, strlen(get_question_cmd), 0);
}

/*Gửi tín hiệu đoán đáp án để server gửi lại cho 2 thằng cùng phòng*/
void send_guess() {
	char get_guess_cmd[LENGTH_MSG];
	sprintf(get_guess_cmd, "./get_guess");
	send(client_sock, get_guess_cmd, strlen(get_guess_cmd), 0);
}

void send_answer() {
	
	char message[LENGTH_MSG] = {};

	const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(entry_answer));

	sprintf(message, "./answer %s", send_buffer);
	
	send(client_sock, message, strlen(message), 0);
	puts(message);
	gtk_entry_set_text(GTK_ENTRY(entry_answer), "");
}

void send_back() {
	char message[LENGTH_MSG];
	if (running == TRUE)
		sprintf(message, "./left_room_running %d", room_id);
	else sprintf(message, "./left_room_eliminated %d", room_id);
	memset(client_arr, 0, sizeof(client_arr));

    send(client_sock, message, strlen(message), 0);
}

/*Gửi tín hiệu để server gửi về thông tin kết thúc*/
void send_result() {
	char message[LENGTH_MSG];
	sprintf(message, "./end_game");
	send(client_sock, message, strlen(message), 0);
}
