#include "head.h"

glm::vec3 lightPos = glm::vec3(0.0f, 15.0f, 0.0f);
glm::vec3 rotatin(30.0f, 90.0f, 0.0f);

GLfloat light_color[3] = { 1.0f, 1.0f, 1.0f };
float lightAngle = 0.0f; // ������ �ʱ� ȸ�� ���� (����)

bool moveForward = false;
bool moveBackward = false;
bool moveLeft = false;
bool moveRight = false;
float speed = 0.1;
float pi = 0.f;

void drawScene() {

	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaderProgramID);

	GLuint modelTransformLoc = glGetUniformLocation(shaderProgramID, "model");
	GLuint projectionTransformLoc = glGetUniformLocation(shaderProgramID, "projection");

	glDisable(GL_CULL_FACE);

	glm::mat4 pTransform = glm::mat4(1.0f);
	pTransform = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 50.0f);
	glUniformMatrix4fv(projectionTransformLoc, 1, GL_FALSE, &pTransform[0][0]);

	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor");

	glBindVertexArray(vao);

	/*glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);*/

	if (cube.ShapeState) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, cube.scale);        // ũ�� ����
		model = glm::rotate(model, glm::radians(rotatin.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotatin.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotatin.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, cube.position); // �̵� ���� ����
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, &model[0][0]);

		glUniform3f(objColorLocation, cube.colors[0], cube.colors[1], cube.colors[2]);
		UpdateBuffer(cube);

		glDrawElements(GL_TRIANGLES, cube.faces.size(), GL_UNSIGNED_INT, cube.faces.data());
	}
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cg.position); // �̵� ���� ����
	model = glm::scale(model, cg.scale);        // ũ�� ����
	model = glm::rotate(model, glm::radians(cg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(cg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(cg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, &model[0][0]);

	glUniform3f(objColorLocation, cg.colors[0], cg.colors[1], cg.colors[2]);
	UpdateBuffer(cg);

	glDrawElements(GL_TRIANGLES, cg.faces.size(), GL_UNSIGNED_INT, cg.faces.data());
	for (int i = 0; i < MAX_ROAD; i++) {
		if (road[i].ShapeState) {
			model = glm::mat4(1.0f);
			model = glm::scale(model, road[i].scale);
			model = glm::rotate(model, glm::radians(rotatin.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotatin.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotatin.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, road[i].position);
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, &model[0][0]);

			glUniform3f(objColorLocation, road[i].colors[0], road[i].colors[1], road[i].colors[2]);
			UpdateBuffer(road[i]);

			glDrawElements(GL_TRIANGLES, road[i].faces.size(), GL_UNSIGNED_INT, road[i].faces.data());
		}
	}

	unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	// ���� ��ġ ȸ���� rotatin ����
	glm::mat4 lightTransform = glm::mat4(1.0f);
	glm::vec4 rotatedLightPos = lightTransform * glm::vec4(lightPos, 1.0f);

	// ������ ��ġ�� ���̴��� ������Ʈ
	glUniform3f(lightPosLocation, rotatedLightPos.x, rotatedLightPos.y, rotatedLightPos.z);
	glUniform3f(lightColorLocation, light_color[0], light_color[1], light_color[2]);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a': case 'A':
		moveLeft = true;
		break;
	case 'd': case 'D':
		moveRight = true;
		break;
	case 'q': case 'Q':
		glutLeaveMainLoop();
		break;
	}
}

GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a': case 'A':
		moveLeft = false;
		break;
	case 'd': case 'D':
		moveRight = false;
		break;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(1);

	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	glUseProgram(shaderProgramID);
	unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos[0], lightPos[1], lightPos[2]);
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, light_color[0], light_color[1], light_color[2]);
}

void UpdateBuffer(const Shape& shape)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertices.size() * sizeof(glm::vec3), shape.vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, shape.normal.size() * sizeof(glm::vec3), shape.normal.data(), GL_DYNAMIC_DRAW);

	for (int i = 0; i < cg.vertices.size(); i++) {
		glm::vec3 vertex = cg.vertices[i];
		cg.normal[i] = glm::normalize(vertex);
	}
	for (int i = 0; i < MAX_ROAD; ++i) { // i ���� �ڸ��� �� ���� �Է�
		if (road[i].ShapeState) {
			for (int j = 0; j < road[i].vertices.size(); ++j) {
				glm::vec3 vertex = road[i].vertices[j];
				road[i].normal[j] = glm::normalize(vertex);
			}
		}
	}
}

//void InitTexture() {
//	BITMAP* bmp;
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	unsigned char* data = LoadDIBitmap("1.bmp", &bmp);
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
//}

bool IsBallOnRoad(const glm::vec3& ballPos, const glm::vec3& roadPos, const glm::vec3& roadScale) {
	// road�� x, z ��� ���
	float roadLeft = roadPos[2] - 2.0f;
	float roadRight = roadPos[2] + 2.0f;
	float roadFront = roadPos[0] - 10.0f;
	float roadBack = roadPos[0] + 10.0f;

	// ball�� road�� x, z ��� �ȿ� �ִ��� Ȯ��
	return (ballPos[0] >= roadLeft && ballPos[0] <= roadRight &&
		ballPos[2] >= roadFront && ballPos[2] <= roadBack);
}

