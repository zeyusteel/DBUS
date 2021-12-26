#include "../gen/gen.h"
#include "../include/comdef.h"

namespaceFoo *pProxy = NULL;

static gboolean send_value_callback(namespaceFoo *object,
                const gchar* arg_value,
                gpointer user_data)
{
    g_print("send sig callback %s\n", arg_value);
    const gchar *p = namespace_foo_get_value(pProxy);
    g_print("get val:%s\n", p);
    return TRUE;
}

static void use_proxy()
{
    GDBusConnection *pConnection = NULL;
    
    GError *pConnError = NULL;
    GError *pProxyError = NULL;
    GError *pError = NULL;

    pConnection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &pConnError);
    if (pConnError == NULL) {
        pProxy = namespace_foo_proxy_new_sync(pConnection,
                        G_DBUS_PROXY_FLAGS_NONE,
                        G_BUS_NAME,
                        OBJ_PATH,
                        NULL,
                        &pProxyError);

        if (pProxy == NULL) {
            g_print("initDBusCommunication: Create proxy failed. Reason: %s.\n", pProxyError->message);
            goto out;
        } else {
            g_print("initDBusCommunication: Create proxy successfully. \n");
        }
    } else {
        g_print("initDBusCommunication: Failed to connect to dbus. Reason: %s.\n", pConnError->message);
        goto out;
    }

	g_signal_connect(pProxy, "send-value", G_CALLBACK(send_value_callback), NULL);

    gboolean retBool = FALSE;
    namespace_foo_call_set_value_sync(pProxy, "hello world", &retBool, NULL, &pError);
    g_print(" return bool: %d\n", (gint)retBool);

    if (pError) {
        g_print("%s\n", pError->message);
        if (pProxy) { g_object_unref(pProxy); }
    }


out:
    if (pProxyError) { g_error_free(pProxyError); }
    if (pConnError) { g_error_free(pConnError); }
    if (pError) { g_error_free(pError); }
}

gint main(gint argc, gchar *argv[])
{
    GMainLoop *loop;
    loop = g_main_loop_new(NULL, FALSE);
    use_proxy();
    g_main_loop_run(loop);
    return 0;
}