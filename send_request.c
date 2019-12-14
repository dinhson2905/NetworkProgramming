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
	send(client_sock, join_room_cmd, strlen(join_room_cmd), 0);
}
