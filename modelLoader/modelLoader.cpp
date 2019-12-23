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
#include <ctime>
 
using namespace glm;
using namespace std;

const int NUM_SECONDS = 1;

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
	vec3(-5.0f, 0.0f,  9.0f),
	vec3(-5.0f, 0.0f,  7.0f),
	vec3(-5.0f, 0.0f,  5.0f),
	vec3(-5.0f, 0.0f,  3.0f),
	vec3(-5.0f, 0.0f,  1.0f),
	vec3(-5.0f, 0.0f, -1.0f),
	vec3(-5.0f, 0.0f, -3.0f),
	vec3(-5.0f, 0.0f, -5.0f),
	vec3(-5.0f, 0.0f, -7.0f),
	vec3(-5.0f, 0.0f, -9.0f)
};

vec3 carnivoreArray[] = {
	vec3(5.0f, 0.0f,  9.0f),
	vec3(5.0f, 0.0f,  7.0f),
	vec3(5.0f, 0.0f,  5.0f),
	vec3(5.0f, 0.0f,  3.0f),
	vec3(5.0f, 0.0f,  1.0f),
	vec3(5.0f, 0.0f, -1.0f),
	vec3(5.0f, 0.0f, -3.0f),
	vec3(5.0f, 0.0f, -5.0f),
	vec3(5.0f, 0.0f, -7.0f),
	vec3(5.0f, 0.0f, -9.0f)
};

std::vector<Matrixes> MatrixArray;
bool canMoveAt0 = true;
bool canMoveAt1 = true;
bool canMoveAt2 = true;
bool canMoveAt3 = true;
bool canMoveAt4 = true;
bool canMoveAt5 = true;
bool canMoveAt6 = true;
bool canMoveAt7 = true;
bool canMoveAt8 = true;
bool canMoveAt9 = true;
bool canMoveAt10 = true;
bool canMoveAt11 = true;
bool canMoveAt12 = true;
bool canMoveAt13 = true;
bool canMoveAt14 = true;
bool canMoveAt15 = true;
bool canMoveAt16 = true;
bool canMoveAt17 = true;
bool canMoveAt18 = true;
bool canMoveAt19 = true;
bool canMoveAt20 = true;
bool canMoveAt21 = true;
bool canMoveAt22 = true;
bool canMoveAt23 = true;
bool canMoveAt24 = true;

int creeperCount = 5;
int boatCount = 5;

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

void moveRandomly() {	
	
	float moveSpeed = 1.0f;
	srand(time(NULL));

	for (int i = 0; i < MatrixArray.size(); i++)
	{
		//for (int j = 0; j < MatrixArray.size(); j++)
		//{

			//if (MatrixArray[i].ModelMatrix[3].x != MatrixArray[j].ModelMatrix[3].x && MatrixArray[i].ModelMatrix[3].z != MatrixArray[j].ModelMatrix[3].z) {


				int randomAxisValue = rand() % 2;

				if (randomAxisValue == 0) {
					// X Value
					int randomMovementvalue = rand() % 2;

					if (randomMovementvalue == 0) {
						// +1 on the X value			
						if (MatrixArray[i].ModelMatrix[3].x >= 5) {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(-moveSpeed, 0.0f, 0.0f));
						}
						else {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(moveSpeed, 0.0f, 0.0f));
						}
					}

					if (randomMovementvalue == 1) {
						// -1 on the X value
						if (MatrixArray[i].ModelMatrix[3].x <= -5) {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(moveSpeed, 0.0f, 0.0f));
						}
						else {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(-moveSpeed, 0.0f, 0.0f));
						}
					}
				}

				if (randomAxisValue == 1) {
					// Z Value
					int randomMovementvalue = rand() % 2;

					if (randomMovementvalue == 0) {
						// +1 on the Z value
						if (MatrixArray[i].ModelMatrix[3].z >= 5) {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, -moveSpeed));
						}
						else {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, moveSpeed));
						}
					}

					if (randomMovementvalue == 1) {
						// -1 on the Z value
						if (MatrixArray[i].ModelMatrix[3].z <= -5) {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, moveSpeed));
						}
						else {
							MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, -moveSpeed));
						}
					}
				}

			//}
		//}


	}
	cout << "creeperCount" << creeperCount << endl;
	cout << "boatCount" << boatCount << endl;
	//cout << "MatrixArray[3].ModelMatrix[3].z" << MatrixArray[3].ModelMatrix[3].z << endl;
	//cout << "MatrixArray[3].ModelMatrix[3].x" << MatrixArray[3].ModelMatrix[3].x << endl;
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
		

	}
}

