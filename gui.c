/* Tất cả đều nằm trong 1 window, khi mà thay đổi giao diện nghĩa là clear cái cũ đi, vẽ cái mới trên window đó*/
/*Hàm clear những giao diện cũ, để tạo giao diện mới*/
void game_init();
void game_quit(GtkWidget *widget, gpointer *data);
void enter_name_screen();
void choose_zoom_screen ();
void wait_friend_screen();
void append_message(char *msg);
void enter_name_clear();
void choose_zoom_clear();
void in_game_clear();
void win_game_screen();
void result_screen();
void lose_game_screen();
void enter_name_clear();
void end_game_clear();

/* ----- Send request function ----*/
void send_name (GtkWidget *widget, gpointer *data);
void send_room (GtkWidget *widget, gpointer *data);
void send_msg();
void send_answer (GtkWidget *widget, gpointer *data);
void send_back();
void send_full_room();
void turn_off_text_box();
/* --------------------------------*/


/*Hàm lấy trên mạng, lấy một file ảnh lưu vào pixbuf*/
GdkPixbuf *create_pixbuf(const gchar * filename) {
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

/*Hàm tạo background*/
void set_background(const gchar * filename) {
	image = create_pixbuf (filename);
	gdk_pixbuf_render_pixmap_and_mask (image, &background, NULL, 0);
    style = gtk_style_new ();
    style->bg_pixmap [0] = background;
	gtk_widget_set_style (GTK_WIDGET(window), GTK_STYLE (style));
}

/*----Hàm clear----*/
void enter_name_clear() {
	if (label_name != NULL) {
		gtk_widget_hide(label_name);
		gtk_widget_hide(entry_name);
	}
}

void choose_room_clear() {
	in_choose_room = FALSE;
	for (int i = 0; i < ROOM_NUM; i++)
		gtk_widget_hide(button_room[i]);
	gtk_widget_hide(label_room);
	label_room = NULL;
}



void play_game() {
	if (my_turn) {
		handler_id = g_signal_connect(entry_msg, "activate", G_CALLBACK(turn_off_text_box), NULL);
	}
	else g_signal_handler_disconnect(entry_msg,handler_id);
}

void turn_on_text_box(){
	handler_id = g_signal_connect(entry_msg,"activate",G_CALLBACK(turn_off_text_box),NULL);
}

void turn_off_text_box(){
	send_msg();
	my_turn = FALSE;
	g_signal_handler_disconnect(entry_msg,handler_id);
}
// void change_to_wait_and_send_msg()
// {
// 	send_msg();
// 	wait_play_game();
// }

void in_game_clear(){
	if (btn_back != NULL) {
		// Chat area
		for(int i = 0; i < ROOM_SIZE; i++) {
			if (label_client[i] != NULL) {
				gtk_widget_hide(label_client[i]);
				label_client[i] = NULL;
			}
		}
		gtk_widget_hide(btn_back);
		gtk_widget_hide(entry_msg);
		gtk_widget_hide(msg_box);
		gtk_widget_hide(question);
		gtk_widget_hide(answer);
		msg_box = NULL;
	}
	if (label_question != NULL) {
		// Answer question area
		gtk_widget_hide(label_question);
		label_question = NULL;
		for (int i = 0; i < 4; i++) {
			gtk_widget_hide(button_option[i]);
			button_option[i] = NULL;
		}
	}
}
void end_game_clear(){
	if (label_name != NULL) {
		gtk_widget_hide(label_name);
		gtk_widget_hide(entry_name);
	}
}


void game_init() {   

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	GdkPixbuf *icon = create_pixbuf("images/logo.jpeg");  
  	gtk_window_set_icon(GTK_WINDOW(window), icon); 
	g_object_unref(icon);    
	set_background("images/CNKDbg.jpg");

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_default_size(GTK_WINDOW(window), SCREEN_WIDTH, SCREEN_HEIGHT);
	gtk_window_set_title(GTK_WINDOW(window), "Chiếc Nón Kỳ Diệu");
}

void enter_name_screen () {
	table = gtk_table_new(7, 5, TRUE);
	
	label_name = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_name), "<span foreground='#FFFFFF'><b>Enter your username:</b></span>");
	gtk_table_attach_defaults(GTK_TABLE(table), label_name, 0, 2, 2, 4);
	
	entry_name =  gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), entry_name, 2, 4, 2, 4);

	g_signal_connect(entry_name, "activate", G_CALLBACK(send_name), NULL);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_widget_show_all(window);
}



void choose_room_screen(char *data) {
	enter_name_clear();
	in_game_clear();
	end_game_clear();
	set_background("images/choose_room.jpg");

	in_choose_room = TRUE;
	in_waiting_friend = FALSE;
	label_room = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_room), "<span foreground = '#FFFFFF'><b>Choose a room</b></span>");
	gtk_table_attach_defaults(GTK_TABLE(table), label_room, 1, 4, 0, 1);
	gtk_widget_show(label_room);
	char row[256];
	convert_room_detail(data);
	for(int i = 0; i < ROOM_NUM; i++) {
		memset(row, 0, strlen(row));
		sprintf(row, "Room %d - %d client\n", room_arr[i].id, room_arr[i].client_num);
		button_room[i] = gtk_button_new_with_label(row);
		
		gtk_table_attach_defaults(GTK_TABLE(table), button_room[i], 1, 4, i+1, i+2);
		int *room_id= malloc(sizeof(int));
		*room_id = i+1;
		g_signal_connect(G_OBJECT(button_room[i]), "clicked", G_CALLBACK(send_room), (gpointer) room_id);
		gtk_widget_show(button_room[i]);
	}

	gtk_widget_show(window);
}



