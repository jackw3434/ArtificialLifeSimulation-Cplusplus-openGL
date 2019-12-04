# modelLoader

## Setup

To run this application you will need to clone this repo and build the dependancies.

glew will be missing from nupengl packages, you will be required to uninstall and reinstall nupengl.core to build and run the application.

To run the application, press F5.
You will be prompted to type in the name of 2 different .obj files, either type "creeper.obj" or "boat.obj".

## Controls

You can use the arrow keys and mouse to move the camera.

P : Will activate Polygon Mode.
F : Will fill the faces after polygon mode has been activated.
C : Will clear the scene of models.
R : Will reload the objects back into the scene.
U : Will delete the UV Buffere and remove the Texture coordinates.
I ; Will Re-apply the texture coordinates.
Q : Will remove the textures.
W : Will generate and bind a texture of "whitePaper.png" to loaded models.
E : Will generate and bind the creeper texture to loaded models.

```
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
```

## Code

The Application can render 2 different models, both textured.

The first thing the code will do is to prompt the user for file name inputs:
```
cout << "Please enter the first valid .obj file.\n>";
getline(cin, fileToLoad);
cout << "You entered: " << fileToLoad1 << endl << endl;
```
If correct file names have been entered, we will initialize the dependancies and create a window to draw in.
```
if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}
	else {
		fprintf(stderr, "initialized GLFW\n");
	}

	glewExperimental = GL_TRUE;
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	

	window = glfwCreateWindow(1024, 768, "Model Loader", NULL, NULL);
	glfwMakeContextCurrent(window);		

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
```

### The next step is to generate and bind the VAO:
```
GLuint VertexArrayID;
glGenVertexArrays(1, &VertexArrayID);
glBindVertexArray(VertexArrayID);
```
### Load the shaders:
```
GLuint shader = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
glUseProgram(shader);

GLuint MatrixID = glGetUniformLocation(shader, "MVP");
GLuint TextureID = glGetUniformLocation(shader, "myTextureSampler");
```

### Start Reading the object.
```
std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals;

bool res = loadOBJ(fileToLoad1.c_str(), vertices, uvs, normals);
bool res2 = loadOBJ(fileToLoad2.c_str(), vertices, uvs, normals);
indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
```

### Generate and Bind Buffers
```
GLuint vertexbuffer;
glGenBuffers(1, &vertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

GLuint uvbuffer;
glGenBuffers(1, &uvbuffer);
glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

GLuint elementbuffer;
glGenBuffers(1, &elementbuffer);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
```

### Generate PNG textures
```
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
```

### Set up the Model, View and Projection and send the transformation to the currently bound shader.
```
glm::mat4 ModelMatrix = glm::mat4(1.0);
glm::mat4 ViewMatrix = getViewMatrix();
glm::mat4 ProjectionMatrix = getProjectionMatrix();		
ModelMatrix = glm::translate(ModelMatrix, glm::vec3(distanceValue, 0.0f, 0.0f));

glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);	
```

### Enable the Buffers:
```
glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glVertexAttribPointer(
	0,                  // attribute
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
);		

glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
glVertexAttribPointer(
	1,                                // attribute
	2,                                // size
	GL_FLOAT,                         // type
	GL_FALSE,                         // normalized?
	0,                                // stride
	(void*)0                          // array buffer offset
);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
```

### Activate textures
```
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, pngTexture);

glUniform1i(TextureID, 0);
```

### Draw
```
glDrawArrays(GL_TRIANGLES, 0, indices.size());

glDrawElements(
	GL_TRIANGLES,		//mode
	indices.size(),		//count
	GL_UNSIGNED_INT,	//type
	(void*)0			//element array buffer offset
);	
```

### Termination/ VBO and Shader Clean up:
```
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
```
if (strcmp(lineHeader, "v") == 0) {
	glm::vec3 vertex;
	fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
	temp_vertices.push_back(vertex);
}
```
The paser checks if the obj is made of Tri's or quad's.
```
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


