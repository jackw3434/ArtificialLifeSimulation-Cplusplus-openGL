# model loader and artificial life simulation ReadMe.md and report.

## Project Aim
To be able to create an obj loader for an artificial life simulation game.

## Used Software
Visual Studio 2019
OpenGL 3.3

## Used Libraries
```c++
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
```
## Game
This is an artificial life simulation game.
The user selects how many herbivores, carnivores and grass they want, as well as how many days to run it for.

Dinosaurs must eat everyday in order to survive until the next day.
Carnivores eat herbivores and herbivores eat grass.
Each second, all dinosaurs will move in a random direction within the area, each second is counted as an hour in the simulation and each day ending after 24 seconds.

All dinosaurs that have eaten will return to the starting positions at the beginning of each day.
The simulation will run until all dinosaurs have either been eaten or starved, which will then prompt the user again if
they wish to restart the game and enter different parameters.

## Setup

To run this application you will need to clone this repo and build the dependancies.

glew will be missing from nupengl packages, you will be required to uninstall and reinstall nupengl.core to build and run the application.

To run the application, press F5.
A terminal will open, explaining how the game works and the rules.
```
Welcome to the dinosaur life simulation game.

To Play, simply enter how many different models you want, options are herbivore, carnivore and grass.
For each different model, enter its name and how many you want.
Then, enter how many days you want the simulation to run for.

Gameplay:
Dinosaurs must eat to survive until the next day.
Carnivors eat herbivores and herbivore eat grass.
Any Dinosaur that hasn't eaten at the end of each day wont have the energy to carry on and will starve.
Carnivores are the red cubes, herbivores are the green cubes and grass is represented as green squares.
```
The user will then be prompted to input the parameters of the simulation, upon completion the simulation will run until the end.
Here is an example of what one simulation setup would look like.

```console
Welcome to the dinosaur life simulation game.
>To Play, simply enter the number of how many different models you want, options are herbivore, carnivore and grass.
>For each different model, enter its name and how many you want.
>Then, enter how many days you want the simulation to run for.
>Gameplay:
>Dinosaurs must eat to survive until the next day.
>Carnivors eat herbivores and herbivore eat grass.
>Any Dinosaur that hasn't eaten at the end of each day wont have the energy to carry on and will starve.
>Carnivores are the red cubes, herbivores are the green cubes and grass is represented as green squares.
>Please enter how many different models you wish to load, either 1, 2 or 3.
>3
You entered: 3 files to load.
Please enter a valid name, either herbivore, carnivore or grass.
>herbivore
You entered: herbivore
Please enter how many herbivore's you wish to open.
>300
You entered: 300
Please enter a valid name, either herbivore, carnivore or grass.
>carnivore
You entered: carnivore
Please enter how many carnivore's you wish to open.
>100
You entered: 100
Please enter a valid name, either herbivore, carnivore or grass.
>grass
You entered: grass
Please enter how many grass's you wish to open.
>200
You entered: 200
Please enter how many days you would like the simulation to run.
>5
You entered: 5 days.
```
The user can enter any number of models they like, providing the user's computer can handle it.
After entering how many days the simulation runs for, the simulation will start.

## Controls

These movement controls were only utilised during the formative assessment and are not required for the game, but the user will still have access to these controls, strictly for demo purposes.

You can use the arrow keys and mouse to move the camera.

P : Will activate Polygon Mode.

F : Will fill the faces after polygon mode has been activated.

C : Will clear the scene of models.

R : Will reload the objects back into the scene.

U : Will delete the herbivore UV Buffer and remove the Texture coordinates.

I ; Will Generate and re-apply and bind the herbivore texture coordinates.

Q : Will remove the textures.

W : Will generate and bind a texture of "whitePaper.png" to loaded models.

E : Will generate and bind a green texture to loaded models.


