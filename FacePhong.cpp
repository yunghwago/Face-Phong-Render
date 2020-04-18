// FacePhong.cpp: renders a face
// Christopher Go and Alex Runciman

//libraries
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include <time.h>
#include <VecMat.h>
#include "Camera.h"
#include <vector>
#include <array>

GLFWwindow *windowClone;
GLFWwindow *window;

// GPU identifiers
GLuint vBuffer = 0; // GPU vertex buffer, valid > 0
GLuint program = 0;

// camera
int initialScreenWidth = 600, initialScreenHeight = 600; //size of application window
Camera camera(initialScreenWidth / 2, initialScreenHeight, vec3(0,0,0), vec3(0, 0, -1), 30);

//variables for mouse functionality
vec2 mouseDown; 
vec2 rotOld, rotNew;
vec3 tranOld, tranNew(0, 0, -1);
float rotSpeed = .3f; 

#define NPTS 57
vec3 normals[NPTS];

/* Vertices Index for the face */
//list of vertices
vec3 points[] = {
	//#: (X, Y, Z)
	//left side of face
	//forehead
	vec3(0, 5.5f, 1.5f), //0
	vec3(0, 4.1f, 1.0f), //1
	vec3(1.4f, 4.5f, 1.2f), //2
	vec3(1.7f, 4.95f, 2), //3
	vec3(2.7f, 4, 2), //4
	vec3(2.65f, 5, 2.5f), //5
	vec3(3.3f, 3.3f, 2.8f), //6
	vec3(3.3f, 4.5f, 3), //7
	vec3(3.9f, 2.6f, 4), //8
	vec3(3.8f, 2.5f, 3.5), //9
	vec3(0, 3.05f, 0.8f), //10
	vec3(1, 3.1f, 0.8f), //11
	vec3(2, 3.4f, 1), //12
	vec3(2.7f, 3.4f, 1.5f), //13
	vec3(3.8f, 3.2f, 2), //14
	vec3(3.8f, 2.1f, 2), //15
	//bridge of nose
	vec3(0, 2, 0.9f), //16
	vec3(1.2f, 2.6f, 1.2f), //17
	//eyebrow
	vec3(2.5f, 2.7f, 1.8f), //18
	vec3(3.2f, 1.9f, 2.4f), //19
	//bridge of nose
	vec3(0, 0.6f, 0.3f), //20
	vec3(0.5f, 0.5f, 0.6f), //21
	vec3(1, 1.9f, 1.2f), //22
	vec3(1, 0.4f, 1), //23
	//upper cheek
	vec3(1.8f, 1, 1.4f), //24
	vec3(3, 0.8f, 2), //25
	vec3(3.2f, 1.5f, 2.5f), //26
	vec3(3.6f, 1.1f, 2.65f), //27
	vec3(3.95f, 1.4f, 3.6f), //28
	//middle cheek
	vec3(1.6f, -0.2f, 1.2f), //29
	vec3(2.5f, 0.2f, 1.5f), //30
	vec3(3.7f, 0.2f, 3), //31
	vec3(3.8f, 0.2f, 4), //32
	vec3(2, -1.5f, 1.5f), //33
	vec3(3, -1.1f, 2), //34
	vec3(3.4f, -0.6f, 2.5f), //35
	vec3(3.7f, -0.9f, 3.95f), //36
	//bulb of nose
	vec3(0, 0.1f, 0), //37
	vec3(0.8f, 0, 0.5f), //38
	vec3(1.1f, -0.3f, 1), //39
	//upper lip
	vec3(0, -0.4f, 0.6f),//40
	vec3(0, -1.25f, 0.6f),//41
	vec3(0.3f, -0.5f, 0.6f),//42
	vec3(0.4f, -1.2f, 0.6f),//43
	vec3(0.85f, -0.5f, 0.8f),//44
	vec3(1.2f, -1.3f, 0.9f),//45
	//lower lip
	vec3(0, -2.1f, 0.8f),//46
	vec3(0, -3.05f, 0.75f),//47
	vec3(1.1f, -2.05f, 1),//48
	vec3(1.4f, -3.1f, 1.2f),//49
	vec3(2.1f, -2.5f, 1.8f),//50
	//lower cheek
	vec3(3, -2.1f, 2.5f),//51
	vec3(3.4f, -1.6f, 3.5f),//52
	//chin
	vec3(0, -3.7f, 1),//53
	vec3(1.4f, -3.7f, 1.2f),//54
	//jawline
	vec3(2, -3.6f, 2),//55
	vec3(2.5f, -3.15f, 3),//56

	
};
float colors[][3] = { { 0,0,1 },{ 0,1,0 },{ 0,1,1 },{ 1,0,0 },{ 1,0,1 },{ 1,1,0 },{ 0,0,0 },{ 1,1,1 } }; // 8 colors
int triangles[][3] = {
	//forehead

	{ 2, 0, 1 }, //t0
	{ 2, 3, 0 }, //t1
	{ 4, 3, 2 }, //t2
	{ 4, 5, 3 }, //t3
	{ 6, 5, 4 }, //t4
	{ 6, 7, 5 }, //t5
	{ 9, 7, 6 }, //t6
	{ 9, 8, 7 }, //t7
	{ 11, 1, 10 }, //t8
	{ 11, 2, 1 }, //t9

			
	{ 12, 2, 11 }, //t10
	{ 13, 2, 12 }, //t11
	{ 13, 4, 2 }, //t12
	{ 14, 4, 13 }, //t13

	{ 14, 6, 4 }, //t14
	{ 15, 6, 14 }, //t15
	{ 15, 9, 6 }, //t16
	//bridge of nose
	{ 17,11,10 }, //t17
	{ 17, 10, 16 }, //t18


	//eyebrow
	{ 18, 11, 17 }, //t19
	{ 18, 12, 11 }, //t20

	{ 18, 13, 12 }, //t21
	{ 18, 14, 13}, //t22
	{ 18, 19, 14}, //t23
	{ 19, 15, 14 }, //t24
	//bridge of nose
	{ 21, 16, 20 }, //t25
	{ 21, 22, 16 }, //t26
	{ 23, 22, 21}, //t27
	{ 22, 17,16 }, //t28
	//upper cheek
	{ 24, 22, 23}, //t29
	{ 25, 22, 24}, //t30
	{ 25, 26, 22 }, //t31
	{ 27, 26, 25}, //t32

	{ 27, 15, 26}, //t33
	{ 28, 15, 27}, //t34
	{ 28, 9, 15}, //t35

	{ 30, 24, 23 }, //t36
	{ 23, 29, 30 }, //t37
	{ 30, 25, 24 }, //t38
	{ 27, 25, 31 }, //t39
	{ 28, 27, 31 }, //t40
	{ 28, 31, 32 }, //t41

	//middle cheek
	{ 29, 33, 34 }, //t42
	{ 29, 34, 30 }, //t43
	//offset
	{ 30, 34, 35 }, //t44
	{ 30, 35, 25 }, //t45
	{ 25, 35, 31 }, //t46
	{ 31, 35, 32 }, //t47
	{ 32, 35, 36 }, //t48
	//bulb of nose

	{ 38, 20, 37}, //t49
	{ 38, 21, 20 }, //t50
	{ 38, 23, 21 }, //t51
	{ 38, 39, 23 }, //t52
	{ 39, 29, 23 }, //t53

	//upper lip
	{ 40, 41, 43 }, //t54
	{ 40, 43, 42 }, //t55
	{ 42, 43, 45 }, //t56
	{ 44, 42, 45 }, //t57
	{ 44, 45, 33 }, //t58
	{ 39, 44, 33 }, //t59
	{ 29, 39, 33 }, //t60

	//lower lip
	{ 46, 47, 49 }, //t61
	{ 46, 49, 48 }, //t62
	{ 48, 49, 50 }, //t63
	{ 48, 50, 33 }, //t64

	//lower cheek
	{ 33, 50, 51 }, //t65
	{ 34, 33, 51 }, //t66

	{ 34, 51, 52 }, //t67
	{ 35, 34, 52 }, //t68
	{ 36, 35, 52 }, //t69
	//chin
	{ 47, 53, 54 }, //t70
	{ 47, 54, 49 }, //t71
	{ 49, 54, 55 }, //t72
	{ 50, 49, 55 }, //t73

	//jawline
	{ 50, 55, 56 }, //t74
	{ 51, 50, 56 }, //t75
	{ 52, 51, 56 }, //t76

	//additional triangles to fill in blanks
	{26, 15, 19} //t77

	//triangles = t +1 because we started at 0
};
int ntriangles = 78;
float faceSize = .05f, faceStretch = faceSize;

