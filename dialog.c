// dùng gdk_threads cho dialog thử xem 
void show_error(char error[]) {
    gdk_threads_init();
	gdk_threads_enter();
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                error);
    gtk_window_set_title(GTK_WINDOW(dialog), "Information");
    // int result = gtk_dialog_run(GTK_DIALOG(dialog));
    // int result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gdk_threads_leave();
}

void show_info(char info[]) {
    gdk_threads_init();
	gdk_threads_enter();
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK,
                info);
    gtk_window_set_title(GTK_WINDOW(dialog), "Information");
    gtk_dialog_run(GTK_DIALOG(dialog));
    // send_full_room();
    gtk_widget_destroy(dialog);
    gdk_threads_leave();
}

void show_question() {    
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_YES_NO,
                LEFT_ROOM_MODAL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Question");
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == MODAL_YES) {
        send_back();
    }

    gtk_widget_destroy(dialog);
    
}

