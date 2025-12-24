#include <jni.h>
#include <android/native_window_jni.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

// 상태 유지 용 전역 변수
static EGLDisplay eglDisplay = EGL_NO_DISPLAY;
static EGLSurface eglSurface = EGL_NO_SURFACE;
static EGLContext eglContext = EGL_NO_CONTEXT;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_MainActivity_onSurfaceCreated(JNIEnv *env, jobject thiz,
                                                           jobject surface) {
    // Java Surface 객체에서 ANativeWindow 생성
    ANativeWindow* window = ANativeWindow_fromSurface(env,surface);

    // EGLDisplay 초기화 (GPU 연결)
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(eglDisplay, nullptr, nullptr);

    // EGLConfig 설정(픽셀 포맷)
    const EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };
    EGLConfig eglConfig;
    EGLint numConfigs;
    eglChooseConfig(eglDisplay,configAttribs,&eglConfig,1,&numConfigs);

    // OpenGL ES 3.0 컨텍스트 생성
    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
    eglContext = eglCreateContext(eglDisplay,eglConfig,EGL_NO_CONTEXT,contextAttribs);

    // EGLSurface 생성 (렌더링 대상, Window에 붙이기)
    eglSurface = eglCreateWindowSurface(eglDisplay,eglConfig,window,nullptr);

    // 컨텍스트와 서피스를 현재 스레드에 바인딩
    eglMakeCurrent(
            eglDisplay,
            eglSurface,
            eglSurface,
            eglContext
            );

    // OpenGL 테스트 렌더링
    glViewport(0,0,ANativeWindow_getWidth(window),ANativeWindow_getHeight(window));
    glClearColor(0.1f,0.2f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 화면에 출력
    eglSwapBuffers(eglDisplay,eglSurface);
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
    if(eglDisplay != EGL_DEFAULT_DISPLAY){
        eglMakeCurrent(
                eglDisplay,
                EGL_NO_SURFACE,
                EGL_NO_SURFACE,
                EGL_NO_CONTEXT
        );
        if(eglSurface != EGL_NO_SURFACE){
            eglDestroySurface(eglDisplay,eglSurface);
            eglSurface = EGL_NO_SURFACE;
        }
        if(eglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(eglDisplay, eglContext);
            eglContext = EGL_NO_CONTEXT;
        }

        eglTerminate(eglDisplay);
        eglDisplay = EGL_NO_DISPLAY;
    }
}