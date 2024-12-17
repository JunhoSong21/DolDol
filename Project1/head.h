#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#define M_PI 3.14
GLuint vao, vbo[2], ebo;
GLvoid drawScene(GLvoid);
GLvoid TimerFunction(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void convertXY(int x, int y, float& fx, float& fy);
void InitBuffer();
void make_vertexShaders();
void make_fragmentShaders();
void display();
void make_shaderProgram();
// GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);
// void InitTexture();
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�
float bGCr = 0.0, bGCg = 0.0, bGCb = 0.0;
GLuint shaderPID;
GLvoid Reshape(int w, int h);
#define WIDTH 1600
#define HEIGHT 1200
float fx = 0.0, fy = 0.0;
#define MAX_LINE_LENGTH 1000
GLint g_window_w, g_window_h;
int finish = 0;
// unsigned int texture;

glm::vec3 cameraPos1 = glm::vec3(0.0f, 0.0f, 10.0f); // ���ʿ��� ī�޶�
glm::vec3 cameraDirection1 = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp1 = glm::vec3(0.0f, 1.0f, 0.0f); // ������ Y������ ����


glm::vec3 cameraPos2 = glm::vec3(0.0f, 30.0f, 0.0f); // ���ʿ��� ī�޶�
glm::vec3 cameraDirection2 = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp2 = glm::vec3(0.0f, 0.0f, -1.0f); // ������ Y������ ����

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

//GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info) {
//    FILE* fp;
//    GLubyte* bits;
//    int bitsize, infosize;
//    BITMAPFILEHEADER header;
//    //--- ���̳ʸ��б��������������
//    if ((fp = fopen(filename, "rb")) == NULL)
//        return NULL;
//    //--- ��Ʈ������������д´�.
//    if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
//        fclose(fp);
//        return NULL;
//    }
//    //--- ������BMP ��������Ȯ���Ѵ�.
//    if (header.bfType != 'MB') {
//        fclose(fp);
//        return NULL;
//    }
//    //--- BITMAPINFOHEADER ��ġ�ΰ���.
//    infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
//    //--- ��Ʈ���̹��������͸������޸��Ҵ����Ѵ�.
//    if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
//        fclose(fp);
//        return NULL;
//    }
//    //--- ��Ʈ������������д´�.
//    if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
//        free(*info);
//        fclose(fp);
//        return NULL;
//    }
//    //--- ��Ʈ����ũ�⼳��
//    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0) {
//        bitsize = ((*info)->bmiHeader.biWidth *
//            (*info)->bmiHeader.biBitCount + 7) / 8.0 *
//            abs((*info)->bmiHeader.biHeight);
//    }
//    //--- ��Ʈ����ũ�⸸ŭ�޸𸮸��Ҵ��Ѵ�.
//    if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
//        free(*info);
//        fclose(fp);
//        return NULL;
//    }
//    //--- ��Ʈ�ʵ����͸�bit(GLubyte Ÿ��)�������Ѵ�.
//    if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
//        free(*info); free(bits);
//        fclose(fp);
//        return NULL;
//    }
//    fclose(fp);
//    return bits;
//}

GLvoid Reshape(int w, int h) {
	display();  // �ʿ��� ��쿡�� ȣ��
}

void display() {
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	GLuint viewTransformLoc = glGetUniformLocation(shaderProgramID, "view");
	glm::mat4 vTransform1 = glm::mat4(1.0f);
	glm::mat4 vTransform2 = glm::mat4(1.0f);
	glm::mat4 vTransform3 = glm::mat4(1.0f);
	glm::mat4 vTransform4 = glm::mat4(1.0f);

	unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ

    glViewport(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
    vTransform1 = glm::lookAt(cameraPos2, cameraDirection2, cameraUp2);
    glUniform3f(viewPosLocation, cameraPos2.x, cameraPos2.y, cameraPos2.z);
    glUniformMatrix4fv(viewTransformLoc, 1, GL_FALSE, &vTransform1[0][0]);
    drawScene();

	glViewport(0, 0, WIDTH, HEIGHT);
	vTransform1 = glm::lookAt(cameraPos1, cameraDirection1, cameraUp1);
	glUniform3f(viewPosLocation, cameraPos1.x, cameraPos1.y, cameraPos1.z);
	glUniformMatrix4fv(viewTransformLoc, 1, GL_FALSE, &vTransform1[0][0]);
	drawScene();

	glutSwapBuffers();
}

void convertXY(int w, int h, float& fx, float& fy)
{
	float x = WIDTH;
	float y = HEIGHT;
	float g_window_w = w;
	float g_window_h = h;
	fx = (2.0f * x / w) - 1.0f;
	fy = 1.0f - (2.0f * y / h);
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

#define MAX_VERTICES 1000

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> random(0.3f, 0.7f);
std::uniform_int_distribution<int> randomInt(0, 1);

class Shape
{
public:
    bool ShapeState;
    bool IsMoving;
    std::vector<glm::vec3> vertices;
    std::vector<float> colors;
    std::vector<glm::vec3> normal;
    std::vector<unsigned int> faces;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Shape() : ShapeState(true), position(0),
        rotation(glm::vec3(30.0f, -30.0f, 0.0f)),
        scale(1.0f, 1.0f, 1.0f),
        IsMoving(false)
    {
        setRandomColor();
    }

    void setRandomColor()
    {
        colors.clear();
        for (int i = 0; i < 3; i++)
        {
            colors.push_back(random(gen));
        }
    }

    void loadFromObjFile(const char* filename)
    {
        FILE* file;
        char line[1000];
        float x, y, z;
        unsigned int v1, v2, v3;        // vertex indices
        unsigned int n1, n2, n3;        // normal indices
        unsigned int t1, t2, t3;        // texture indices

        fopen_s(&file, filename, "r");
        if (!file) {
            perror("���� ���� ����");
            return;
        }

        // ���� ������ �ʱ�ȭ
        vertices.clear();
        faces.clear();
        normal.clear();

        // ���� �Ľ�
        while (fgets(line, sizeof(line), file)) {
            if (line[0] == '#' || line[0] == 'g' || strlen(line) == 0 || line[0] == '\n')
                continue;

            if (line[0] == 'v' && line[1] == ' ') {
                // ���ؽ� �Ľ�
                if (sscanf_s(line + 2, "%f %f %f", &x, &y, &z) == 3) {
                    vertices.push_back(glm::vec3(x, y, z));
                }
            }

            else if (line[0] == 'f' && line[1] == ' ') {
                // �� ���� �Ľ�
                if (strstr(line, "//")) {
                    // v//n ���� (���ؽ��� ��� �ε����� �Բ� ���� ���)
                    if (sscanf_s(line + 2, "%u//%u %u//%u %u//%u", &v1, &n1, &v2, &n2, &v3, &n3) == 6) {
                        faces.push_back(v1 - 1);
                        faces.push_back(v2 - 1);
                        faces.push_back(v3 - 1);
                    }
                }
                else if (strstr(line, "/")) {
                    // v/t/n ���� (���ؽ�, �ؽ�ó, ��� �ε����� �Բ� ���� ���)
                    if (sscanf_s(line + 2, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3) == 9) {
                        faces.push_back(v1 - 1);
                        faces.push_back(v2 - 1);
                        faces.push_back(v3 - 1);
                    }
                }
                else if (sscanf_s(line + 2, "%u %u %u", &v1, &v2, &v3) == 3) {
                    // v ���� (���ؽ� �ε����� �ִ� ���)
                    faces.push_back(v1 - 1);
                    faces.push_back(v2 - 1);
                    faces.push_back(v3 - 1);
                }
            }
        }
        if (normal.empty()) { // ��� �����Ͱ� ���� ��� ���
            for (size_t i = 0; i < faces.size(); i += 3) {
                glm::vec3 v0 = vertices[faces[i]];
                glm::vec3 v1 = vertices[faces[i + 1]];
                glm::vec3 v2 = vertices[faces[i + 2]];
                glm::vec3 computedNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

                // ��� ������ ������ ������ �Ҵ�
                normal.push_back(computedNormal);
                normal.push_back(computedNormal);
                normal.push_back(computedNormal);
            }
        }
        setRandomColor();
        fclose(file);
    }

};

#define MAX_ROAD 10

Shape road[MAX_ROAD];
Shape cg;
Shape cube;

void UpdateBuffer(const Shape& shape);

bool a = 0;
int stage = 0;

void random_road(Shape road[MAX_ROAD]) {
    float z_positions[] = { -4.0f, 0.0f, 4.0f };
    int previousIndex = -1;
    float extraXOffset = 0.0f;

    for (int i = 1; i < MAX_ROAD; i++) {

        int randomIndex;
        do {
            randomIndex = rand() % 3;
        } while ((previousIndex != -1 &&
            ((z_positions[previousIndex] == -4.0f && z_positions[randomIndex] == 4.0f) ||
                (z_positions[previousIndex] == 4.0f && z_positions[randomIndex] == -4.0f))));

        if (randomIndex == previousIndex) {
            extraXOffset += 4.0f;
        }
        else {
            extraXOffset = 0.0f;
        }

        road[i].position = glm::vec3(16.0f * i + extraXOffset, -1.0f, z_positions[randomIndex]);
        previousIndex = randomIndex;
    }
}

void main(int argc, char** argv)
{
    srand(static_cast<unsigned int>(time(0)));
    if (!a) {
        a = 1;
        cg.loadFromObjFile("ball.obj");
        cg.ShapeState = 1;
        cg.scale = glm::vec3(0.2f, 0.2f, 0.2f);

        for (int i = 0; i < MAX_ROAD; i++) {
            road[i].loadFromObjFile("untitled.obj");
            road[i].ShapeState = 1;
        }
        random_road(road);
        road[0].position = glm::vec3(0.0f, -1.0f, 0.0f);
        cube.loadFromObjFile("cube.obj");
        if (stage > 0) {
            cube.ShapeState = 1;
            // ť�� Z�� ��ġ�� -4 ~ 4 ���� ���� ������ ����
            float randomZ = -4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (4.0f - (-4.0f))));
            cube.position = glm::vec3(5.0f, 0.0f, randomZ); // ������ Z �� ����
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Dol");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

    glEnable(GL_DEPTH_TEST);
    make_shaderProgram();
    InitBuffer();
    glutTimerFunc(16, TimerFunction, 1);
    glutDisplayFunc(display);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}