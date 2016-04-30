#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <JavaScriptCore/JavaScript.h>

 
static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

int main(int argc, char* argv[])
{
    // Initialize GTK+
    gtk_init(&argc, &argv);
	
    WebKitWebContext *web_ctx=webkit_web_context_get_default();
    gchar *module_path = g_build_filename(g_get_current_dir(),"modules/",NULL); 
    webkit_web_context_set_web_extensions_directory(web_ctx,module_path);

    // Create an 800x600 window that will contain the browser instance
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
	
    WebKitSettings *config=webkit_settings_new();
    webkit_settings_set_enable_webgl(config,true);
    webkit_settings_set_enable_developer_extras(config,true);
    webkit_settings_set_enable_write_console_messages_to_stdout(config,true);

    // Create a browser instance
    WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(config));

   // g_signal_connect(webView,"window-object-cleared", G_CALLBACK(window_object_cleared_cb), webView);

    // Put the browser area into the main window
    gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

    // Set up callbacks so that if either the main window or the browser instance is
    // closed, the program will exit
    g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
    g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

    // Load a web page into the browser instance
    webkit_web_view_load_uri(webView, "http://www.woodstitch.com/extend_js_example.html");

    // Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(webView));

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(main_window);

    // Run the main GTK+ event loop
    gtk_main();

    return 0;
}


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{

    g_message("Destroying window.");
    gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
    g_message("Closing Web View");
    gtk_widget_destroy(window);
    return TRUE;
}