```c++
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
		glBufferData(GL_ARRAY_BUFFER, herbivoreUvs.size() * sizeof(vec2), &herbivoreUvs[0], GL_STATIC_DRAW);
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
			cout << "Failed to load texture" << endl;
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
		unsigned char* data = stbi_load("green.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "Failed to load texture" << endl;
		}
		stbi_image_free(data);
	}		
}
```

## Code

The Application can render 3 different textured models.

The first thing the code will do is to prompt the user for file name inputs:
```c++
cout << "Please enter how many different .obj files you wish to load, either 1, 2 or 3.\n>";
cin >> number;
cout << "You entered: " << number << " files to load." << endl;

if (number == 0 || number > 3) {	
	cout << "Please load either 1, 2 or 3 models" << endl;
	getInput();
}

for (int i = 0; i < number; i++)
{
	cout << "Please enter a valid .obj file name, either herbivore, carnivore or grass.\n>";
	cin >> fileToLoad;
	cout << "You entered: " << fileToLoad << endl;

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
```
If correct file names have been entered, we will initialize the dependancies and create a window to draw in.
```c++
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

	window = glfwCreateWindow(1024, 768, "Artificial Life Simulation Game", NULL, NULL);
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
```

### The next step is to generate and bind the VAO:
```c++
GLuint VertexArrayID;
glGenVertexArrays(1, &VertexArrayID);
glBindVertexArray(VertexArrayID);
```
### Load the shaders:
```c++
GLuint shader = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
glUseProgram(shader);

GLuint MatrixID = glGetUniformLocation(shader, "MVP");
GLuint TextureID = glGetUniformLocation(shader, "myTextureSampler");
```

### Start Reading the object.
```c++
vector< vec3 > herbivoreVertices;
vector< vec2 > herbivoreUvs;
vector< vec3 > herbivoreNormals;

vector< vec3 > carnivoreVertices;
vector< vec2 > carnivoreUvs;
vector< vec3 > carnivoreNormals;

vector< vec3 > grassVertices;
vector< vec2 > grassUvs;
vector< vec3 > grassNormals;

int herbivoreIndex = 0;
int carnivoreIndex = 0;
int grassIndex = 0;

int size = (myList.size() / 3) - 1;	

float herbPosition = -size;
float carnPosition = -size;	
float grassPosition = -size;

for (vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

	string fileValue = *i;

	if (fileValue == "herbivore") {				

		if (herbivoreIndex == 0) {
			loadOBJ("cube.obj", herbivoreVertices, herbivoreUvs, herbivoreNormals);
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
			loadOBJ("cube.obj", carnivoreVertices, carnivoreUvs, carnivoreNormals);
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
	else if (fileValue == "grass") {

		if (grassIndex == 0) {
			loadOBJ("grass.obj", grassVertices, grassUvs, grassNormals);
		}

		float randomXGrassPosition = rand() % 11 + (-5);

		grassArray.push_back(vec3(randomXGrassPosition, -1, grassPosition));
		grassPosition += 2;

		Matrixes tempMatrix;
		tempMatrix.name = fileValue;
		tempMatrix.ModelMatrix = mat4(1.0);
		tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, grassArray[grassIndex]);
		MatrixArray.push_back(tempMatrix);

		grassIndex++;
		grassCount++;
	}
	else{
		cout << fileValue << " does not exist" << endl;
	}
};	
```

