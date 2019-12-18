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
#include "shader.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include <glm\gtc\type_ptr.hpp>
#include <time.h>
#include <windows.h>

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

vec3 herbivoreArray[] = { 
	vec3(-10.0f, 0.0f,  9.0f),
	vec3(-10.0f, 0.0f,  7.0f),
	vec3(-10.0f, 0.0f,  5.0f),
	vec3(-10.0f, 0.0f,  3.0f),
	vec3(-10.0f, 0.0f,  1.0f),
	vec3(-10.0f, 0.0f, -1.0f),
	vec3(-10.0f, 0.0f, -3.0f),
	vec3(-10.0f, 0.0f, -5.0f),
	vec3(-10.0f, 0.0f, -7.0f),
	vec3(-10.0f, 0.0f, -9.0f)
};

vec3 carnivoreArray[] = {
	vec3(10.0f, 0.0f,  9.0f),
	vec3(10.0f, 0.0f,  7.0f),
	vec3(10.0f, 0.0f,  5.0f),
	vec3(10.0f, 0.0f,  3.0f),
	vec3(10.0f, 0.0f,  1.0f),
	vec3(10.0f, 0.0f, -1.0f),
	vec3(10.0f, 0.0f, -3.0f),
	vec3(10.0f, 0.0f, -5.0f),
	vec3(10.0f, 0.0f, -7.0f),
	vec3(10.0f, 0.0f, -9.0f)
};

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

	if (number == 0 || number > 2) {	
		cout << "Please load either 1 or 2 models model" << endl;
		getInput();
	}

	for (int i = 0; i < number; i++)
	{
		cout << "Please enter a valid .obj file name, either creeper.obj or boat.obj.\n>";
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

void wait(int seconds)
{
	clock_t endwait;
	endwait = clock() + 1 * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

void moveRandomly() {		
	
	srand(time(NULL));

	for (int i = 0; i < MatrixArray.size(); i++)
	{		
			int randomAxisValue = rand() % 2;		

			if (randomAxisValue == 0) {
				// X Value
				int randomMovementvalue = rand() % 2;
				//	cout << randomMovementvalue;		

				if (randomMovementvalue == 0) {
					// +1 on the X value			
					if (MatrixArray[i].ModelMatrix[3].x >= 10) {
						cout << "x == 10 " << endl;
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(-0.01f, 0.0f, 0.0f));
					}
					else {
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.01f, 0.0f, 0.0f));
					}
					//	cout << "+1 on the X value" << endl;
				}

				if (randomMovementvalue == 1) {
					// -1 on the X value
					if (MatrixArray[i].ModelMatrix[3].x <= -10) {
						cout << "x == -10 " << endl;
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.01f, 0.0f, 0.0f));
					}
					else {
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(-0.01f, 0.0f, 0.0f));
					}
					//	cout << "-1 on the X value" << endl;
				}
			}

			if (randomAxisValue == 1) {
				// Z Value
				int randomMovementvalue = rand() % 2;				

				if (randomMovementvalue == 0) {
					// +1 on the Z value
					if (MatrixArray[i].ModelMatrix[3].z >= 10) {
						cout << "z == 10 " << endl;
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, -0.01f));
					}
					else {
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, 0.01f));
					}
				}

				if (randomMovementvalue == 1) {
					// -1 on the Z value
					if (MatrixArray[i].ModelMatrix[3].z <= -10) {
						cout << "z == -10 " << endl;
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, 0.01f));
					}
					else {
						MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, -0.01f));
					}
					//cout << "-1 on the Z value" << endl;
				}
			}		
	}
};

