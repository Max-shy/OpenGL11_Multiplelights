#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"shader_s.h"
#include "stb_image.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);//��������
void mouse_callback(GLFWwindow* window, double xpos, double ypos);//�������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//��������
unsigned int loadTexture(const char* path);//��������

const unsigned int  SCR_WIDTH = 800; 
const unsigned int SCR_HEIGHT = 600;

//͸���Ȳ���
float mixValue = 0.2f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//֡��
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

//��Դ���ꡢ��Դ��ɫ
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);



int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//������Ȼ���
	glEnable(GL_DEPTH_TEST);

	//��ɫ��
	Shader lightshader("Multiplelights.vs", "Multiplelights.fs");
	Shader lightCube("lightCube.vs", "lightCube.fs");
	//�����嶥��
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//ʮ�����ӵ�λ��
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//���Դ����
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	//����VAO��VBO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//����VAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//��VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�������嶥������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  8* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//����
	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	// shader configuration
	// --------------------
	lightshader.use();
	lightshader.setInt("material.diffuse", 0);
	lightshader.setInt("material.specular", 1);


	while (!glfwWindowShouldClose(window)) {
		
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�������

		//������Ⱦ���� 
		lightshader.use();
		lightshader.setVec3("viewPos", camera.Position);//���������λ��
		lightshader.setFloat("material.shininess", 32.0f);// ��������
		
		//��Դ����
		lightshader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightshader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightshader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightshader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightshader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightshader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightshader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightshader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightshader.setFloat("pointLights[0].constant", 1.0f);
		lightshader.setFloat("pointLights[0].linear", 0.09f);
		lightshader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		lightshader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightshader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightshader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightshader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightshader.setFloat("pointLights[1].constant", 1.0f);
		lightshader.setFloat("pointLights[1].linear", 0.09f);
		lightshader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		lightshader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightshader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightshader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightshader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightshader.setFloat("pointLights[2].constant", 1.0f);
		lightshader.setFloat("pointLights[2].linear", 0.09f);
		lightshader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		lightshader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightshader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightshader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightshader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightshader.setFloat("pointLights[3].constant", 1.0f);
		lightshader.setFloat("pointLights[3].linear", 0.09f);
		lightshader.setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		lightshader.setVec3("spotLight.position", camera.Position);
		lightshader.setVec3("spotLight.direction", camera.Front);
		lightshader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightshader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightshader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightshader.setFloat("spotLight.constant", 1.0f);
		lightshader.setFloat("spotLight.linear", 0.09f);
		lightshader.setFloat("spotLight.quadratic", 0.032f);
		lightshader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightshader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		

		//ͶӰ����+��ͼ����+model����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		lightshader.setMat4("projection", projection);
		lightshader.setMat4("view", view);
		lightshader.setMat4("model", model);
		lightshader.setFloat("mixValue", mixValue);
		
		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);


		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightshader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		 
		//������Դ����
		lightCube.use();
		lightCube.setVec3("lightColor", lightColor);
		lightCube.setMat4("projection", projection);
		lightCube.setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);//����Դ�ƶ���ָ��λ��
		//model = glm::scale(model, glm::vec3(0.2f));//����Դ���ŵ�ָ����С
		//lightCube.setMat4("model", model);

		glBindVertexArray(lightVAO);

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightCube.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();  
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow*window,int width,int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {//��������
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f;
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f;
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}