### Generate and Bind Buffers
```c++
GLuint herbivoreVertexbuffer;
glGenBuffers(1, &herbivoreVertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, herbivoreVertexbuffer);
if (herbivoreVertices.size()) {
	glBufferData(GL_ARRAY_BUFFER, herbivoreVertices.size() * sizeof(vec3), &herbivoreVertices[0], GL_STATIC_DRAW);
}

GLuint herbivoreUvbuffer;
glGenBuffers(1, &herbivoreUvbuffer);
glBindBuffer(GL_ARRAY_BUFFER, herbivoreUvbuffer);
if (herbivoreUvs.size()) {
	glBufferData(GL_ARRAY_BUFFER, herbivoreUvs.size() * sizeof(vec2), &herbivoreUvs[0], GL_STATIC_DRAW);
}	

GLuint grassVertexbuffer;
glGenBuffers(1, &grassVertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, grassVertexbuffer);
if (grassVertices.size()) {
	glBufferData(GL_ARRAY_BUFFER, grassVertices.size() * sizeof(vec3), &grassVertices[0], GL_STATIC_DRAW);
}

GLuint grassUvbuffer;
glGenBuffers(1, &grassUvbuffer);
glBindBuffer(GL_ARRAY_BUFFER, grassUvbuffer);
if (grassUvs.size()) {
	glBufferData(GL_ARRAY_BUFFER, grassUvs.size() * sizeof(vec2), &grassUvs[0], GL_STATIC_DRAW);
}

GLuint carnivoreVertexbuffer;
glGenBuffers(1, &carnivoreVertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, carnivoreVertexbuffer);
if (carnivoreVertices.size()) {
	glBufferData(GL_ARRAY_BUFFER, carnivoreVertices.size() * sizeof(vec3), &carnivoreVertices[0], GL_STATIC_DRAW);
}
GLuint carnivoreUvbuffer;
glGenBuffers(1, &carnivoreUvbuffer);
glBindBuffer(GL_ARRAY_BUFFER, carnivoreUvbuffer);
if (carnivoreUvs.size()) {
	glBufferData(GL_ARRAY_BUFFER, carnivoreUvs.size() * sizeof(vec2), &carnivoreUvs[0], GL_STATIC_DRAW);
}
```

### Generate PNG textures for each model.
```c++
unsigned int pngTextureHerbivore;
glGenTextures(1, &pngTextureHerbivore);
glBindTexture(GL_TEXTURE_2D, pngTextureHerbivore);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int widthHerbivore, heightHerbivore, nrChannelsHerbivore;
unsigned char* dataHerbivore = stbi_load("green.png", &widthHerbivore, &heightHerbivore, &nrChannelsHerbivore, 0);
if (dataHerbivore)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthHerbivore, heightHerbivore, 0, GL_RGB, GL_UNSIGNED_BYTE, dataHerbivore);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	cout << "Failed to load texture" << endl;
}
stbi_image_free(dataHerbivore);

unsigned int pngTextureGrass;
glGenTextures(1, &pngTextureGrass);
glBindTexture(GL_TEXTURE_2D, pngTextureGrass);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int widthGrass, heightGrass, nrChannelsGrass;
unsigned char* dataGrass = stbi_load("grassTexture.png", &widthGrass, &heightGrass, &nrChannelsGrass, 0);
if (dataGrass)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthGrass, heightGrass, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataGrass);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	cout << "Failed to load texture" << endl;
}
stbi_image_free(dataGrass);

unsigned int pngTextureCarnivore;
glGenTextures(1, &pngTextureCarnivore);
glBindTexture(GL_TEXTURE_2D, pngTextureCarnivore);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int widthCarnivore, heightCarnivore, nrChannelsCarnivore;
unsigned char* dataCarnivore = stbi_load("red.png", &widthCarnivore, &heightCarnivore, &nrChannelsCarnivore, 0);
if (dataCarnivore)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthCarnivore, heightCarnivore, 0, GL_RGB, GL_UNSIGNED_BYTE, dataCarnivore);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	cout << "Failed to load texture" << endl;
}
stbi_image_free(dataCarnivore);
```
## Create a ModelMatrix for each of the objects to be transformed independantly
```c++
for (vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

	string fileValue = *i;

	if (fileValue == "herbivore") {				

		if (herbivoreIndex == 0) {
			loadOBJ("cube.obj", herbivoreVertices, herbivoreUvs, herbivoreNormals);
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
			loadOBJ("cube.obj", carnivoreVertices, carnivoreUvs, carnivoreNormals);
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
	else if (fileValue == "grass") {

		if (grassIndex == 0) {
			loadOBJ("grass.obj", grassVertices, grassUvs, grassNormals);
		}

		float randomXGrassPosition = rand() % 11 + (-5);

		grassArray.push_back(vec3(randomXGrassPosition, -1, grassPosition));
		grassPosition += 2;

		Matrixes tempMatrix;
		tempMatrix.name = fileValue;
		tempMatrix.ModelMatrix = mat4(1.0);
		tempMatrix.ModelMatrix = translate(tempMatrix.ModelMatrix, grassArray[grassIndex]);
		MatrixArray.push_back(tempMatrix);

		grassIndex++;
		grassCount++;
	}
	else{
		cout << fileValue << " does not exist" << endl;
	}
};
```