void movementControls(GLFWwindow* window, GLuint &VertexArrayID, GLuint creeperUvbuffer, vector<vec2> creeperUvs,vector<vec3> creeperVertices, GLuint boatUvbuffer,vector<vec2> boatUvs,vector<vec3> boatVertices, GLuint TextureID, GLuint MatrixID)
{
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
		glDeleteBuffers(1, &creeperUvbuffer);
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		// Apply Texture Coords
		//GLuint uvbuffer;
		glGenBuffers(1, &creeperUvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, creeperUvbuffer);
		glBufferData(GL_ARRAY_BUFFER, creeperUvs.size() * sizeof(glm::vec2), &creeperUvs[0], GL_STATIC_DRAW);
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
		MatrixArray[0].ModelMatrix = translate(MatrixArray[0].ModelMatrix, glm::vec3(0.5f, 0.0f, 0.0f));		
	}	
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		MatrixArray[1].ModelMatrix = translate(MatrixArray[1].ModelMatrix, glm::vec3(0.5f, 0.1f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		MatrixArray[2].ModelMatrix = translate(MatrixArray[2].ModelMatrix, glm::vec3(-0.5f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		MatrixArray[3].ModelMatrix = translate(MatrixArray[3].ModelMatrix, glm::vec3(-0.6f, 0.1f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {	
		moveRandomly();
	}
}

void draw(GLuint MatrixID, GLuint creeperVertexbuffer, GLuint creeperUvbuffer,vector<vec3> creeperVertices, GLuint boatVertexbuffer, GLuint boatUvbuffer, vector<vec3> boatVertices, unsigned int pngTexture)
{
	mat4 ViewMatrix = getViewMatrix();
	mat4 ProjectionMatrix = getProjectionMatrix();

	int size = myList.size();
	int creeperIndex = 0;
	int boatIndex = 0;
	int index = 0;

	string fileValue;

	index = 0;

	for (std::vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

		fileValue = *i;

		if (fileValue == "creeper.obj") {

			// 1rst attribute buffer : creeperVertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, creeperVertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, creeperUvbuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pngTexture);
			// Set our "myTextureSampler" sampler to user Texture Unit 0
			glUniform1i(TextureID, 0);

			mat4 MVP;
			MVP = ProjectionMatrix * ViewMatrix * MatrixArray[index].ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, creeperVertices.size());
		}
		if (fileValue == "boat.obj") {

			// 1rst attribute buffer : boatVertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, boatVertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// 2nd attribute buffer : boatUVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, boatUvbuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pngTexture);
			// Set our "myTextureSampler" sampler to user Texture Unit 0
			glUniform1i(TextureID, 0);

			mat4 MVP;			
			MVP = ProjectionMatrix * ViewMatrix * MatrixArray[index].ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, boatVertices.size());
		}

		index++;
	};
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

	std::vector< glm::vec3 > creeperVertices;
	std::vector< glm::vec2 > creeperUvs;
	std::vector< glm::vec3 > creeperNormals;

	std::vector< glm::vec3 > boatVertices;
	std::vector< glm::vec2 > boatUvs;
	std::vector< glm::vec3 > boatNormals;

	string fileValue;

	int creeperIndex = 0;
	int boatIndex = 0;
	int index = 0;

	for (std::vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

		cout << *i << ' ' << endl;;
		fileValue = *i;

		if (fileValue == "creeper.obj") {

			if (creeperIndex == 0) {
				loadOBJ(fileValue.c_str(), creeperVertices, creeperUvs, creeperNormals);
			}
			Matrixes tempMatrix;
			tempMatrix.name = fileValue;
			tempMatrix.ModelMatrix = mat4(1.0);

			tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, herbivoreArray[creeperIndex]);

			MatrixArray.push_back(tempMatrix);
			creeperIndex++;
		}
		else if (fileValue == "boat.obj") {
			if (boatIndex == 0) {
				loadOBJ("creeper.obj", boatVertices, boatUvs, boatNormals);
			}
			Matrixes tempMatrix;
			tempMatrix.name = fileValue;
			tempMatrix.ModelMatrix = mat4(1.0);					

			tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, carnivoreArray[boatIndex]);

			MatrixArray.push_back(tempMatrix);
			boatIndex++;
		}
		else{
			cout << fileValue << " does not exist" << endl;
		}

		index++;
	};		

	////////////////////////////////////////////////////////////////// CREEPER
	GLuint creeperVertexbuffer;
	glGenBuffers(1, &creeperVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, creeperVertexbuffer);
	if (creeperVertices.size()) {
		glBufferData(GL_ARRAY_BUFFER, creeperVertices.size() * sizeof(glm::vec3), &creeperVertices[0], GL_STATIC_DRAW);
	}

	GLuint creeperUvbuffer;
	glGenBuffers(1, &creeperUvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, creeperUvbuffer);
	if (creeperUvs.size()) {
		glBufferData(GL_ARRAY_BUFFER, creeperUvs.size() * sizeof(glm::vec2), &creeperUvs[0], GL_STATIC_DRAW);
	}
	////////////////////////////////////////////////////////////////// CREEPER

	////////////////////////////////////////////////////////////////// BOAT
	GLuint boatVertexbuffer;
	glGenBuffers(1, &boatVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, boatVertexbuffer);
	if (boatVertices.size()) {
		glBufferData(GL_ARRAY_BUFFER, boatVertices.size() * sizeof(glm::vec3), &boatVertices[0], GL_STATIC_DRAW);
	}
	GLuint boatUvbuffer;
	glGenBuffers(1, &boatUvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, boatUvbuffer);
	if (boatUvs.size()) {
		glBufferData(GL_ARRAY_BUFFER, boatUvs.size() * sizeof(glm::vec2), &boatUvs[0], GL_STATIC_DRAW);
	}
	////////////////////////////////////////////////////////////////// BOAT

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

		computeMatricesFromInputs();
		movementControls(window, VertexArrayID, creeperUvbuffer, creeperUvs, creeperVertices, boatUvbuffer, boatUvs, boatVertices, TextureID, MatrixID);
		draw(MatrixID, creeperVertexbuffer, creeperUvbuffer, creeperVertices, boatVertexbuffer, boatUvbuffer, boatVertices, pngTexture);					
		//moveRandomly();
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	
	// Cleanup VBO and shader
	glDeleteBuffers(1, &creeperVertexbuffer);
	glDeleteBuffers(1, &creeperUvbuffer);
	glDeleteBuffers(1, &boatVertexbuffer);
	glDeleteBuffers(1, &boatUvbuffer);
	glDeleteProgram(shader);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	return 0;
}