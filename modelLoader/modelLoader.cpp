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

using namespace glm;
using namespace std;

GLFWwindow* window;
GLuint TextureID;
string fileToLoad = "";
string numberofFilesToOpen = "";

std::vector<std::string> myList;

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

	std::cout << "Please enter how many different .obj files you wish to load.\n>";
	cin >> number;
	std::cout << "You entered: " << number << " files to load." << endl;
	
	for (int i = 0; i < number; i++)
	{		
		std::cout << "Please enter the " << i <<" valid .obj file name.\n>";
		cin >> fileToLoad;
		std::cout << "You entered: " << fileToLoad << i << endl;
		
		std::cout << "Please enter how many " << fileToLoad << "'s you wish to open.\n>";
		cin >> numberofFilesToOpen;
		std::cout << "You entered: " << numberofFilesToOpen << endl;

		for (int n = 0; n < stoi(numberofFilesToOpen); n++)
		{
			myList.push_back(fileToLoad.c_str());
		}	
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

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	string fileValue;
	
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

	int index = 0;

	for (std::vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {	
	
		std::cout << *i << ' ' << endl;;
		fileValue = *i;		

		if (fileValue == "creeper.obj") {
			loadOBJ(fileValue.c_str(), vertices, uvs, normals, creeperPositions[index]);
		}
		if (fileValue == "boat.obj") {
			loadOBJ(fileValue.c_str(), vertices, uvs, normals, boatPositions[index]);
		}
	
		index++;
	};	
		
		/*std::vector< unsigned short > indices;
		std::vector< glm::vec3 > indexed_vertices;
		std::vector< glm::vec2 > indexed_uvs;
		std::vector< glm::vec3 > indexed_normals;

		indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);*/

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

		// Generate buffer for the indices
		//GLuint elementbuffer;
		//glGenBuffers(1, &elementbuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

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

		int count = 0;

		do
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Compute the MVP matrix from keyboard and mouse input
			computeMatricesFromInputs();

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


			float distanceValue = 2.0f;


			vec3 objPositions[]
			{
				vec3(1.0f, 0.0f, 0.0f),
				vec3(0.0f, 1.0f, 0.0f),
				vec3(0.0f, 0.0f, 1.0f),
				vec3(2.0f, 1.0f, 1.0f),
				vec3(1.0f, 2.0f, 1.0f),
				vec3(1.0f, 1.0f, 2.0f),
			};

			glm::mat4 ModelMatrix = glm::mat4(1.0);
			glm::mat4 ViewMatrix = getViewMatrix();
			glm::mat4 ProjectionMatrix = getProjectionMatrix();


			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(distanceValue, 0.0f, 0.0f));
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			/*int length =  stoi(f1ToOpen);

			for (GLuint i = 0; i < length; i++)
			{

				ModelMatrix = glm::translate(ModelMatrix, objPositions[i]);
				glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			}*/

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// Index buffer
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pngTexture);
			// Set our "myTextureSampler" sampler to user Texture Unit 0
			glUniform1i(TextureID, 0);			

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());

			//glDrawElements(
			//	GL_TRIANGLES,		//mode
			//	indices.size(),		//count
			//	GL_UNSIGNED_INT,	//type
			//	(void*)0			//element array buffer offset
			//);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glfwSwapBuffers(window);
			glfwPollEvents();
			count++;
		}

		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0 || glfwGetKey(window, GLFW_KEY_Q));

		// Cleanup VBO and shader
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteProgram(shader);
		glDeleteTextures(1, &TextureID);
		glDeleteVertexArrays(1, &VertexArrayID);
		glfwTerminate();
		return 0;	
}