void draw(GLuint MatrixID, GLuint creeperVertexbuffer, GLuint creeperUvbuffer, vector<vec3> creeperVertices, GLuint boatVertexbuffer, GLuint boatUvbuffer, vector<vec3> boatVertices, unsigned int pngTextureCreeper, unsigned int pngTextureBoat)
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
			glBindTexture(GL_TEXTURE_2D, pngTextureCreeper);
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
			glBindTexture(GL_TEXTURE_2D, pngTextureBoat);
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

void moveEachSecond() {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();
	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();

	int deltaTime = float(currentTime - lastTime);

	//cout << deltaTime << endl;

	if (floor(deltaTime) == 1 && canMoveAt1 == true) {
		moveRandomly();
		canMoveAt1 = false;
	}
	if (floor(deltaTime) == 2 && canMoveAt2 == true) {
		moveRandomly();
		canMoveAt2 = false;
	}
	if (floor(deltaTime) == 3 && canMoveAt3 == true) {
		moveRandomly();
		canMoveAt3 = false;
	}
	if (floor(deltaTime) == 4 && canMoveAt4 == true) {
		moveRandomly();
		canMoveAt4 = false;
	}
	if (floor(deltaTime) == 5 && canMoveAt5 == true) {
		moveRandomly();
		canMoveAt5 = false;
	}
	if (floor(deltaTime) == 6 && canMoveAt6 == true) {
		moveRandomly();
		canMoveAt6 = false;
	}
	if (floor(deltaTime) == 7 && canMoveAt7 == true) {
		moveRandomly();
		canMoveAt7 = false;
	}
	if (floor(deltaTime) == 8 && canMoveAt8 == true) {
		moveRandomly();
		canMoveAt8 = false;
	}
	if (floor(deltaTime) == 9 && canMoveAt9 == true) {
		moveRandomly();
		canMoveAt9 = false;
	}
	if (floor(deltaTime) == 10 && canMoveAt10 == true) {
		moveRandomly();
		canMoveAt10 = false;
	}
	if (floor(deltaTime) == 11 && canMoveAt11 == true) {
		moveRandomly();
		canMoveAt11 = false;
	}
	if (floor(deltaTime) == 12 && canMoveAt12 == true) {
		moveRandomly();
		canMoveAt12 = false;
	}
	if (floor(deltaTime) == 13 && canMoveAt13 == true) {
		moveRandomly();
		canMoveAt13 = false;
	}
	if (floor(deltaTime) == 14 && canMoveAt14 == true) {
		moveRandomly();
		canMoveAt14 = false;
	}
	if (floor(deltaTime) == 15 && canMoveAt15 == true) {
		moveRandomly();
		canMoveAt15 = false;
	}
	if (floor(deltaTime) == 16 && canMoveAt16 == true) {
		moveRandomly();
		canMoveAt16 = false;
	}
	if (floor(deltaTime) == 17 && canMoveAt17 == true) {
		moveRandomly();
		canMoveAt17 = false;
	}
	if (floor(deltaTime) == 18 && canMoveAt18 == true) {
		moveRandomly();
		canMoveAt18 = false;
	}
	if (floor(deltaTime) == 19 && canMoveAt19 == true) {
		moveRandomly();
		canMoveAt19 = false;
	}
	if (floor(deltaTime) == 20 && canMoveAt20 == true) {
		moveRandomly();
		canMoveAt20 = false;
	}
	if (floor(deltaTime) == 21 && canMoveAt21 == true) {
		moveRandomly();
		canMoveAt21 = false;
	}
	if (floor(deltaTime) == 22 && canMoveAt22 == true) {
		moveRandomly();
		canMoveAt22 = false;
	}
	if (floor(deltaTime) == 23 && canMoveAt23 == true) {
		moveRandomly();
		canMoveAt23 = false;
	}
	if (floor(deltaTime) == 24 && canMoveAt24 == true) {
		moveRandomly();
		canMoveAt24 = false;

		// get list of remaining objects
		// re draw with initial positions again
		// continue		
		
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

		//cout << *i << ' ' << endl;;
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

	
	unsigned int pngTextureCreeper;
	glGenTextures(1, &pngTextureCreeper);
	glBindTexture(GL_TEXTURE_2D, pngTextureCreeper);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthCreeper, heightCreeper, nrChannelsCreeper;
	unsigned char* dataCreeper = stbi_load("Texture.png", &widthCreeper, &heightCreeper, &nrChannelsCreeper, 0);
	if (dataCreeper)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthCreeper, heightCreeper, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataCreeper);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(dataCreeper);

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

	unsigned int pngTextureBoat;
	glGenTextures(1, &pngTextureBoat);
	glBindTexture(GL_TEXTURE_2D, pngTextureBoat);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthBoat, heightBoat, nrChannelsBoat;
	unsigned char* dataBoat = stbi_load("whitePaper.png", &widthBoat, &heightBoat, &nrChannelsBoat, 0);
	if (dataBoat)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthBoat, heightBoat, 0, GL_RGB, GL_UNSIGNED_BYTE, dataBoat);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(dataBoat);

	do {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();
		movementControls(window, VertexArrayID, creeperUvbuffer, creeperUvs, creeperVertices, boatUvbuffer, boatUvs, boatVertices, TextureID, MatrixID);
		draw(MatrixID, creeperVertexbuffer, creeperUvbuffer, creeperVertices, boatVertexbuffer, boatUvbuffer, boatVertices, pngTextureCreeper, pngTextureBoat);
		moveEachSecond();

		for (size_t i = 0; i < MatrixArray.size(); i++)
		{

			for (size_t j = 0; j < MatrixArray.size(); j++)
			{
				if (i != j) {
					if (MatrixArray[i].ModelMatrix[3].x == MatrixArray[j].ModelMatrix[3].x && MatrixArray[i].ModelMatrix[3].z == MatrixArray[j].ModelMatrix[3].z) {
						if (MatrixArray[i].name != MatrixArray[j].name) {
						//	cout << myList.size() << endl;
						//	cout << MatrixArray.size() << endl;
							cout << MatrixArray[i].name + " hit " + MatrixArray[j].name << endl;
						//	cout << "collision" << endl;
						//	cout << "Creeper hit boat" << endl;
							MatrixArray.erase(MatrixArray.begin() + i);
							myList.erase(myList.begin() + i);
							creeperCount -= 1;						
							
							//cout << myList.size() << endl;
							cout <<" MatrixArray.size() " << MatrixArray.size() << endl;
						}
						//if (MatrixArray[i].name == "boat.obj" && MatrixArray[i].name == MatrixArray[j].name) {
							/*string fileValue = "boat.obj";
							myList.push_back(fileValue);
							Matrixes tempMatrix;
							tempMatrix.name = fileValue;
							tempMatrix.ModelMatrix = mat4(1.0);

							tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, carnivoreArray[boatIndex + 5]);

							MatrixArray.push_back(tempMatrix);	*/					
						//}
						else {
							
							//cout << MatrixArray[i].name + " hit " + MatrixArray[j].name << endl;
						}
					}
				}
			}			
		}

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