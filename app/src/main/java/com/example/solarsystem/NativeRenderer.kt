package com.example.solarsystem

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class NativeRenderer : GLSurfaceView.Renderer {

    // JNI 함수 선언 (C++의 이름과 정확히 매칭되어야 함)
    private external fun init()
    private external fun resize(w: Int, h: Int)
    private external fun drawFrame()

    companion object {
        init {
            // 위 CMake에서 설정한 이름과 동일해야 함
            System.loadLibrary("solarsystem")
        }
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        init()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        resize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        drawFrame()
    }
}