/*Gửi name cho server xử lý, server trả lại client thông tin*/
void send_name(GtkWidget *widget, gpointer *data) {
	// Get username
	const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(widget));
	strcpy(client_name, send_buffer);
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

    append_message(send_buffer);
	sprintf(message, "./new_character %s", send_buffer);
    send(client_sock, message, strlen(message), 0);
	puts(message);
	gtk_entry_set_text(GTK_ENTRY(entry_msg), "");	
}

void send_question() {
	char get_question_cmd[LENGTH_MSG];
	sprintf(get_question_cmd, "./get_question");
	send(client_sock, get_question_cmd, strlen(get_question_cmd), 0);
}


/*./answer 1#char*/