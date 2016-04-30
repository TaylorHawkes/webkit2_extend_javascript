#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>

/* Class initialize */
static void class_init_cb(JSContextRef ctx, JSObjectRef object) {
    g_message("Custom class initialize.");
}

/* Class finalize */
static void class_finalize_cb(JSObjectRef object) {
    g_message("Custom class finalize.");
}

/*API FUNCTION DEFIINITONS*/
static JSValueRef notification_notify_cb(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
{
	g_message("Cool We just called a c function from Javascript");
      /* Converts JSValue to char */
        size_t len;
        char *cstr;
        JSStringRef jsstr = JSValueToStringCopy(context, arguments[0], NULL);
        len = JSStringGetMaximumUTF8CStringSize(jsstr);
        cstr = g_new(char, len);
        JSStringGetUTF8CString(jsstr, cstr, len);
        g_message("%s",cstr);

    return JSValueMakeUndefined(context);
}

/* JS API FUNCTIONS DEclerations*/
static const JSStaticFunction notification_staticfuncs[] =
{
    { "notify", notification_notify_cb, kJSPropertyAttributeReadOnly },
    { NULL, NULL, 0 }
};

JSObjectRef class_constructor_cb(JSContextRef context, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
	g_message("Custom class constructor");

    return JSValueToObject(context, JSValueMakeNumber(context, 0), exception);
}

static const JSClassDefinition class_def ={
    0,                     // version
    kJSClassAttributeNone, // attributes
    "CustomClass",         // className
    NULL,                  // parentClass
    NULL,                  // staticValues
    notification_staticfuncs,  // staticFunctions
    class_init_cb,         // initialize
    class_finalize_cb,     // finalize
    NULL,                  // hasProperty
    NULL,                  // getProperty
    NULL,                  // setProperty
    NULL,                  // deleteProperty
    NULL,                  // getPropertyNames
    NULL,                  // callAsFunction
    class_constructor_cb,  // callAsConstructor
    NULL,                  // hasInstance  
    NULL                   // convertToType
};

static void window_object_cleared_cb (WebKitScriptWorld *world, WebKitWebPage *web_page, WebKitFrame *frame, gpointer user_data) {
    JSGlobalContextRef context;
    context = webkit_frame_get_javascript_context_for_script_world (frame, world);
    JSClassRef classDef = JSClassCreate(&class_def);
    JSObjectRef classObj = JSObjectMake(context, classDef, context);
    JSObjectRef globalObj = JSContextGetGlobalObject(context);
    JSStringRef str = JSStringCreateWithUTF8CString("CustomClass");
    JSObjectSetProperty(context, globalObj, str, classObj, kJSPropertyAttributeNone, NULL);
}

// initialize the extension 
G_MODULE_EXPORT void webkit_web_extension_initialize (WebKitWebExtension *extension)
{
    g_message("EXTENSION INITIALIZED\n");
    
    g_signal_connect (webkit_script_world_get_default(),
                      "window-object-cleared", 
                      G_CALLBACK (window_object_cleared_cb), 
                      NULL);
}
