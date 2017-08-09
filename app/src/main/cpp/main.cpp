
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jni.h>

#include <dlfcn.h>

#include "elf_hooker.h"

static void *(*__old_impl_dlopen)(const char *filename, int flag);

static int (*__old_impl_connect)(int sockfd, struct sockaddr *serv_addr, int addrlen);

static void *(*__old_impl_android_dlopen_ext)(const char *filename, int flags, const void *extinfo);

static int (*__orig_sock)(int, int, int);

static int (*__orig_close)(int);

static int (*__old_impl_getaddrinfo)(const char *hostname, const char *servname,
                                     const struct addrinfo *hints, struct addrinfo **res);

static int
(*__old_impl_recvmmsg)(int, struct mmsghdr *, unsigned int, int, const struct timespec *);

static int (*__old_impl_accept)(int, struct sockaddr *, socklen_t *);

static int (*__old_impl_accept4)(int, struct sockaddr *, socklen_t *, int);

static int (*__old_impl_bind)(int, const struct sockaddr *, int);

static int (*__old_impl_getsockname)(int, struct sockaddr *, socklen_t *);

static int (*__old_impl_getsockopt)(int, int, int, void *, socklen_t *);

static int (*__old_impl_listen)(int, int);

static int (*__old_impl_recvmsg)(int, struct msghdr *, int);

static int (*__old_impl_sendmmsg)(int, const struct mmsghdr *, unsigned int, int);

static int (*__old_impl_sendmsg)(int, const struct msghdr *, int);

static int (*__old_impl_setsockopt)(int, int, int, const void *, socklen_t);

static int (*__old_impl_shutdown)(int, int);

static int (*__old_impl_socketpair)(int, int, int, int *);

void print_ip_address(const struct sockaddr *add) {
    if (NULL == add) {
        log_info("NULL sockAddr structure!");
        return;
    }
    const struct sockaddr_in *a = (const struct sockaddr_in *) add;
    char *ip = inet_ntoa(a->sin_addr);
    log_info("ip:%s, family:%d, prot:%d", ip, a->sin_family, a->sin_port);
}

//void print_msg_info(const struct mmsghdr *mmsg) {
//    if (NULL == mmsg) {
//        log_info("NULL mmsg structure!");
//        return;
//    }
//    const struct msghdr *msghdr = (const struct msghdr *) mmsg->msg_hdr;
//    log_info("msg_name:%s, msg_control:%d, msg_flags:%d", msghdr->msg_name, msghdr->msg_control,
//             msghdr->msg_flags);
//}

