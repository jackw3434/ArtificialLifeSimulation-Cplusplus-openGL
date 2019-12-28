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

class Matrixes {
public:
	string name;
	mat4 ModelMatrix;
	bool hasEaten = false;	
};

vector<string> myList;
vector<vec3> herbivoreArray;
vector<vec3> carnivoreArray;
vector<Matrixes> MatrixArray;

GLFWwindow* window;
GLuint TextureID;

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

bool dayFinished = false;

int herbivoreCount;
int carnivoreCount;
int days;
int currentDay;

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

	days = 0;
	currentDay = 0;
	herbivoreCount = 0;
	carnivoreCount = 0;

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
		cout << "Please enter a valid .obj file name, either herbivore or carnivore.\n>";
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

	cout << "Please enter how many days you would like the simulation to run.\n>";
	cin >> days;
	cout << "You entered: " << days << " days." << endl;
}

void moveRandomly() {	

	int edgeValue = MatrixArray.size() -1;

	float moveSpeed = 1.0f;
	srand(time(NULL));

	for (int i = 0; i < MatrixArray.size(); i++)
	{	

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
				if (MatrixArray[i].ModelMatrix[3].z >= MatrixArray[edgeValue].ModelMatrix[3].z) {
					MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, -moveSpeed));
				}
				else {
					MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, moveSpeed));
				}
			}

			if (randomMovementvalue == 1) {
				// -1 on the Z value
				if (MatrixArray[i].ModelMatrix[3].z <= MatrixArray[0].ModelMatrix[3].z) {
					MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, moveSpeed));
				}
				else {
					MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, glm::vec3(0.0f, 0.0f, -moveSpeed));
				}
			}
		}
	}
};

void movementControls(GLFWwindow* window, GLuint &VertexArrayID, GLuint herbivoreUvbuffer, vector<vec2> herbivoreUvs,vector<vec3> herbivoreVertices, GLuint carnivoreUvbuffer,vector<vec2> carnivoreUvs,vector<vec3> carnivoreVertices, GLuint TextureID, GLuint MatrixID)
{
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		// fill in wireframe
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
		glDeleteBuffers(1, &herbivoreUvbuffer);
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		// Apply Texture Coords
		glGenBuffers(1, &herbivoreUvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, herbivoreUvbuffer);
		glBufferData(GL_ARRAY_BUFFER, herbivoreUvs.size() * sizeof(glm::vec2), &herbivoreUvs[0], GL_STATIC_DRAW);
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
}

void draw(GLuint MatrixID, GLuint herbivoreVertexbuffer, GLuint herbivoreUvbuffer, vector<vec3> herbivoreVertices, GLuint carnivoreVertexbuffer, GLuint carnivoreUvbuffer, vector<vec3> carnivoreVertices, unsigned int pngTextureHerbivore, unsigned int pngTextureCarnivore)
{
	mat4 ViewMatrix = getViewMatrix();
	mat4 ProjectionMatrix = getProjectionMatrix();
	
	int index = 0;	

	for (std::vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

		string fileValue = *i;

		if (fileValue == "herbivore") {

			// 1rst attribute buffer : herbivoreVertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, herbivoreVertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, herbivoreUvbuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pngTextureHerbivore);
			// Set our "myTextureSampler" sampler to user Texture Unit 0
			glUniform1i(TextureID, 0);

			mat4 MVP;
			MVP = ProjectionMatrix * ViewMatrix * MatrixArray[index].ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, herbivoreVertices.size());
		}
		if (fileValue == "carnivore") {

			// 1rst attribute buffer : carnivoreVertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, carnivoreVertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// 2nd attribute buffer : carnivoreUVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, carnivoreUvbuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pngTextureCarnivore);
			// Set our "myTextureSampler" sampler to user Texture Unit 0
			glUniform1i(TextureID, 0);

			mat4 MVP;
			MVP = ProjectionMatrix * ViewMatrix * MatrixArray[index].ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, carnivoreVertices.size());
		}

		index++;
	};
}

