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
Q : Will remove the textures.
W : Will generate and bind a texture of "whitePaper.png" to loaded models.
E : Will generate and bind the creeper texture to loaded models.

## Code

The first thing the code will do is to prompt the user for file name inputs:
```
  cout << "Please enter the first valid .obj file.\n>";
	getline(cin, fileToLoad);
	cout << "You entered: " << fileToLoad1 << endl << endl;

```