#Main game loop
The logic of the game has been refactored into 3 methods that are called each frame, collision(), dayCycles() and draw();
Most of the game logic happens within dayCycles(), everything to do with movements, textures and models happens within draw().
```c++
do {	
	collision();
	dayCycles();
	draw(MatrixID, herbivoreVertexbuffer, herbivoreUvbuffer, herbivoreVertices, grassVertexbuffer, grassUvbuffer, grassVertices,           carnivoreVertexbuffer, carnivoreUvbuffer, carnivoreVertices, pngTextureHerbivore, pngTextureCarnivore, pngTextureGrass);
}
```

### Set up the Model, View and Projection and send the transformation to the currently bound shader for each of the objects that have been input by the user, which is handle with my draw() function which will also enable the buffers, activate textures and draw.
```c++
void draw(GLuint MatrixID, GLuint herbivoreVertexbuffer, GLuint herbivoreUvbuffer, vector<vec3> herbivoreVertices, GLuint grassVertexbuffer, GLuint grassUvbuffer, vector<vec3> grassVertices, GLuint carnivoreVertexbuffer, GLuint carnivoreUvbuffer, vector<vec3> carnivoreVertices, unsigned int pngTextureHerbivore, unsigned int pngTextureCarnivore, unsigned int pngTextureGrass)
{
mat4 ViewMatrix = getViewMatrix();
mat4 ProjectionMatrix = getProjectionMatrix();

int index = 0;	

    for (vector<string>::const_iterator i = myList.begin(); i != myList.end(); ++i) {

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

	if (fileValue == "grass") {

		// 1rst attribute buffer : grassVertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, grassVertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : grassUVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, grassUvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pngTextureGrass);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		MatrixArray[index].hasEaten = true;
		mat4 MVP;
		MVP = ProjectionMatrix * ViewMatrix * MatrixArray[index].ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, grassVertices.size());
	}

		index++;
    };
}	
```

After this setup process is complete, the game will start to run and process the data calling into other functions.
One of the functions that gets kicked off at the begining is the `dayCycles()` function. 

```c++
void dayCycles() {

	for (int i = 0; i < days; i++)
	{
		if (currentDay == i) {			
			moveEachSecond();
		}
	}		
}
```

This function will loop for the number of days entered by the user, calling into another function that runs every second.
Trying to accomplish specific movement executions each second was a challange, but i found a solution that has worked, if i had more time this would be an area for improvement as the code is bulky and could be condensed.

The function calculates deltaTime to get the value of seconds.

At the start of the day we set the hunger values for `hasEaten` to be `false`.
At each second I call into another function, `moveRandomly()`. The problem I encountered is that this would be true for each frame and would therefor moveRandomly more than once, my not-so-nice solution for this was to add boolean values to disable the ability as soon as it has run once, which is what i wanted to achieve in the first place.

At the end of the last hour of the day I restart the day by calling into my `restartDay()` method.