const char *usage = "\nUsage\n\
    mouse-drag:\t\trotate\n\
    with shift:\t\ttranslate xy\n\
    mouse-wheel:\ttranslate z\n\
    s:\tstretch cube\n\
    f:\tchange field-of-view\n";

const char *vertexShader = "\
	#version 130								\n\
	in vec3 point;								\n\
	in vec3 normal;								\n\
	out vec3 vPoint;							\n\
	out vec3 vNormal;							\n\
	uniform mat4 modelview;						\n\
	uniform mat4 persp;							\n\
	void main() {								\n\
		gl_Position = persp*modelview*vec4(point, 1);								\n\
		vNormal = normalize((modelview*vec4(normal, 0)).xyz);						\n\
		vPoint = (modelview*vec4(point, 1)).xyz; //light vector			\n\
	}";

const char *pixelShader = "\
	#version 130								\n\
	in vec3 vPoint;								\n\
	in vec3 vNormal;							\n\
	out vec4 pColor;							\n\
	uniform vec3 lightPos = vec3(1,0,0);		\n\
	uniform vec3 color = vec3(0,1,1);			\n\
	uniform float a =.1;						\n\
	vec3 N = normalize(vNormal);       	// unit-length surface normal	\n\
	void main() {														\n\
		vec3 L = normalize(lightPos-vPoint);	//light vector			\n\
		float d = abs(dot(N,L)); //one sided  diffuse					\n\
		vec3 R = reflect(L, N);				// reflected light vector	\n\
		vec3 E = normalize(vPoint);			// eye vector				\n\
		float h = max(0, dot(R, E));			// highlight term		\n\
		float s = pow(h, 100);				// specular term			\n\
		float intensity = clamp(a + d + s, 0, 1);						\n\
		pColor = vec4(intensity * color, 1);							\n\
	}";