extern "C" {

static void *__nativehook_impl_dlopen(const char *filename, int flag) {
    log_info("__nativehook_impl_dlopen -> (%s)\n", filename);
    void *res = __old_impl_dlopen(filename, flag);
    return res;
}

static int __nativehook_impl_connect(int sockfd, struct sockaddr *serv_addr, int addrlen) {
    log_info("__nativehook_impl_connect ->\n");
    print_ip_address(serv_addr);
    int res = __old_impl_connect(sockfd, serv_addr, addrlen);
    return res;
}

static void *
__nativehook_impl_android_dlopen_ext(const char *filename, int flags, const void *extinfo) {
    log_info("__nativehook_impl_android_dlopen_ext -> (%s)\n", filename);
    void *res = __old_impl_android_dlopen_ext(filename, flags, extinfo);
    return res;
}
static int __za_socket(int family, int type, int protocol) {
    int ret = -1;
    if (NULL != __orig_sock) {
        ret = __orig_sock(family, type, protocol);
    }
    log_info("socket function ->>>");
    log_info("sock:%d, family:%d, type:%d, protocol:%d", ret, family, type, protocol);
    return ret;
}

static int __za_close(int fd) {
    int ret = __orig_close(fd);
//    log_info("close function ->>>>");
    log_info("close fd:%d result:%d\n", fd, ret);
    return ret;
}

static int __za_getaddrinfo(const char *hostname, const char *servname,
                            const struct addrinfo *hints, struct addrinfo **res) {
    int ret = -1;
    log_info("getaddrinfo function ->>>");
    log_info("hostname: %s, servname%s",
             hostname, servname);
    if (NULL != __old_impl_getaddrinfo) {
        ret = __old_impl_getaddrinfo(hostname, servname, hints, res);
    }
    return ret;
}

static int
__za_recvmmsg(int a, struct mmsghdr *b, unsigned int c, int d, const struct timespec *e) {
    int ret = -1;
//    print_msg_info(b);
    log_info("recvmmsg function->>>>>");
    if (NULL != __old_impl_recvmmsg) {
        ret = __old_impl_recvmmsg(a, b, c, d, e);
    }
    return ret;
}

static int __za_accept(int a, struct sockaddr *b, socklen_t *c) {
    int ret = -1;
    log_info("accept function->>>");
    if (NULL != __old_impl_accept) {
        ret = __old_impl_accept(a, b, c);
    }
    return ret;
}

static int __za_accept4(int a, struct sockaddr *b, socklen_t *c, int d) {
    int ret = -1;
    log_info("accept4 function->>>");
    if (NULL != __old_impl_accept4) {
        ret = __old_impl_accept4(a, b, c, d);
    }
    return ret;
}

static int __za_bind(int a, const struct sockaddr *b, int c) {
    int ret = -1;
    log_info("bind function->>>");
    if (NULL != __old_impl_bind) {
        ret = __old_impl_bind(a, b, c);
    }
    return ret;
}

static int __za_getsockname(int a, struct sockaddr *b, socklen_t *c) {
    int ret = -1;
    log_info("getsockname function->>>");
    if (NULL != __old_impl_getsockname) {
        ret = __old_impl_getsockname(a, b, c);
    }
    return ret;
}

static int __za_getsockopt(int a, int b, int c, void *d, socklen_t *e) {
    int ret = -1;
    log_info("getsockopt function ->>>");
    if (NULL != __old_impl_getsockopt) {
        ret = __old_impl_getsockopt(a, b, c, d, e);
    }
    return ret;
}

static int __za_listen(int a, int b) {
    int ret = -1;
    log_info("listen function ->>>");
    if (NULL != __old_impl_listen) {
        ret = __old_impl_listen(a, b);
    }
    return ret;
}

static int __za_recvmsg(int a, struct msghdr *b, int c) {
    int ret = -1;
    log_info("recvmsg function ->>>");
    if (NULL != __old_impl_recvmsg) {
        ret = __old_impl_recvmsg(a, b, c);
    }
    return ret;
}

static int __za_sendmmsg(int a, const struct mmsghdr *b, unsigned int c, int d) {
    int ret = -1;
    log_info("sendmmsg function->>>");
    if (NULL != __old_impl_sendmmsg) {
        ret = __old_impl_sendmmsg(a, b, c, d);
    }
    return ret;
}

static int __za_sendmsg(int a, const struct msghdr *b, int c) {
    int ret = -1;
    log_info("sendmsg function ->>>");
    if (NULL != __old_impl_sendmsg) {
        ret = __old_impl_sendmsg(a, b, c);
    }
    return ret;
}

static int __za_setsockopt(int a, int b, int c, const void *d, socklen_t e) {
    int ret = -1;
    log_info("setsockopt function ->>>");
    if (NULL != __old_impl_setsockopt) {
        ret = __old_impl_setsockopt(a, b, c, d, e);
    }
    return ret;
}

static int __za_shutdown(int a, int b) {
    int ret = -1;
    log_info("shutdown function ->>>");
    if (NULL != __old_impl_shutdown) {
        ret = __old_impl_shutdown(a, b);
    }
    return ret;
}

static int __za_socketpair(int a, int b, int c, int *d) {
    int ret = -1;
    log_info("socketpair function ->>>");
    if (NULL != __old_impl_socketpair) {
        ret = __old_impl_socketpair(a, b, c, d);
    }
    return ret;
}

}

static bool __prehook(const char *module_name, const char *func_name) {
    if (strstr(module_name, "libwebviewchromium.so") != NULL) {
        return true;
    }
    return false;
}

#if (ELFHOOK_STANDALONE)

