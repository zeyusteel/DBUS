#include "../gen/gen.h"
#include "../include/comdef.h"

/*
  gboolean (*handle_set_value) (
    namespaceGDBusDemo *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_in_put);
*/

static gboolean handle_set_value_callback(namespaceFoo *object,
                    GDBusMethodInvocation *invocation,
                    const gchar *arg_in,
                    gpointer user_data)
{
    g_print("run callback arg_in:%s\n", arg_in);
    namespace_foo_set_value(object, arg_in);
    namespace_foo_complete_set_value(object, invocation, TRUE);
    return TRUE;
}

static gboolean sendSignal(gconstpointer p)          
{                                                         
    const gchar *pVal = namespace_foo_get_value((namespaceFoo*)p);
    namespace_foo_emit_send_value((namespaceFoo*)p, pVal);
    g_print("Server sendSignal %s is called.\n", pVal);            
    return TRUE;                                          
}

static void on_bus_acquired_comm_way(GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
    g_print("Acquired a message bus connection by common way\n");
    GError *pError = NULL;

    namespaceFoo *pSkeleton = namespace_foo_skeleton_new();
    g_signal_connect(pSkeleton, "handle-set-value", G_CALLBACK(handle_set_value_callback), 
                                                    NULL);// user_data

    g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(pSkeleton),
                                                connection,
                                                OBJ_PATH,
                                                &pError);

    if (pError == 0) {
        g_print("skeleton export successfully. \n");
        namespace_foo_set_value(pSkeleton, "only server init");
        g_timeout_add(2000, (GSourceFunc)sendSignal, pSkeleton);
    } else {
        g_print("Error: Failed to export object. Reason: %s.\n", pError->message);
        g_error_free(pError);
        g_object_unref(pSkeleton);
    }

}

static void on_bus_acquired(GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
    on_bus_acquired_comm_way(connection, name, user_data);
}

static void on_name_acquired(GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    g_print("Acquired the name %s\n", name);
}

static void on_name_lost(GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    g_print("Lost the name %s\n", name);
}

 

gint main(gint argc, gchar *argv[])
{
    GMainLoop *loop; 
    guint id;

    loop = g_main_loop_new(NULL, FALSE);// FALSE: do not running 
    id = g_bus_own_name(G_BUS_TYPE_SESSION,
                        G_BUS_NAME,
                        G_BUS_NAME_OWNER_FLAGS_NONE,
                        on_bus_acquired,
                        on_name_acquired,
                        on_name_lost,
                        loop,
                        NULL);


    g_main_loop_run(loop);

    g_bus_unown_name(id);   //Stops owning a name
    g_main_loop_unref(loop);//free the loop and free all associated memory

    return 0;
}