void moveEachSecond() {
	// Compute time difference between current and last frame
	static double lastTime = glfwGetTime();		
	double currentTime = glfwGetTime();
	int deltaTime = float(currentTime - lastTime);	

	if (floor(deltaTime) == 1 && canMoveAt1 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;		
		moveRandomly();
		canMoveAt1 = false;
	}
	if (floor(deltaTime) == 2 && canMoveAt2 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt2 = false;
	}
	if (floor(deltaTime) == 3 && canMoveAt3 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt3 = false;
	}
	if (floor(deltaTime) == 4 && canMoveAt4 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt4 = false;
	}
	if (floor(deltaTime) == 5 && canMoveAt5 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt5 = false;
	}
	if (floor(deltaTime) == 6 && canMoveAt6 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt6 = false;
	}
	if (floor(deltaTime) == 7 && canMoveAt7 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt7 = false;
	}
	if (floor(deltaTime) == 8 && canMoveAt8 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt8 = false;
	}
	if (floor(deltaTime) == 9 && canMoveAt9 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt9 = false;
	}
	if (floor(deltaTime) == 10 && canMoveAt10 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt10 = false;
	}
	if (floor(deltaTime) == 11 && canMoveAt11 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt11 = false;
	}
	if (floor(deltaTime) == 12 && canMoveAt12 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt12 = false;
	}
	if (floor(deltaTime) == 13 && canMoveAt13 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt13 = false;
	}
	if (floor(deltaTime) == 14 && canMoveAt14 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt14 = false;
	}
	if (floor(deltaTime) == 15 && canMoveAt15 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt15 = false;
	}
	if (floor(deltaTime) == 16 && canMoveAt16 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt16 = false;
	}
	if (floor(deltaTime) == 17 && canMoveAt17 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt17 = false;
	}
	if (floor(deltaTime) == 18 && canMoveAt18 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt18 = false;
	}
	if (floor(deltaTime) == 19 && canMoveAt19 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt19 = false;
	}
	if (floor(deltaTime) == 20 && canMoveAt20 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt20 = false;
	}
	if (floor(deltaTime) == 21 && canMoveAt21 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt21 = false;
	}
	if (floor(deltaTime) == 22 && canMoveAt22 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt22 = false;
	}
	if (floor(deltaTime) == 23 && canMoveAt23 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();
		canMoveAt23 = false;
	}
	if (floor(deltaTime) >= 24 && canMoveAt24 == true) {
		cout << "Day: " << currentDay << " Time: " << deltaTime << " O'clock" << endl;
		moveRandomly();		

		int herbivoreIndex = 0;
		int carnivoreIndex = 0;

		if (MatrixArray.size() > 0) {
			for (int i = 0; i < MatrixArray.size();)
			{
				if (MatrixArray.size() > 0 && MatrixArray[i].hasEaten == false) {

					if (MatrixArray[i].name == "herbivore") {
						herbivoreCount--;
					}
					if (MatrixArray[i].name == "carnivore") {
						carnivoreCount--;
					}
					MatrixArray.erase(MatrixArray.begin() + i);
					myList.erase(myList.begin() + i);
				}
				else if (MatrixArray[i].hasEaten == true) {

					if (MatrixArray[i].name == "herbivore") {
						MatrixArray[i].ModelMatrix[3].x = herbivoreArray[herbivoreIndex].x;
						MatrixArray[i].ModelMatrix[3].z = herbivoreArray[herbivoreIndex].z;
						herbivoreIndex++;
					}
					if (MatrixArray[i].name == "carnivore") {
						MatrixArray[i].ModelMatrix[3].x = carnivoreArray[carnivoreIndex].x;
						MatrixArray[i].ModelMatrix[3].z = carnivoreArray[carnivoreIndex].z;
						carnivoreIndex++;
					}
					i++;
				}
			}
		}
		else {
			return;
		}
		canMoveAt0 = true;
		canMoveAt1 = true;
		canMoveAt2 = true;
		canMoveAt3 = true;
		canMoveAt4 = true;
		canMoveAt5 = true;
		canMoveAt6 = true;
		canMoveAt7 = true;
		canMoveAt8 = true;
		canMoveAt9 = true;
		canMoveAt10 = true;
		canMoveAt11 = true;
		canMoveAt12 = true;
		canMoveAt13 = true;
		canMoveAt14 = true;
		canMoveAt15 = true;
		canMoveAt16 = true;
		canMoveAt17 = true;
		canMoveAt18 = true;
		canMoveAt19 = true;
		canMoveAt20 = true;
		canMoveAt21 = true;
		canMoveAt22 = true;
		canMoveAt23 = true;
		canMoveAt24 = true;	

		cout << "day finished " << currentDay << endl;		
		cout << "herbivore count  " << herbivoreCount << endl;
		cout << "carnivore count  " << carnivoreCount << endl;
		glfwSetTime(0);
		currentDay++;		
	}	
}