int main(int argc, char* argv[])
{
    // char ch = 0;
    // elf_hooker hooker;
    //
    // void* h = dlopen("libart.so", RTLD_LAZY);
    // void* f = dlsym(h,"artAllocObjectFromCodeResolvedRegion");
    // log_info("artAllocObjectFromCodeResolvedRegion : %p\n", f);
    //
    // hooker.set_prehook_cb(__prehook);
    // hooker.phrase_proc_maps();
    // hooker.dump_module_list();
    // hooker.hook_all_modules("dlopen", (void*)__nativehook_impl_dlopen, (void**)&__old_impl_dlopen);
    // hooker.hook_all_modules("connect", (void*)__nativehook_impl_connect, (void**)&__old_impl_connect);
    //
    // do {
    //     ch = getc(stdin);
    // } while(ch != 'q');
    return 0;
}

#else

#include <jni.h>

static char *__class_name = "com/bsty/jnihook/ElfHooker";
static elf_hooker __hooker;
static JavaVM *__java_vm = NULL;
static bool __is_attached = false;

static JNIEnv *__getEnv(bool *attached);

static void __releaseEnv(bool attached);

static int __set_hook(JNIEnv *env, jobject thiz);

static int __test(JNIEnv *env, jobject thiz);

static int __elfhooker_init(JavaVM *vm, JNIEnv *env);

static void __elfhooker_deinit(void);

static JNINativeMethod __methods[] =
        {
                {"setHook", "()I", (void *) __set_hook},
                {"test",    "()I", (void *) __test},
        };

static int __set_hook(JNIEnv *env, jobject thiz) {
    log_info("__set_hook() -->\r\n");
//    __hooker.set_prehook_cb(__prehook);
    __hooker.phrase_proc_maps();
    __hooker.dump_module_list();
//    __hooker.hook_all_modules("dlopen", (void*)__nativehook_impl_dlopen, (void**)&__old_impl_dlopen);
//    __hooker.hook_all_modules("connect", (void*)__nativehook_impl_connect, (void**)&__old_impl_connect);
//    __hooker.hook_all_modules("android_dlopen_ext", (void*)__nativehook_impl_android_dlopen_ext, (void**)&__old_impl_android_dlopen_ext);

//    elf_module *module = __hooker.create_module("libart.so");
    elf_module *libc_module = __hooker.create_module("libc.so");
//    log_info("module base:%lx, %lx, %s\n",
//             (unsigned long) module->get_base_addr(),
//             (unsigned long) module->get_bias_addr(),
//             module->get_module_name());


//    module->hook("dlopen", (void *) __nativehook_impl_dlopen, (void **) &__old_impl_dlopen);
//    module->hook("android_dlopen_ext", (void *) __nativehook_impl_android_dlopen_ext,
//                 (void **) &__old_impl_android_dlopen_ext);
    libc_module->hook("socket", (void *) __za_socket, (void **) &__orig_sock);
//    libc_module->hook("connect", (void *) __nativehook_impl_connect, (void **) &__old_impl_connect);
//    module->hook("close", (void *) __za_close, (void **) &__orig_close);
//    libc_module->hook("getaddrinfo", (void *) __za_getaddrinfo, (void **) &__old_impl_getaddrinfo);
//    libc_module->hook("recvmmsg", (void *) __za_recvmmsg, (void **) &__old_impl_recvmmsg);
//    libc_module->hook("accept", (void *) __za_accept, (void **) &__old_impl_accept);
//    libc_module->hook("accept4", (void *) __za_accept4, (void **) &__old_impl_accept4);
//    libc_module->hook("bind", (void *) __za_bind, (void **) &__old_impl_bind);
//    libc_module->hook("getsockname", (void *) __za_getsockname, (void **) &__old_impl_getsockname);
//    libc_module->hook("getsockopt", (void *) __za_getsockopt, (void **) &__old_impl_getsockopt);
//    libc_module->hook("listen", (void *) __za_listen, (void **) &__old_impl_listen);
//    libc_module->hook("recvmmsg", (void *) __za_recvmmsg, (void **) &__old_impl_recvmmsg);
//    libc_module->hook("recvmsg", (void *) __za_recvmsg, (void **) &__old_impl_recvmsg);
//    libc_module->hook("sendmmsg", (void *) __za_sendmmsg, (void **) &__old_impl_sendmmsg);
//    libc_module->hook("sendmsg", (void *) __za_sendmsg, (void **) &__old_impl_sendmsg);
//    libc_module->hook("setsockopt", (void *) __za_setsockopt, (void **) &__old_impl_setsockopt);
//    libc_module->hook("shutdown", (void *) __za_shutdown, (void **) &__old_impl_shutdown);
//    libc_module->hook("socketpair", (void *) __za_socketpair, (void **) &__old_impl_socketpair);libc_module->hook("accept", (void *) __za_accept, (void **) &__old_impl_accept);

//    module->hook("accept4", (void *) __za_accept4, (void **) &__old_impl_accept4);
//    module->hook("bind", (void *) __za_bind, (void **) &__old_impl_bind);
//    module->hook("getsockname", (void *) __za_getsockname, (void **) &__old_impl_getsockname);
//    module->hook("getsockopt", (void *) __za_getsockopt, (void **) &__old_impl_getsockopt);
//    module->hook("listen", (void *) __za_listen, (void **) &__old_impl_listen);
//    module->hook("recvmmsg", (void *) __za_recvmmsg, (void **) &__old_impl_recvmmsg);
//    module->hook("recvmsg", (void *) __za_recvmsg, (void **) &__old_impl_recvmsg);
//    module->hook("sendmmsg", (void *) __za_sendmmsg, (void **) &__old_impl_sendmmsg);
//    module->hook("sendmsg", (void *) __za_sendmsg, (void **) &__old_impl_sendmsg);
//    module->hook("setsockopt", (void *) __za_setsockopt, (void **) &__old_impl_setsockopt);
//    module->hook("shutdown", (void *) __za_shutdown, (void **) &__old_impl_shutdown);
//    module->hook("socketpair", (void *) __za_socketpair, (void **) &__old_impl_socketpair);
//     _hooker.hook_all_modules("close", (void*)__za_close, (void**)&__orig_close);
#if 0
    void* h = dlopen("libart.so", RTLD_LAZY);
    if (h != NULL) {
        void* f = dlsym(h,"artAllocObjectFromCodeResolvedRegion");
        log_info("artAllocObjectFromCodeResolvedRegion : %p\n", f);
    } else {
        log_error("open libart.so fail\n");
    }
#endif
    return 0;
}

