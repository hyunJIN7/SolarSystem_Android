#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_solarsystem_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_MainActivity_onSurfaceCreated(JNIEnv *env, jobject thiz,
                                                           jobject surface) {
    // TODO: implement onSurfaceCreated()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_MainActivity_onSurfaceChanged(JNIEnv *env, jobject thiz, jint width,
                                                           jint height) {
    // TODO: implement onSurfaceChanged()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_MainActivity_onSurfaceDestroyed(JNIEnv *env, jobject thiz) {
    // TODO: implement onSurfaceDestroyed()
}