void dayCycles() {

	for (int i = 0; i < days; i++)
	{
		if (currentDay == i) {			
			moveEachSecond();
		}
	}			
}

void collision() {
	
	for (size_t i = 0; i < MatrixArray.size(); i++)
	{

		for (size_t j = 0; j < MatrixArray.size(); j++)
		{
			if (i != j) {
				if (MatrixArray[i].ModelMatrix[3].x == MatrixArray[j].ModelMatrix[3].x && MatrixArray[i].ModelMatrix[3].z == MatrixArray[j].ModelMatrix[3].z) {				
					if (MatrixArray[i].name == "carnivore" && MatrixArray[i].name != MatrixArray[j].name) {

						// Has eaten will survive to the next day.
						cout << "i != j name == carnivore, carnivore hitting herbivore " << MatrixArray[i].name + " hit " + MatrixArray[j].name << endl;

						MatrixArray.erase(MatrixArray.begin() + j);
						myList.erase(myList.begin() + j);
						herbivoreCount--;

						cout << "herbivore count  " << herbivoreCount << endl;
						cout << "carnivore count  " << carnivoreCount << endl;

						MatrixArray[i].hasEaten = true;
				
					}
					if (MatrixArray[i].name == "herbivore" && MatrixArray[i].name != MatrixArray[j].name) {

						// Has eaten will survive to the next day.
						cout << "i != j name == herbivore, herbivore hitting carnivore " << MatrixArray[i].name + " hit " + MatrixArray[j].name << endl;

						MatrixArray.erase(MatrixArray.begin() + i);
						myList.erase(myList.begin() + i);
						herbivoreCount--;

						cout << "herbivore count  " << herbivoreCount << endl;
						cout << "carnivore count  " << carnivoreCount << endl;

						MatrixArray[j].hasEaten = true;



						/// Reproducing

						int size = (myList.size() / 2) - 1;

						float herbPosition = -size;
						float carnPosition = -size;
						carnPosition += 2;						

						carnivoreArray.push_back(vec3(5.0f, 0.0f, carnPosition));						

						string fileValue = "carnivore";
						myList.push_back(fileValue);

						Matrixes tempMatrix;
						tempMatrix.name = fileValue;
						tempMatrix.ModelMatrix = mat4(1.0);

						int carnoSize;

						for (size_t i = 0; i < MatrixArray.size(); i++)
						{
							if (MatrixArray[i].name == "carnivore") {
								carnoSize++;
							}
						}

						tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, carnivoreArray[carnoSize]);

						MatrixArray.push_back(tempMatrix);
						
						carnivoreCount++;						
					}
				}
			}
		}
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

	std::vector< glm::vec3 > herbivoreVertices;
	std::vector< glm::vec2 > herbivoreUvs;
	std::vector< glm::vec3 > herbivoreNormals;

	std::vector< glm::vec3 > carnivoreVertices;
	std::vector< glm::vec2 > carnivoreUvs;
	std::vector< glm::vec3 > carnivoreNormals;

	int herbivoreIndex = 0;
	int carnivoreIndex = 0;

	int size = (myList.size() / 2) - 1;	

	float herbPosition = -size;
	float carnPosition = -size;	

	for (std::vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

		string fileValue = *i;

		if (fileValue == "herbivore") {				

			if (herbivoreIndex == 0) {
				loadOBJ("creeper.obj", herbivoreVertices, herbivoreUvs, herbivoreNormals);
			}

			herbivoreArray.push_back(vec3(-5.0f, 0.0f, herbPosition));
			herbPosition += 2;

			Matrixes tempMatrix;
			tempMatrix.name = fileValue;
			tempMatrix.ModelMatrix = mat4(1.0);
			tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, herbivoreArray[herbivoreIndex]);
			MatrixArray.push_back(tempMatrix);
			
			herbivoreIndex++;
			herbivoreCount++;
		}
		else if (fileValue == "carnivore") {

			if (carnivoreIndex == 0) {
				loadOBJ("creeper.obj", carnivoreVertices, carnivoreUvs, carnivoreNormals);
			}

			carnivoreArray.push_back(vec3(5.0f, 0.0f, carnPosition));
			carnPosition += 2;
			
			Matrixes tempMatrix;
			tempMatrix.name = fileValue;
			tempMatrix.ModelMatrix = mat4(1.0);	
			tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, carnivoreArray[carnivoreIndex]);
			MatrixArray.push_back(tempMatrix);

			carnivoreIndex++;
			carnivoreCount++;
		}
		else{
			cout << fileValue << " does not exist" << endl;
		}
	};		

	////////////////////////////////////////////////////////////////// CREEPER	
	
	GLuint herbivoreVertexbuffer;
	glGenBuffers(1, &herbivoreVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, herbivoreVertexbuffer);
	if (herbivoreVertices.size()) {
		glBufferData(GL_ARRAY_BUFFER, herbivoreVertices.size() * sizeof(glm::vec3), &herbivoreVertices[0], GL_STATIC_DRAW);
	}

	GLuint herbivoreUvbuffer;
	glGenBuffers(1, &herbivoreUvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, herbivoreUvbuffer);
	if (herbivoreUvs.size()) {
		glBufferData(GL_ARRAY_BUFFER, herbivoreUvs.size() * sizeof(glm::vec2), &herbivoreUvs[0], GL_STATIC_DRAW);
	}
	////////////////////////////////////////////////////////////////// CREEPER
	
	unsigned int pngTextureHerbivore;
	glGenTextures(1, &pngTextureHerbivore);
	glBindTexture(GL_TEXTURE_2D, pngTextureHerbivore);

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

	GLuint carnivoreVertexbuffer;
	glGenBuffers(1, &carnivoreVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, carnivoreVertexbuffer);
	if (carnivoreVertices.size()) {
		glBufferData(GL_ARRAY_BUFFER, carnivoreVertices.size() * sizeof(glm::vec3), &carnivoreVertices[0], GL_STATIC_DRAW);
	}
	GLuint carnivoreUvbuffer;
	glGenBuffers(1, &carnivoreUvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, carnivoreUvbuffer);
	if (carnivoreUvs.size()) {
		glBufferData(GL_ARRAY_BUFFER, carnivoreUvs.size() * sizeof(glm::vec2), &carnivoreUvs[0], GL_STATIC_DRAW);
	}
	////////////////////////////////////////////////////////////////// BOAT

	unsigned int pngTextureCarnivore;
	glGenTextures(1, &pngTextureCarnivore);
	glBindTexture(GL_TEXTURE_2D, pngTextureCarnivore);

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
		movementControls(window, VertexArrayID, herbivoreUvbuffer, herbivoreUvs, herbivoreVertices, carnivoreUvbuffer, carnivoreUvs, carnivoreVertices, TextureID, MatrixID);

		collision();

		draw(MatrixID, herbivoreVertexbuffer, herbivoreUvbuffer, herbivoreVertices, carnivoreVertexbuffer, carnivoreUvbuffer, carnivoreVertices, pngTextureHerbivore, pngTextureCarnivore);
	
		dayCycles();	
			
		
		// END
		if (MatrixArray.size() == 0) {

			herbivoreArray.clear();
			carnivoreArray.clear();

			MatrixArray.clear();
			myList.clear();

			glfwWindowShouldClose(window);
			glDeleteBuffers(1, &herbivoreVertexbuffer);
			glDeleteBuffers(1, &herbivoreUvbuffer);
			glDeleteBuffers(1, &carnivoreVertexbuffer);
			glDeleteBuffers(1, &carnivoreUvbuffer);
			glDeleteProgram(shader);
			glDeleteTextures(1, &TextureID);
			glDeleteVertexArrays(1, &VertexArrayID);
			glfwTerminate();

			cout << "All Dino's have been eaten or starved!" << endl;
			
			string answer;
			cout << "Would you Like to start again?( Y or N )\n>";
			cin >> answer;

			if (answer == "Y" || answer == "y") {				
				main();
			}
			else {				
				return 0;
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
	glDeleteBuffers(1, &herbivoreVertexbuffer);
	glDeleteBuffers(1, &herbivoreUvbuffer);
	glDeleteBuffers(1, &carnivoreVertexbuffer);
	glDeleteBuffers(1, &carnivoreUvbuffer);
	glDeleteProgram(shader);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	return 0;
}