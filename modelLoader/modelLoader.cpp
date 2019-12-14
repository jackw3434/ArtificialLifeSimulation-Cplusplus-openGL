#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loadVBO.hpp"
#include "shader.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include <glm\gtc\type_ptr.hpp>

using namespace glm;
using namespace std;

GLFWwindow* window;
GLuint TextureID;
std::vector<std::string> myList;

double creeperPositions[]
{
	0.0,
	2.0,
	4.0,
	6.0,
	8.0,
	10.0,
	12.0,
	14.0,
	16.0,
	18.0,
};

double boatPositions[]
{
	100.0,
	250.0,
	400.0,
	550.0,
	700.0,
	850.0,
	1000.0,
	1150.0,
	1300.0,
	1450.0,
};

class Matrixes {
public:
	string name;
	mat4 ModelMatrix;
};
float xValue = 1.0;
float xValue1 = 1.0;

std::vector<Matrixes> MatrixArray;

void init(void) {

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}
	else {
		fprintf(stderr, "initialized GLFW\n");
	}

	glewExperimental = GL_TRUE;
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 	

	window = glfwCreateWindow(1024, 768, "Model Loader", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //Dark Blue Background

	glfwPollEvents(); // Set mouse to centre of screen
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return;
	}
};

void getInput(void) {

	int number;
	string fileToLoad = "";
	string numberofFilesToOpen = "";

	cout << "Please enter how many different .obj files you wish to load.\n>";
	cin >> number;
	cout << "You entered: " << number << " files to load." << endl;

	for (int i = 0; i < number; i++)
	{
		cout << "Please enter the " << i << " valid .obj file name.\n>";
		cin >> fileToLoad;
		cout << "You entered: " << fileToLoad << i << endl;

		cout << "Please enter how many " << fileToLoad << "'s you wish to open.\n>";
		cin >> numberofFilesToOpen;
		cout << "You entered: " << numberofFilesToOpen << endl;

		for (int n = 0; n < stoi(numberofFilesToOpen); n++)
		{
			myList.push_back(fileToLoad.c_str());
		}
	};
}

void movementControls(GLFWwindow* window, GLuint VertexArrayID, GLuint uvbuffer, vector<vec2> uvs, GLuint TextureID) {

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		// draw in wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		// draw in wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		//Clear The Scene
		glDeleteVertexArrays(1, &VertexArrayID);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		//Reload the Scene
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		// Remove Texture Coords
		glDeleteBuffers(1, &uvbuffer);
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		// Apply Texture Coords
		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		// Removed Textures
		glDeleteTextures(1, &TextureID);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		unsigned int pngTexture;
		glGenTextures(1, &pngTexture);
		glBindTexture(GL_TEXTURE_2D, pngTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load("whitePaper.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

		unsigned int pngTexture;
		glGenTextures(1, &pngTexture);
		glBindTexture(GL_TEXTURE_2D, pngTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load("Texture.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		MatrixArray[1].ModelMatrix = translate(MatrixArray[1].ModelMatrix, glm::vec3(0.5f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		MatrixArray[1].ModelMatrix = translate(MatrixArray[1].ModelMatrix, glm::vec3(-0.5f, 0.0f, 0.0f));
	}
}

int main()
{
	getInput();
	init();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint shader = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	glUseProgram(shader);

	GLuint MatrixID = glGetUniformLocation(shader, "MVP");
	GLuint TextureID = glGetUniformLocation(shader, "myTextureSampler");

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	string fileValue;

	int creeperIndex = 0;
	int boatIndex = 0;
	int index = 0;

	for (std::vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

		std::cout << *i << ' ' << endl;;
		fileValue = *i;

		if (fileValue == "creeper.obj") {
			loadOBJ(fileValue.c_str(), vertices, uvs, normals, creeperPositions[creeperIndex]);
			
			Matrixes tempMatrix;
			tempMatrix.name = fileValue;
			tempMatrix.ModelMatrix = mat4(1.0);

			MatrixArray.push_back(tempMatrix);
			creeperIndex++;
		}
		if (fileValue == "boat.obj") {
			loadOBJ(fileValue.c_str(), vertices, uvs, normals, boatPositions[boatIndex]);

			Matrixes tempMatrix;
			tempMatrix.name = fileValue;
			tempMatrix.ModelMatrix = mat4(1.0);

			MatrixArray.push_back(tempMatrix);
			boatIndex++;
		}

		index++;
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	unsigned int pngTexture;
	glGenTextures(1, &pngTexture);
	glBindTexture(GL_TEXTURE_2D, pngTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Texture.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();

		movementControls(window, VertexArrayID, uvbuffer, uvs, TextureID);		

		mat4 ViewMatrix = getViewMatrix();
		mat4 ProjectionMatrix = getProjectionMatrix();
		mat4 MVP;

		int size = myList.size();

		for (int i = 0; i < size -1; i++)
		{
			MVP = ProjectionMatrix * ViewMatrix * MatrixArray[i].ModelMatrix;

			glUniformMatrix4fv(MatrixID, i, GL_FALSE, &MVP[0][0]);
		}		
	
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pngTexture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		////////////////////////////////////////////////////////////////////////////////////////////		

		mat4 ModelMatrix2 = glm::mat4(1.0);	
		ModelMatrix2 = translate(ModelMatrix2, vec3(xValue, 2.0f, 0.0f));
		mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;

		// Send our transformation to the currently bound shader
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);

		// THE KEY TO ALL SUCCESS LIES BENEATH
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {		
			xValue += 0.5;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			xValue -= 0.5;
		}

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : uvs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pngTexture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// Draw the triangles
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());


		////////////////////////////////////////////////////////////////////////////////////////////		

		mat4 ModelMatrix3 = glm::mat4(1.0);
		ModelMatrix3 = translate(ModelMatrix3, vec3(xValue1, 2.0f, 1.0f));
		mat4 MVP3 = ProjectionMatrix * ViewMatrix * ModelMatrix3;

		// Send our transformation to the currently bound shader
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);

		// THE KEY TO ALL SUCCESS LIES BENEATH
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			xValue1 += 0.5;
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			xValue1 -= 0.5;
		}

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : uvs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pngTexture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// Draw the triangles
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(shader);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	return 0;
}