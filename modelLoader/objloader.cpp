#define _CRT_SECURE_NO_DEPRECATE

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

#include "objloader.hpp"
#include <GL\glew.h>
using namespace std;

class ObjMaterial {
public:
	string  name;
	GLfloat diffuse[4];
	GLfloat ambient[4];
	GLfloat specular[4];
	GLfloat emmissive[4];
	GLfloat shininess;
	GLuint  texture;
	bool    isVideo = false;
	GLuint  curFrame = 0;
	vector<GLuint>  videoFrames;
public:
	void clear() {
		name = "";
		isVideo = false;
		texture = -1;
		videoFrames.clear();
	}
};

ObjMaterial temp_mtl;

bool LoadMaterials(const char* mtlFile, const char* chmtl) {
	
	FILE* file = fopen(mtlFile, "r");
	string string1;
	string string2;

	string line;
	string textFile;
	ifstream myFile(mtlFile);

	while (getline(myFile, line)) {
		textFile += line;
		textFile.push_back('\n');
	};

	cout << textFile << endl;

	if (file == NULL) {
		cout << "Impossible to open the file !!!" << mtlFile << endl;
		return false;
	}
	else {
		//cout << "opened the file in LoadMaterials !!!" << mtlFile << endl;
	}		

	while (true) {		

		char lineHeader[128];
		char lineHeader2[128];
		// read the first and second word of the line
		int res = fscanf(file, "%s %s", &lineHeader, &lineHeader2);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
	
		// else : parse lineHeader	

			string1 = lineHeader2;
			string2 = chmtl;		

		if (string1 == string2) {

			temp_mtl.clear();
			temp_mtl.name = string2;
			cout << "new mtl in MTL file " << temp_mtl.name << endl;
			
			size_t location = textFile.find(string2);
			size_t locationWord = textFile.find(string2, location);
			cout << "reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee " << locationWord << endl;;
		
		
		}
		//if (strcmp(lineHeader, "Kd") == 0) {		
		//	fscanf(file, "%f %f %f\n", &temp_mtl.diffuse[0], &temp_mtl.diffuse[1], &temp_mtl.diffuse[2]);
		//	temp_mtl.diffuse[3] = 1.0f;
		//	//cout << "kd " << temp_mtl.name << " " << temp_mtl.diffuse[0] << " " << temp_mtl.diffuse[1] << " " << temp_mtl.diffuse[2] << endl;
		//}
		//else if (strcmp(lineHeader, "Ka") == 0) {			
		//	fscanf(file, "%f %f %f\n", &temp_mtl.ambient[0], &temp_mtl.ambient[1], &temp_mtl.ambient[2]);
		//	temp_mtl.ambient[3] = 1.0f;
		//	//cout << "ka " << temp_mtl.name << " " << temp_mtl.ambient[0] << " " << temp_mtl.ambient[1] << " " << temp_mtl.ambient[2] << endl;
		//}
		//else if (strcmp(lineHeader, "Ks") == 0) {			
		//	fscanf(file, "%f %f %f\n", &temp_mtl.specular[0], &temp_mtl.specular[1], &temp_mtl.specular[2]);
		//	temp_mtl.specular[3] = 1.0f;
		//	//cout << "ks " << temp_mtl.name << " " << temp_mtl.specular[0] << " " << temp_mtl.specular[1] << " " << temp_mtl.specular[2] << endl;
		//}
		//else if (strcmp(lineHeader, "Ke") == 0) {		
		//	fscanf(file, "%f %f %f\n", &temp_mtl.emmissive[0], &temp_mtl.emmissive[1], &temp_mtl.emmissive[2]);
		//	temp_mtl.emmissive[3] = 1.0f;
		//	//cout << "ke " << temp_mtl.name << " " << temp_mtl.emmissive[0] << " " << temp_mtl.emmissive[1] << " " << temp_mtl.emmissive[2]  << endl;
		//}
		//else if (strcmp(lineHeader, "Ns") == 0) {	
		//	fscanf(file, "%f\n", &temp_mtl.shininess);
		//	//cout << "Ns " << temp_mtl.name << " " << temp_mtl.shininess << endl;
		//}		
		/*else if (strcmp(lineHeader, "Ni") == 0) {
			fscanf(file, "%f\n", &temp_mtl.shininess);
			cout << "Ns " << temp_mtl.name << " " << temp_mtl.shininess << endl;
		}
		else if (strcmp(lineHeader, "d") == 0) {
			fscanf(file, "%f\n", &temp_mtl.shininess);
			cout << "Ns " << temp_mtl.name << " " << temp_mtl.shininess << endl;
		}*/
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	return true;
}

bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	string line;
	string textFile;
	ifstream myFile(path);
	char MaterialFilename[4096];
	
	while (getline(myFile, line)) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
	
		// else : parse lineHeader
		if (strcmp(lineHeader, "mtllib") == 0)
		{		
			fscanf(file, "%s", MaterialFilename);	
			cout << "MaterialFilename " << MaterialFilename << endl;			
		} 
		else if (strcmp(lineHeader, "usemtl") == 0) {				

			char chmtl[128];
			fscanf(file, "%s", chmtl);		
			
			LoadMaterials(MaterialFilename, chmtl);

			//temp_group.material = chmtl;			
		}
		else if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {	
				
					int length = 0;
					size_t slashLength = count(line.begin(), line.end(), '/');
					//cout << line << "\n";
					//cout << slashLength << "\n";
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
		
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	printf("Loaded OBJ");
	return true;
}