## moveEachSecond()
```c++
void moveEachSecond() {
	// Compute time difference between current and last frame
	static double lastTime = glfwGetTime();		
	double currentTime = glfwGetTime();
	int deltaTime = float(currentTime - lastTime);	

	if (floor(deltaTime) == 1 && canMoveAt1 == true) {

		for (int i = 0; i < MatrixArray.size(); i++)
		{
			MatrixArray[i].hasEaten = false;
		}

		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt1 = false;
	}
	if (floor(deltaTime) == 2 && canMoveAt2 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt2 = false;
	}
	if (floor(deltaTime) == 3 && canMoveAt3 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt3 = false;
	}
	if (floor(deltaTime) == 4 && canMoveAt4 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt4 = false;
	}
	if (floor(deltaTime) == 5 && canMoveAt5 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt5 = false;
	}
	if (floor(deltaTime) == 6 && canMoveAt6 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt6 = false;
	}
	if (floor(deltaTime) == 7 && canMoveAt7 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt7 = false;
	}
	if (floor(deltaTime) == 8 && canMoveAt8 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt8 = false;
	}
	if (floor(deltaTime) == 9 && canMoveAt9 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt9 = false;
	}
	if (floor(deltaTime) == 10 && canMoveAt10 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt10 = false;
	}
	if (floor(deltaTime) == 11 && canMoveAt11 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt11 = false;
	}
	if (floor(deltaTime) == 12 && canMoveAt12 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt12 = false;
	}
	if (floor(deltaTime) == 13 && canMoveAt13 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt13 = false;
	}
	if (floor(deltaTime) == 14 && canMoveAt14 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt14 = false;
	}
	if (floor(deltaTime) == 15 && canMoveAt15 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt15 = false;
	}
	if (floor(deltaTime) == 16 && canMoveAt16 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt16 = false;
	}
	if (floor(deltaTime) == 17 && canMoveAt17 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt17 = false;
	}
	if (floor(deltaTime) == 18 && canMoveAt18 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt18 = false;
	}
	if (floor(deltaTime) == 19 && canMoveAt19 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt19 = false;
	}
	if (floor(deltaTime) == 20 && canMoveAt20 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt20 = false;
	}
	if (floor(deltaTime) == 21 && canMoveAt21 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt21 = false;
	}
	if (floor(deltaTime) == 22 && canMoveAt22 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt22 = false;
	}
	if (floor(deltaTime) == 23 && canMoveAt23 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();
		canMoveAt23 = false;
	}
	if (floor(deltaTime) >= 24 && canMoveAt24 == true) {
		cout << "Day: " << currentDay + 1 << " Time: " << deltaTime << ":00" << endl;
		moveRandomly();	
		restartDay();
	}	
}
```
## moveRandomly()
In this method I use a random number generater to select either 0 or 1 to choose to either translate on the X axis of the Z axis.
Once chosen I use the random number generater again to move the model by either +1 or -1, checking it position too keep it in bounds.
This is called for all of the dinosaurs each second.
```c++ 
void moveRandomly() {	

int edgeValue = MatrixArray.size() -1;

float moveSpeed = 1.0f;
srand(time(NULL));

for (int i = 0; i < MatrixArray.size(); i++)
{	

if(MatrixArray[i].name != "grass"){

	int randomAxisValue = rand() % 2;

	if (randomAxisValue == 0) {
		// X Value
		int randomMovementvalue = rand() % 2;

		if (randomMovementvalue == 0) {
			// +1 on the X value			
			if (MatrixArray[i].ModelMatrix[3].x >= 5) {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(-moveSpeed, 0.0f, 0.0f));
			}
			else {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(moveSpeed, 0.0f, 0.0f));
			}
		}

		if (randomMovementvalue == 1) {
			// -1 on the X value
			if (MatrixArray[i].ModelMatrix[3].x <= -5) {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(moveSpeed, 0.0f, 0.0f));
			}
			else {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(-moveSpeed, 0.0f, 0.0f));
			}
		}
	}

	if (randomAxisValue == 1) {
		// Z Value
		int randomMovementvalue = rand() % 2;

		if (randomMovementvalue == 0) {
			// +1 on the Z value
			if (MatrixArray[i].ModelMatrix[3].z >= MatrixArray[edgeValue].ModelMatrix[3].z) {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(0.0f, 0.0f, -moveSpeed));
			}
			else {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(0.0f, 0.0f, moveSpeed));
			}
		}

		if (randomMovementvalue == 1) {
			// -1 on the Z value
			if (MatrixArray[i].ModelMatrix[3].z <= MatrixArray[0].ModelMatrix[3].z) {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(0.0f, 0.0f, moveSpeed));
			}
			else {
				MatrixArray[i].ModelMatrix = translate(MatrixArray[i].ModelMatrix, vec3(0.0f, 0.0f, -moveSpeed));
			}
		}
	}
}		
}
};

```

