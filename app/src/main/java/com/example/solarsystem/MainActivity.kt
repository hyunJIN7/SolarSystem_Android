package com.example.solarsystem

import android.content.Context
import android.os.Bundle
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import androidx.activity.ComponentActivity
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.viewinterop.AndroidView
import androidx.activity.compose.setContent

class MainActivity : ComponentActivity() {
    companion object {
        // Used to load the 'solarsystem' library on application startup.
        init {
            System.loadLibrary("solarsystem")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent{
            MainScrenn()
        }
    }

    @Composable
    fun MainScrenn() {
        AndroidView(
            modifier = Modifier,
            factory = {
                context->
                NativeSurfaceView(context)
            }
        )
    }


    inner class NativeSurfaceView(context: Context) : SurfaceView(context), SurfaceHolder.Callback {
        init {
            holder.addCallback(this)
        }
        override fun surfaceCreated(holder: SurfaceHolder) {
            onSurfaceCreated(holder.surface)
        }
        override fun surfaceChanged(
            holder: SurfaceHolder,
            format: Int,
            width: Int,
            height: Int
        ) {
            onSurfaceChanged(width, height)
        }
        override fun surfaceDestroyed(holder: SurfaceHolder) {
            onSurfaceDestroyed()
        }
    }


    // JNI 함수 선언. Bridege to native code
    external fun onSurfaceCreated(surface : Surface)
    external fun onSurfaceChanged(width : Int, height : Int)
    external fun onSurfaceDestroyed()
}