int sizePts = (sizeof(points) / sizeof(vec3)) * sizeof(vec3);

void InitVertexBuffer() {
	for (int i = 0; i < ntriangles; i++) {
		int *t = triangles[i];
		vec3 p1 = points[t[0]], p2 = points[t[1]], p3 = points[t[2]];
		vec3 n = normalize(cross(p3 - p2, p2 - p1));
		for (int k = 0; k < 3; k++) {
			normals[t[k]] += n;
		}
	}
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	// allocate memory for vertex positions and normals
	
	int sizeNrms = (sizeof(normals)/sizeof(vec3)) * sizeof(vec3);
	glBufferData(GL_ARRAY_BUFFER, sizePts + sizeNrms, NULL, GL_STATIC_DRAW);
	// copy data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizePts, &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizePts, sizeNrms, &normals[0]);

	for (int i = 0; i < NPTS; i++) {
		normals[i] = normalize(normals[i]);
	}

}

//resize object
void Resize(GLFWwindow *window, int width, int height) {
	camera.Resize(width / 2, height);
}

// camera pan
bool Shift(GLFWwindow *w) {
	return glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
		glfwGetKey(w, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
}

void MouseButton(GLFWwindow *w, int butn, int action, int mods) {
	if (action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(w, &x, &y);
		camera.MouseDown((int)x, (int)y);
	}
	if (action == GLFW_RELEASE)
		camera.MouseUp();
}

void MouseMove(GLFWwindow *w, double x, double y) {
	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera.MouseDrag((int)x, (int)y, Shift(w));
}

void MouseWheel(GLFWwindow *w, double xoffset, double yoffset) {
	camera.MouseWheel(yoffset, Shift(w));
}

void Display() {
	// clear screen
	glClearColor(.7, .7, .7, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	// enable depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// enable blend and line smoothing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	int screenHeight, screenWidth;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	int halfWidth = screenWidth / 2;
	
	// access GPU vertex buffer
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

	glViewport(0, 0, screenWidth, screenHeight);

	VertexAttribPointer(program, "point", 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	VertexAttribPointer(program, "normal", 3, GL_FLOAT, GL_FALSE, 0, (void *)sizePts);

	// send vertex information to shader
	SetUniform(program, "modelview", camera.modelview * Scale(faceSize, faceSize, faceStretch));
	SetUniform(program, "persp", camera.persp);
	SetUniform(program, "lightPos", vec3(.7f, .4f, -.2f));
	
	// actually draw face
	glDrawElements(GL_TRIANGLES, sizeof(triangles)/sizeof(int), GL_UNSIGNED_INT, triangles);
	//generate right half of face
	for (int i = 0; i < NPTS; i++)
	{
		points[i].x = -points[i].x;
	}
	InitVertexBuffer();
	VertexAttribPointer(program, "point", 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	VertexAttribPointer(program, "normal", 3, GL_FLOAT, GL_FALSE, 0, (void *)sizePts);
	//draw right half of face 
	glDrawElements(GL_TRIANGLES, sizeof(triangles) / sizeof(int), GL_UNSIGNED_INT, triangles);

	glFlush();
}

void ErrorGFLW(int id, const char *reason) {
	printf("GFLW error %i: %s\n", id, reason);
}

// keyboard functionality
static void Keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		float fieldOfView = camera.GetFOV();
		bool shift = mods & GLFW_MOD_SHIFT;
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		fieldOfView += key == 'F' ? shift ? -5 : 5 : 0;
		fieldOfView = fieldOfView < 5 ? 5 : fieldOfView > 150 ? 150 : fieldOfView;
		faceStretch *= key == 'S' ? shift ? .9f : 1.1f : 1;
		faceStretch = faceStretch < .02f ? .02f : faceStretch;
		camera.SetFOV(fieldOfView);
	}
}

void Normalize(vec3 &points) {
	// scale and offset so that points all within +/-1 in x, y, and z
	vec3 mn(FLT_MAX), mx(-FLT_MAX);
	for (int i = 0; i < (sizeof(points)/sizeof(int)); i++) {
		vec3 p = points[i];
		for (int k = 0; k < 3; k++) {
			if (p[k] < mn[k]) mn[k] = p[k];
			if (p[k] > mx[k]) mx[k] = p[k];
		}
	}
	vec3 center = .5f*(mn + mx);
	float maxrange = 0;
	for (int k = 0; k < 3; k++)
		if (mx[k] - mn[k] > maxrange)
			maxrange = mx[k] - mn[k];
	float s = 2.f / maxrange;
	for (int i = 0; i < (sizeof(points) / sizeof(int)); i++) {
		(vec3)points[i] = s * ((vec3)points[i] - center);
	}
}

int main() {
	for (int i = 0; i < NPTS; i++) {
		normals[i] = vec3(0, 0, 0);
	}
	Normalize(*points);

	camera.SetSpeed(.3f, .003f);
	glfwInit();
	window = glfwCreateWindow(initialScreenWidth, initialScreenHeight, "Craig's Face", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	InitVertexBuffer();
	printf(usage);

	// callbacks to enable input
	glfwSetCursorPosCallback(window, MouseMove);
	glfwSetMouseButtonCallback(window, MouseButton);
	glfwSetScrollCallback(window, MouseWheel);
	glfwSetKeyCallback(window, Keyboard);
	glfwSetWindowSizeCallback(window, Resize);
	// loop program until user closes it
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		Display();
		glfwSwapBuffers(window);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vBuffer);
	glfwDestroyWindow(window);
	glfwTerminate();
}