After 24 seconds, dinosaurs have had the whole day to roam around to find food to ensure their survivel before the day restarts.
Keeping count of how many models are around and tracking how many days the simulation runs for.

The function will keep all grass in their origional places, an improvement on this feature if i had more time would be to make the reminaing grass spread out by 1 at the end of each day to increase food for the herbivores.

Any dinosaurs that havnt eaten are removed and any that have eaten return to the start position. If i had more time I could implement a feature where if 2 dinosaurs of the same species who have both eaten meet within the same day then there could be a chance for reproducing and would therefor add another dinosaur of that species to the start of the next day, increasing the population.

## restartDay()
```c++
void restartDay() {
	int herbivoreIndex = 0;
	int carnivoreIndex = 0;
	int grassIndex = 0;

	if (currentDay == days) {
		return;
	}

	if (MatrixArray.size() > 0) {
		for (int i = 0; i < MatrixArray.size();)
		{
			if (MatrixArray.size() > 0) {
				if (MatrixArray[i].name == "grass") {
					MatrixArray[i].ModelMatrix[3].x = grassArray[grassIndex].x;
					MatrixArray[i].ModelMatrix[3].z = grassArray[grassIndex].z;
					grassIndex++;
				}

				if (MatrixArray[i].hasEaten == false) {

					if (MatrixArray[i].name == "herbivore") {
						herbivoreCount--;
					}

					if (MatrixArray[i].name == "carnivore") {
						carnivoreCount--;
					}

					MatrixArray.erase(MatrixArray.begin() + i);
					myList.erase(myList.begin() + i);
					i--;
				}
				else if (MatrixArray[i].hasEaten == true) {

					if (MatrixArray[i].name == "herbivore") {
						MatrixArray[i].ModelMatrix[3].x = herbivoreArray[herbivoreIndex].x;
						MatrixArray[i].ModelMatrix[3].z = herbivoreArray[herbivoreIndex].z;
						herbivoreIndex++;
						
						//add children
					}
					if (MatrixArray[i].name == "carnivore") {

						// move carno's that have eaten back to start and add their children
						MatrixArray[i].ModelMatrix[3].x = carnivoreArray[carnivoreIndex].x;
						MatrixArray[i].ModelMatrix[3].z = carnivoreArray[carnivoreIndex].z;
						carnivoreIndex++;

						//add children						
					}
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

	cout << "Day " << currentDay + 1 << " finished." << endl;
	cout << "Herbivore count: " << herbivoreCount << endl;
	cout << "Carnivore count: " << carnivoreCount << endl;
	cout << "Grass count: " << grassCount << endl;
	glfwSetTime(0);
	currentDay++;
}
```
At the end of the day the user is presented with the results of that day.

