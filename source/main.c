#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


gint window_width;
gint window_height;

char *directory_path;


gboolean
fileEntryOnRightClick
(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data)
{
	return TRUE;
}


void
buildFileEntry
(GtkWidget *file_list)
{
	return;
}


void
buildBrowsingView
(GtkWidget *window)
{
	

	GtkWidget *topGrid = gtk_grid_new();

	gtk_widget_add_css_class( topGrid, "top-grid" );

	gtk_grid_insert_row( topGrid, 0 );
	gtk_grid_insert_row( topGrid, 1 );

	gtk_grid_insert_column( topGrid, 0 );
	gtk_grid_insert_column( topGrid, 1 );

	GtkWidget *actionBar = gtk_action_bar_new();

	GtkWidget *newFileButton = gtk_button_new_with_label( "New file" );

	gtk_widget_set_halign( actionBar, 1 );

	gtk_action_bar_pack_end( actionBar, newFileButton );

	gtk_grid_attach( topGrid, actionBar, 0, 0, 1, 1 );

	GtkWidget *bookmarks = gtk_label_new( "Bookmarks" );

	gtk_widget_add_css_class( topGrid, "bookmarks" );

	gtk_grid_attach( topGrid, bookmarks, 0, 1, 1, 1 );

	GtkWidget *file_list = gtk_list_box_new();

	gtk_grid_attach( topGrid, file_list, 1, 1, 1, 1 );

	GtkWidget *file_entry;

	char *filename;

	DIR *directory = opendir(".");

	if (directory == NULL)
	{
		printf("Error on function [buildBrowsingView]. Could not open directory.\n");
		return;
	}

	struct dirent *entry;

	while ( (entry = readdir(directory)) != NULL )
	{
		file_entry = gtk_button_new_with_label( entry->d_name );

		gtk_widget_set_halign( file_entry, 1 );

		gtk_widget_add_css_class( file_entry, "entry" );

		if ( entry->d_type == DT_DIR )
		{
			gtk_widget_add_css_class( file_entry, "directory-entry" );
		}

		else
		{
			gtk_widget_add_css_class( file_entry, "file-entry" );
		}

		GtkGesture *rightClickGesture = gtk_gesture_click_new ();

		gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (rightClickGesture), 3);

		gtk_widget_add_controller (file_entry, GTK_EVENT_CONTROLLER (rightClickGesture));

		g_signal_connect(rightClickGesture, "pressed", G_CALLBACK(fileEntryOnRightClick), NULL);

		gtk_list_box_append(file_list, file_entry);
	}

	gtk_window_set_child(GTK_WINDOW(window), topGrid);
}


static void
onActive
(GtkApplication *app)
{
	GtkWidget *window = gtk_application_window_new(app);

	gtk_window_set_title(GTK_WINDOW(window), "Untitled Document");

	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

	GtkCssProvider *cssProvider = gtk_css_provider_new();

	GdkDisplay *display = gdk_display_get_default();

	gtk_css_provider_load_from_string(cssProvider, "*{font-size:15pt;} .directory-entry{background: rgb(28, 118, 228);} .directory-entry:hover{background: rgb(11 86 178);}");

	gtk_style_context_add_provider_for_display(display, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

	buildBrowsingView(window);

	gtk_window_present(GTK_WINDOW(window));
}


int
main
(int argc, char **argv)
{
	GtkApplication *app = gtk_application_new("com.gtk-file-browser", G_APPLICATION_HANDLES_OPEN);

	g_signal_connect(app, "activate", G_CALLBACK(onActive), NULL);

	return g_application_run(G_APPLICATION(app), argc, argv);
}