void wait_friend_screen(char *data) {
	// printf("wait_friend_screen\n");
	choose_room_clear();
	step = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(step), "Nhiệm vụ : vượt qua 5 câu hỏi");
	gtk_table_attach_defaults(GTK_TABLE(table), step, 2, 4, 0, 1);
	gtk_widget_show(step);
	// printf("naruto\n");
	in_waiting_friend = TRUE;
	refresh_friend_room(data);
	// printf("naruto\n");
	if (running_client <= ROOM_SIZE - 1) 
		show_info("Please wait another player!");
	// printf("naruto\n");
	btn_back = gtk_button_new_with_label("Back");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_back, 0, 1, 0, 1);
	g_signal_connect(btn_back, "clicked", G_CALLBACK(show_question), NULL);
	gtk_widget_show(btn_back);
	// printf("naruto\n");
	msg_box = gtk_text_view_new();
	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scroll_window), msg_box);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll_window),
									GTK_POLICY_AUTOMATIC,
									GTK_POLICY_AUTOMATIC);

	gtk_table_attach_defaults(GTK_TABLE(table), scroll_window, 2, 4, 2, 5);
	gtk_widget_show(msg_box);
	gtk_widget_show(scroll_window);

	entry_msg = gtk_entry_new();
	// g_signal_connect(entry_msg, "activate", G_CALLBACK(send_msg), NULL);
	gtk_table_attach_defaults(GTK_TABLE(table), entry_msg, 2, 4, 5, 6);
	gtk_widget_show(entry_msg);
	gtk_widget_show(window);
	// printf("wait_friend_screen\n");
}
void refresh_friend_room(char *data) {
	for(int i = 0; i < ROOM_SIZE; i++) {
		if (label_client[i] != NULL) {
			gtk_widget_hide(label_client[i]);
			label_client[i] = NULL;
		}
	}
	// printf("refresh_friend_room\n");
	convert_client_detail(data);
	printf("refresh_friend_room\n");
	for(int i = 0; i < running_client; i++) {
		puts(client_arr[i].name);
		if (label_client[i] == NULL) {
			label_client[i] = gtk_label_new(NULL);
			// Make client_name to bold if current client
			char temp[LENGTH_MSG];
			if (strcmp(client_name, client_arr[i].name) == 0) {
				sprintf(temp,"<b>%s</b>", client_arr[i].name);
				gtk_label_set_markup(GTK_LABEL(label_client[i]), temp);
			}
			else gtk_label_set_text(GTK_LABEL(label_client[i]), client_arr[i].name);
			gtk_table_attach_defaults(GTK_TABLE(table), label_client[i], 2+i, 3+i, 1, 2);
			gtk_widget_show(label_client[i]);

		} 
	}
	if(running_client == ROOM_SIZE) {
		show_info("Phòng chơi đã đầy. Bắt đầu ngay!");
		send_full_room();
		// play_game(); 
	}
}

void append_message(char *msg) {
	GtkTextBuffer * buffer;
	GtkTextMark * mark;
	GtkTextIter iter;
	if (msg_box == NULL)
		return;
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(msg_box));

	mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	if (gtk_text_buffer_get_char_count(buffer))
		gtk_text_buffer_insert(buffer, &iter, "\n", 1);

	gtk_text_buffer_insert(buffer, &iter, msg, -1);
	gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW(msg_box), mark);

}

void win_game_screen() {
	in_game_clear();
	gtk_widget_show(btn_back);
	end_game_result = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(end_game_result), "<b>Các bạn đã chiến thắng</b>");
	gtk_table_attach_defaults(GTK_TABLE(table), end_game_result, 1, 3, 1, 2);
	gtk_widget_show(end_game_result);

	result_screen();	
}

void lose_game_screen() {
	in_game_clear();
	gtk_widget_show(btn_back);
	end_game_result = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(end_game_result), "<b>Các bạn đã thua cuộc</b>");
	gtk_table_attach_defaults(GTK_TABLE(table), end_game_result, 1, 3, 1, 2);
	gtk_widget_show(end_game_result);

	result_screen();
}

void result_screen() {
	result_box = gtk_text_view_new();
	gtk_table_attach_defaults(GTK_TABLE(table), result_box, 1, 3, 2, 5);
	GtkTextBuffer *buffer2 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (result_box));
	
	char temp[LENGTH_MSG], another_client[40];
	sprintf(temp, "%s trả lời đúng câu: ", client_name);
	for(int i = 0; i < q_cur; i++) {
		if (strcmp(answer_client[i], client_name) == 0)
			sprintf(temp + strlen(temp), "%d, ", i+1);
	}
	int j = 0;
	for(int i = 0; i < q_cur; i++) {
		if (strcmp(answer_client[i], client_name) != 0) {
			if (j == 0) {
				sprintf(temp + strlen(temp), "--- %s trả lời đúng câu: ", answer_client[i]);
				j++;
			}
			sprintf(temp + strlen(temp), "%d, ", i+1);
		}
	}

	gtk_text_buffer_set_text (buffer2, temp, -1);
	gtk_widget_show(result_box);
}