## Collision()
With lots of models moving around it is required to know when certain modles need to interact with each other.
Keeping track if any carnivores eat any herbivores and if any herbivores eat any grass.
```c++

void collision() {
	
	for (size_t i = 0; i < MatrixArray.size(); i++)
	{

		for (size_t j = 0; j < MatrixArray.size(); j++)
		{
			if (i != j) {
				if (MatrixArray[i].ModelMatrix[3].x == MatrixArray[j].ModelMatrix[3].x && MatrixArray[i].ModelMatrix[3].z == MatrixArray[j].ModelMatrix[3].z) {	

					if (MatrixArray[i].name == "carnivore" && MatrixArray[j].name == "herbivore") {

						// Has eaten will survive to the next day. carnivore hitting herbivore 		
						cout << "A carnivore has eaten a herbivore." << endl;
						MatrixArray[i].hasEaten = true;

						MatrixArray.erase(MatrixArray.begin() + j);
						myList.erase(myList.begin() + j);

						herbivoreCount--;
						cout << "herbivore count: " << herbivoreCount << endl;

						i--;
						j--;										
					}		
					if (MatrixArray[i].name == "herbivore" && MatrixArray[j].name == "grass") {

						// Has eaten will survive to the next day. herbivore hitting grass
						cout << "A herbivore has eaten a grass." << endl;
						MatrixArray[i].hasEaten = true;

						MatrixArray.erase(MatrixArray.begin() + j);
						myList.erase(myList.begin() + j);	

						grassCount--;
						cout << "grass count: " << grassCount << endl;
					}
				}
			}
		}
	}
}
```

## End
If there are no more dinosaurs alive or the simulation has run its course the game will end, asking the user to play again or not.

```c++
// END
// If End Of Simulation
int remainder = MatrixArray.size() - grassCount;
if (currentDay == days) {			

	herbivoreArray.clear();
	carnivoreArray.clear();
	grassArray.clear();

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

	cout << "The Simulation has ended" << endl;

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
// If All dino's are dead
else if (remainder == 0) {

	herbivoreArray.clear();
	carnivoreArray.clear();
	grassArray.clear();

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
```


### Termination/ VBO and Shader Clean up:
```c++
glDeleteBuffers(1, &vertexbuffer);
glDeleteBuffers(1, &uvbuffer);
glDeleteProgram(shader);
glDeleteTextures(1, &TextureID);
glDeleteVertexArrays(1, &VertexArrayID);
glfwTerminate();
return 0;
```

# OBJ Paser
The paser reads each line of the obj file and stores the values of v, vt, vn and f:
```c++
if (strcmp(lineHeader, "v") == 0) {
	glm::vec3 vertex;
	fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
	temp_vertices.push_back(vertex);
}
```
The paser checks if the obj is made of Tri's or quad's.
```c++
if (strcmp(lineHeader, "f") == 0) {	

	int length = 0;
	size_t slashLength = count(line.begin(), line.end(), '/');				
	length = slashLength;

	if (length == 8) {
		//cout << "QUADS" << "\n";
		unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
		int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
		if (matches != 12) {
			printf("File can't be read by our simple parser :-( Try exporting with other options\n");
			return false;
		}
		vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[1]);
		vertexIndices.push_back(vertexIndex[2]);
		vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[2]);
		vertexIndices.push_back(vertexIndex[3]);

		uvIndices.push_back(uvIndex[0]);
		uvIndices.push_back(uvIndex[1]);
		uvIndices.push_back(uvIndex[2]);
		uvIndices.push_back(uvIndex[0]);
		uvIndices.push_back(uvIndex[2]);
		uvIndices.push_back(uvIndex[3]);

		normalIndices.push_back(normalIndex[0]);
		normalIndices.push_back(normalIndex[1]);
		normalIndices.push_back(normalIndex[2]);
		normalIndices.push_back(normalIndex[0]);
		normalIndices.push_back(normalIndex[2]);
		normalIndices.push_back(normalIndex[3]);
	}

	if (length == 6) {
		//cout << "TRI's" << "\n";
		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
		int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
		if (matches != 9) {
			printf("File can't be read by our simple parser :-( Try exporting with other options\n");
			return false;
		}
		vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[1]);
		vertexIndices.push_back(vertexIndex[2]);
		uvIndices.push_back(uvIndex[0]);
		uvIndices.push_back(uvIndex[1]);
		uvIndices.push_back(uvIndex[2]);
		normalIndices.push_back(normalIndex[0]);
		normalIndices.push_back(normalIndex[1]);
		normalIndices.push_back(normalIndex[2]);
		}
	}	
```

#Report