bool IsCollision(const glm::vec3& ballPos, const glm::vec3& cubePos, const glm::vec3& cubeScale) {
	// cube�� x, y, z ��� ���
	float cubeLeft = cubePos[0] - cubeScale[0];
	float cubeRight = cubePos[0] + cubeScale[0];
	float cubeTop = cubePos[1] + cubeScale[1];
	float cubeBottom = cubePos[1] - cubeScale[1];
	float cubeFront = cubePos[2] - cubeScale[2];
	float cubeBack = cubePos[2] + cubeScale[2];

	// ball�� ��ġ�� cube�� ��� ���� �ִ��� Ȯ��
	return (ballPos[0] >= cubeLeft && ballPos[0] <= cubeRight &&
		ballPos[1] >= cubeBottom && ballPos[1] <= cubeTop &&
		ballPos[2] >= cubeFront && ballPos[2] <= cubeBack);
}


void TimerFunction(int value)
{
	if (stage >= 3)
		glutLeaveMainLoop();

	for (int i = 0; i < MAX_ROAD; i++) {
		road[i].position[0] -= speed;
	}
	cube.position[0] -= speed;
	cg.rotation[0] -= speed * 50.0f;      // z�� ���� ȸ��
	speed += 0.001;

	// ������ ó��
	for (int i = 0; i < MAX_ROAD; i++) {
		if (moveLeft) {
			road[i].position[2] += speed / 2;      // ���� �������� z�� �̵�
		}
		if (moveRight) {
			road[i].position[2] -= speed / 2;      // ���� �������� z�� �̵� (�ݴ�)
		}
	}
	if (moveLeft) {
		cube.position[2] += speed / 2;
		cameraDirection1[0] += 0.005f;
		cg.rotation[2] += speed * 50.0f;          // z�� ���� ȸ��
	}
	if (moveRight) {
		cube.position[2] -= speed / 2;
		cameraDirection1[0] -= 0.005f;
		cg.rotation[2] -= speed * 50.0f;          // z�� ���� ȸ��
	}

	pi += 0.008f;
	if (cube.IsMoving)
		cube.position[2] += sin(pi) * 0.1f;

	bool Onroad = false; // �ʱⰪ�� false�� ����

	// ���� �� road ���� �ִ��� ���� (x, z ��鿡��)
	for (int i = 0; i < MAX_ROAD; i++) {
		if (road[i].ShapeState) { // Ȱ��ȭ�� road�� �˻�
			if (IsBallOnRoad(cg.position, road[i].position, road[i].scale)) {
				Onroad = true; // �� �� �ϳ��� ���� ���� ������ true�� ����
				break;         // �ϳ��� �����ϸ� �� �̻� �˻����� ����
			}
		}
	}

	// ��� ����� ���� ������ ���� y���� ����
	if (!Onroad || road[0].position[1] > -1.0f) {
		for (int i = 0; i < MAX_ROAD; i++) {
			road[i].position[1] += 0.1f ; // ���� y�� ����
		}
	}

	if (road[0].position[0] <= -155) {
		stage += 1;
		//bGCr = stage * 0.1f, bGCg = stage * 0.1f, bGCb = stage * 0.1f;
		random_road(road);
		for (int i = 0; i < MAX_ROAD; i++) {
			road[i].position[1] = -1.0f;
		}
		road[0].position = glm::vec3(0.0f, -1.0f, 0.0f);
		if (stage > 0) {
			cube.ShapeState = 1;
		}
		cameraDirection1[0] = 0.0f;
		speed = 0.1f;
	}

	if (road[0].position[1] > 5.0f) {
		random_road(road);
		for (int i = 0; i < MAX_ROAD; i++) {
			road[i].position[1] = -1.0f;
		}
		road[0].position = glm::vec3(0.0f, -1.0f, 0.0f);
		if (stage > 0) {
			cube.ShapeState = 1;
		}
		cameraDirection1[0] = 0.0f;
		speed = 0.1f;
	}

	if (stage > 0) {
		if (cube.position[0] < -5) {
			float randomZ = -4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (4.0f - (-4.0f))));
			cube.ShapeState = 1;
			cube.position = glm::vec3(27.0f, 0.0f, randomZ); // ������ Z �� ����

			cube.IsMoving = randomInt(gen);
		}
	}

	if (cube.ShapeState && IsCollision(cg.position, cube.position, cube.scale)) {
		// ���� �ӵ��� �ݴ�� ����
		speed *= -0.25f;

		// ť�� ��Ȱ��ȭ
		cube.ShapeState = 0;
		std::cout << "Collision with cube! Cube is now deactivated." << std::endl;
	}

	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 1);
}
