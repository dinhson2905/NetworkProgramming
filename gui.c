/* Tất cả đều nằm trong 1 window, khi mà thay đổi giao diện nghĩa là clear cái cũ đi, vẽ cái mới trên window đó*/
/*Hàm clear những giao diện cũ, để tạo giao diện mới*/
void enter_name_clear();
void choose_room_clear();
void in_game_clear();
void end_game_clear();
/*-------Hàm giao diện--------*/
void game_init(); /*khởi tạo game*/
void enter_name_screen(); /* Nhập tên */
void choose_room_screen(); /* Giao diện chọn phòng */
void wait_friend_screen(); /* Giao diện lúc người chơi chưa play game */
void win_game_screen();
void result_screen();
void lose_game_screen();
void play_game(); /* Hàm này được gọi khi đủ người chơi trong phòng, gọi đến hàm new_question */
void new_question(); /* Hiển thị câu hỏi trên giao diện */
void append_message(char *data);
void redisplay_answer_grade();/*Hiện thị lại đáp án và câu hỏi*/
void input_answer();/*Hiển thị thanh nhập answer*/
/* ----- Send request function ----*/
void send_name(GtkWidget *widget, gpointer *data);
void send_room(GtkWidget *widget, gpointer *data);
void send_character();
void send_answer();
void send_back();
void send_question();
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
		gtk_widget_hide(label_question);
		gtk_widget_hide(label_answer);
		gtk_widget_hide(label_enter_char);
		
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
	choose_room_clear();

	set_background("images/in_game.png");

	label_enter_char = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_enter_char), "<span><b>Enter Letter: </b></span>");
	gtk_table_attach_defaults(GTK_TABLE(table), label_enter_char, 0, 1, 6, 7);
	gtk_widget_show(label_enter_char);
	in_waiting_friend = TRUE;
	refresh_friend_room(data);
	if (running_client <= ROOM_SIZE - 1) show_info("Please wait another player!");

	btn_back = gtk_button_new_with_label("Back");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_back, 4, 5, 6, 7);
	g_signal_connect(btn_back, "clicked", G_CALLBACK(send_back), NULL);
	gtk_widget_show(btn_back);

	msg_box = gtk_text_view_new();
	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scroll_window), msg_box);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll_window),
									GTK_POLICY_AUTOMATIC,
									GTK_POLICY_AUTOMATIC);

	gtk_table_attach_defaults(GTK_TABLE(table), scroll_window, 0, 3, 2, 6);
	gtk_widget_show(msg_box);
	gtk_widget_show(scroll_window);

	entry_msg = gtk_entry_new();
	handler_id = g_signal_connect(entry_msg, "activate", G_CALLBACK(send_character), NULL);
	g_signal_handler_disconnect(entry_msg, handler_id);
	gtk_table_attach_defaults(GTK_TABLE(table), entry_msg, 1, 3, 6, 7);
	gtk_widget_show(entry_msg);
	gtk_widget_show(window);
}

void refresh_friend_room(char *data) {
	for(int i = 0; i < ROOM_SIZE; i++) {
		if (label_client[i] != NULL) {
			gtk_widget_hide(label_client[i]);
			label_client[i] = NULL;
		}
	}
	convert_client_detail(data);
	
	for(int i = 0; i < running_client; i++) {
		//puts(client_arr[i].name);
		if (label_client[i] == NULL) {
			label_client[i] = gtk_label_new(NULL);
			// Make client_name to bold if current client
			char temp[LENGTH_MSG];
			if (strcmp(client_name, client_arr[i].name) == 0) {
				sprintf(temp,"%s", client_arr[i].name);
				gtk_label_set_markup(GTK_LABEL(label_client[i]), temp);
			} else gtk_label_set_text(GTK_LABEL(label_client[i]), client_arr[i].name);

			gtk_table_attach_defaults(GTK_TABLE(table), label_client[i], 3+i, 4+i, 2, 3);
			gtk_widget_show(label_client[i]);
		} 
	}

	if(running_client == ROOM_SIZE) {
		show_info(START_GAME);
		play_game(); 
	}
}

void play_game() {
	running = TRUE;
	send_question();
	q_cur = 0;
}

void display_question(char *data) {
	
	convert_question(data);	
	gtk_widget_hide(label_question);
	gtk_widget_hide(label_answer);

	label_question = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(label_question), question.question);
	gtk_table_attach_defaults(GTK_TABLE(table), label_question, 0, 5, 0, 1);
	gtk_widget_show(label_question);

	label_answer = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(label_answer), question.hide_answer);
	gtk_table_attach_defaults(GTK_TABLE(table), label_answer, 0, 5, 1, 2);
	gtk_widget_show(label_answer);
	
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

void redisplay_answer_grade() {
	
	gtk_widget_hide(label_answer);

	label_answer = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(label_answer), question.hide_answer);
	gtk_table_attach_defaults(GTK_TABLE(table), label_answer, 0, 5, 1, 2);
	gtk_widget_show(label_answer);
	

	for(int i = 0; i < running_client; i++) {
		printf("%s %d %d\n", client_arr[i].name, client_arr[i].grade, client_arr[i].turn);
		if (label_grade[i] != NULL) {
			gtk_widget_hide(label_grade[i]);
		}

		label_grade[i] = gtk_label_new(NULL);
		char temp[10];
		sprintf(temp, "%d", client_arr[i].grade);
		gtk_label_set_text(GTK_LABEL(label_grade[i]), temp);
		gtk_table_attach_defaults(GTK_TABLE(table), label_grade[i], 3+i, 4+i, 3, 4);
		gtk_widget_show(label_grade[i]);
	}
	
}

void input_answer() {
	
	if (entry_msg != NULL) {
		gtk_widget_hide(entry_msg);
		gtk_widget_hide(label_enter_char);
	}

	label_enter_answer = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_enter_answer), "<span><b>Enter Answer: </b></span>");
	gtk_table_attach_defaults(GTK_TABLE(table), label_enter_answer, 0, 1, 6, 7);
	gtk_widget_show(label_enter_answer);

	entry_answer = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), entry_answer, 1, 3, 6, 7);
	handler_id = g_signal_connect(entry_answer, "activate", G_CALLBACK(send_answer), NULL);

	gtk_widget_show(entry_answer);
}