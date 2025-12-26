#include <jni.h>
#include <GLES3/gl3.h>
#include <vector>

// 쉐이더 소스 (Vertex Shader)
const char* vShaderCode =
        "#version 300 es\n"
        "layout(location = 0) in vec3 vPosition;\n"
        "layout(location = 1) in vec4 vColor;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
        "  gl_Position = vec4(vPosition, 1.0);\n"
        "  outColor = vColor;\n"
        "}\n";

// 쉐이더 소스 (Fragment Shader)
const char* fShaderCode =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec4 outColor;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "  fragColor = outColor;\n"
        "}\n";

// 전역 변수
GLuint program;
GLuint vao;
GLuint vbo;

// 내부 헬퍼 함수 (C++ 내부에서만 쓰므로 extern "C" 밖에 있어도 무방)
GLuint loadShader(GLenum type, const char* shaderCode) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    return shader;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_NativeRenderer_init(JNIEnv *env, jobject thiz) {
    // 1. 프로그램 생성
    GLuint vShader = loadShader(GL_VERTEX_SHADER, vShaderCode);
    GLuint fShader = loadShader(GL_FRAGMENT_SHADER, fShaderCode);
    program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    // 2. 데이터 준비 (Interleaved: Position + Color)
    GLfloat vertices[] = {
            // x,     y,     z,      r,    g,    b,    a
            0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, // 상단 (Red)
            -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, // 좌하 (Green)
            0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f  // 우하 (Blue)
    };

    // 3. VAO 생성 및 바인딩
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 4. VBO 생성 및 데이터 업로드
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 5. 정점 속성 정의 (Stride = 7 floats * 4 bytes = 28 bytes)
    GLsizei stride = 7 * sizeof(GLfloat);

    // 위치 속성 (0번 슬롯, 크기 3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // 색상 속성 (1번 슬롯, 크기 4, 오프셋은 float 3개 뒤)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 6. 바인딩 해제 (안전핀)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_NativeRenderer_resize(JNIEnv *env, jobject thiz, jint w, jint h) {
    glViewport(0, 0, w, h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_solarsystem_NativeRenderer_drawFrame(JNIEnv *env, jobject thiz) {
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    // VAO만 바인딩하면 모든 설정이 복구됨
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
}