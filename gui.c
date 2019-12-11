#include <gtk/gtk.h>
#include "client_config.h"

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
/* --------------------------------*/


static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
 
  /*Create a window with a title and a default size*/
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "GNOME Button");
  gtk_window_set_default_size (GTK_WINDOW (window), 250, 50);

  /*Create a button with a label, and add it to the window*/
  button = gtk_button_new_with_label ("Click Me");
  gtk_container_add (GTK_CONTAINER (window), button);
 
  /*Connecting the clicked signal to the callback*/
  g_signal_connect (GTK_BUTTON (button), 
                    "clicked", 
                    G_CALLBACK (show_dialog), 
                    GTK_WINDOW (window));
 
  gtk_widget_show_all (window);
}

int
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
 
  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
 
  return status;
}