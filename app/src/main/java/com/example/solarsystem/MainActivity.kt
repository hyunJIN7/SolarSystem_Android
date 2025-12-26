package com.example.solarsystem

import android.opengl.GLSurfaceView
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.viewinterop.AndroidView
import com.example.solarsystem.NativeRenderer

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            OpenGLComposeScreen()
        }
    }
}

@Composable
fun OpenGLComposeScreen() {
    AndroidView(
        modifier = Modifier.fillMaxSize(),
        factory = { context ->
            GLSurfaceView(context).apply {
                // OpenGL ES 3.0 설정
                setEGLContextClientVersion(3)
                setRenderer(NativeRenderer())
                // 필요할 때만 그리도록 설정 가능 (RENDERMODE_CONTINUOUSLY가 기본)
                renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
            }
        }
    )
}