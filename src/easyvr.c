#include <stdio.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <glib.h>
#include <libconfig.h>

#define DBUS_CHANNEL "org.conteplator.walle"

static DBusHandlerResult dbus_filter (DBusConnection *connection, DBusMessage *message, void *user_data)
{
    if (dbus_message_is_signal (message, DBUS_CHANNEL, "Customize")) {
        printf ("Message customize received\n");
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    if (dbus_message_is_signal (message, DBUS_CHANNEL, "Quit" )) {
        printf ("Message quit received\n");
        GMainLoop *loop = (GMainLoop*) user_data;
        g_main_loop_quit (loop);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}


int main()
{
    DBusConnection *connection;
    DBusError error;
    GMainLoop *loop;

    loop = g_main_loop_new (NULL, FALSE);
    dbus_error_init (&error);

    connection = dbus_bus_get (DBUS_BUS_SESSION, &error);

    if (dbus_error_is_set (&error)) {
        printf ("Error connection to the daemon bus: %s", error.message);
        dbus_error_free (&error);
        return 1;
    }

    dbus_bus_add_match (connection, "type='signal', interface='" DBUS_CHANNEL "'", NULL);
    dbus_connection_add_filter (connection, dbus_filter, loop, NULL);

    dbus_connection_setup_with_g_main (connection, NULL);

    g_main_loop_run (loop);

    return 0;
}


#include <sys/un.h>
#include <sys/socket.h>

#define SERVER_SOCK_FILE "/tmp/easyvr.socket"

void socket_test()
{
    int sdesc;
    struct sockaddr_un addr;
    int ret;
    char buff[8192];
    struct sockaddr_un from;
    int ok = 1;
    int len;
    socklen_t fromlen = sizeof (from);

    if ((sdesc = socket (PF_UNIX, SOCK_DGRAM, 0)) < 0) {
        perror ("socket");
        ok = 0;
    }

    if (ok) {
        memset (&addr, 0, sizeof (addr));
        addr.sun_family = AF_UNIX;
        strcpy (addr.sun_path, SERVER_SOCK_FILE);
        unlink (SERVER_SOCK_FILE);
        if (bind (sdesc, (struct sockaddr *)&addr, sizeof (addr)) < 0) {
            perror ("bind");
            ok = 0;
        }
    }

    while ((len = recvfrom (sdesc, buff, 8192, 0, (struct sockaddr *)&from, &fromlen)) > 0) {
        printf ("recvfrom: %s\n", buff);
        strcpy (buff, "transmit good!");
        ret = sendto (sdesc, buff, strlen (buff) + 1, 0, (struct sockaddr *)&from, fromlen);
        if (ret < 0) {
            perror ("sendto");
            break;
        }
    }


    if (sdesc >= 0) {
        close (sdesc);
    }

    printf ("Hello %d", 10);
}
