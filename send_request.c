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
	room_id = *((int *)data);
	sprintf(join_room_cmd, "./join_room %d", room_id);
	send(client_sock, join_room_cmd, strlen(join_room_cmd),0);
}

void send_msg() {
    char message[LENGTH_MSG] = {};
    const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(entry_msg));
    append_message(send_buffer);
	sprintf(message, "./new_message %s", send_buffer);
    send(client_sock, message, strlen(message), 0);
	puts(message);
	gtk_entry_set_text(GTK_ENTRY(entry_msg), "");	
}

void send_answer(GtkWidget *widget, gpointer *data) {
	if (running == TRUE) {
		char answer_cmd[LENGTH_MSG];
		sprintf(answer_cmd, "./answer %d %d", q_cur, *((int *)data));
		puts(answer_cmd);
		send(client_sock, answer_cmd, strlen(answer_cmd),0);
	} else {
		show_error(ELIMINATED);
	}
}

void send_full_room(){
	char message[LENGTH_MSG];
	sprintf(message, "./send_full_room %d", room_id);
	send(client_sock,message,strlen(message),0);
}

void send_back() {
	char message[LENGTH_MSG];
	if (running == TRUE)
		sprintf(message, "./left_room_running %d", room_id);
	else sprintf(message, "./left_room_eliminated %d", room_id);
    send(client_sock, message, strlen(message), 0);
}