static int __test(JNIEnv *env, jobject thiz) {
    log_info("__test() -->\r\n");
    __hooker.dump_proc_maps();
    return 0;
}

static int __elfhooker_register_native_methods(JNIEnv *env, const char *class_name,
                                               JNINativeMethod *methods, int num_methods) {

    log_info("RegisterNatives start for \'%s\'", __class_name);

    jclass clazz = env->FindClass(class_name);
    if (clazz == NULL) {
        log_error("Native registration unable to find class \'%s\'", class_name);
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, methods, num_methods) < 0) {
        log_error("RegisterNatives failed for \'%s\'", class_name);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static int __elfhooker_init(JavaVM *vm, JNIEnv *env) {
    log_info("hookwrapper_init() -->\r\n");
    if (!__elfhooker_register_native_methods(env, __class_name,
                                             __methods, sizeof(__methods) / sizeof(__methods[0]))) {
        log_error("register hookJNIMethod fail, \r\n");
        __elfhooker_deinit();
        return -2;
    }

    return 0;
}

static void __elfhooker_deinit(void) {
    log_info("hookwrapper_deinit()->\r\n");
    return;
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    bool attached;
    __java_vm = vm;

    if ((env = __getEnv(&__is_attached)) == NULL) {
        log_error("getEnv fail\r\n");
        return -1;
    }
    assert(!__is_attached);
    if (__elfhooker_init(vm, env) < 0) {
        log_error("__elfhooker_init fail\r\n");
        return -1;
    }
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    bool attached;
    JNIEnv *env = __getEnv(&__is_attached);
    assert(!__is_attached);

    __elfhooker_deinit();
    return;
}

static JNIEnv *__getEnv(bool *attached) {
    JNIEnv *env = NULL;
    *attached = false;
    int ret = __java_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (ret == JNI_EDETACHED) {
        if (0 != __java_vm->AttachCurrentThread(&env, NULL)) {
            return NULL;
        }
        *attached = true;
        return env;
    }

    if (ret != JNI_OK) {
        return NULL;
    }

    return env;
}

static void __releaseEnv(bool attached) {
    if (attached)
        __java_vm->DetachCurrentThread();
}

JNIEXPORT void JNICALL
Java_com_bsty_jnihook_MainActivity_hook(JNIEnv *env, jobject instance) {

    // TODO

    __set_hook(env, instance);

}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_bsty_jnihook_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

#endif

