#include <jni.h>
#include <GLES3/gl3.h>
#include <vector>

// 쉐이더 소스
const char* vShaderCode =
        "#version 300 es\n"
        "layout(location = 0) in vec4 vPosition;\n"
        "layout(location = 1) in vec4 vColor;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
        "  gl_Position = vPosition;\n"
        "  outColor = vColor;\n"
        "}\n";

// 쉐이더 소스 (Fragment Shader: 보간된 색상을 출력)
const char* fShaderCode =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec4 outColor;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "  fragColor = outColor;\n"
        "}\n";

GLuint program;

GLuint loadShader(GLenum type, const char* shaderCode) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    return shader;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_NativeRenderer_init(JNIEnv *env, jobject thiz) {
    GLuint vShader = loadShader(GL_VERTEX_SHADER, vShaderCode);
    GLuint fShader = loadShader(GL_FRAGMENT_SHADER, fShaderCode);
    program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_NativeRenderer_resize(JNIEnv *env, jobject thiz, jint w, jint h) {
    glViewport(0, 0, w, h);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_NativeRenderer_drawFrame(JNIEnv *env, jobject thiz) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    GLfloat vertices[] = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
    GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}