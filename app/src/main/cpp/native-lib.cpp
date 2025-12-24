#include <jni.h>
#include <android/native_window_jni.h>

// EGL 및 OpenGL ES 헤더 파일 포함
#include <EGL/egl.h>
#include <GLES3/gl3.h>

// 렌더링 스레드
#include <thread>
#include <atomic>
#include <chrono>

// EGL 상태 유지 용 전역 변수
static EGLDisplay eglDisplay = EGL_NO_DISPLAY;
static EGLSurface eglSurface = EGL_NO_SURFACE;
static EGLContext eglContext = EGL_NO_CONTEXT;

// 렌더 스레드 관련 전역 변수
static std::thread renderThread;
static std::atomic<bool> isRunning(false);
static ANativeWindow* nativeWindow = nullptr;

void renderLoop(){
    // EGLContext는 스레드별로 바인딩되므로 이 스레드에 바인딩해야 함
    // eglMakeCurrent 반드시 렌더 스레드에서
    eglMakeCurrent(eglDisplay,
                   eglSurface,
                   eglSurface,
                   eglContext);

    while(isRunning){
        glClearColor(0.1,0.2,0.4,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        eglSwapBuffers(eglDisplay,eglSurface);

        // 60 FPS로 렌더링
        std::this_thread::sleep_for(
            std::chrono::microseconds(16)
        );
    }
    // 스레드 종료 시 컨텍스트 언바인딩
    eglMakeCurrent(
            eglDisplay,
            EGL_NO_SURFACE,
            EGL_NO_SURFACE,
            EGL_NO_CONTEXT
            );
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_MainActivity_onSurfaceCreated(JNIEnv *env, jobject thiz,
                                                           jobject surface) {
    // Java Surface 객체에서 ANativeWindow 생성
    nativeWindow = ANativeWindow_fromSurface(env,surface);

    // EGLDisplay 초기화 (GPU 연결)
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
        // 오류 처리: 로그 출력 후 함수 종료
//        __android_log_print(ANDROID_LOG_ERROR, "EGL", "eglGetDisplay 실패");
        return;
    }
    if (!eglInitialize(eglDisplay, nullptr, nullptr)) {
//        __android_log_print(ANDROID_LOG_ERROR, "EGL", "eglInitialize 실패");
        return;
    }
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
    eglSurface = eglCreateWindowSurface(eglDisplay,eglConfig,nativeWindow,nullptr);

    // 렌더 스레드 시작
    isRunning = true;
    renderThread = std::thread(renderLoop);
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
    isRunning = false;

    if(renderThread.joinable()){
        renderThread.join();
    }

    if(eglDisplay != EGL_DEFAULT_DISPLAY){
        eglDestroySurface(eglDisplay, eglSurface);
        eglDestroyContext(eglDisplay, eglContext);
        eglTerminate(eglDisplay);
    }
    eglSurface = EGL_NO_SURFACE;
    eglContext = EGL_NO_CONTEXT;
    eglDisplay = EGL_NO_DISPLAY;
}