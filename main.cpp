#define GLM_FORCE_RADIANS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>
#define GLEW_STATIC

using namespace std;

//compiler flags: g++ main.cpp -lGLEW -lGL -lglfw -lSOIL -std=c++11

char* initShaderString(char* filename){
	FILE *fp;
	fp = fopen(filename, "r");//"r" = readonly
	int size;
	char *str;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	str = (char*)malloc(size + 1);
	fread(str, 1, size, fp);
	str[size] = '\0';
	fclose(fp);
    return str;
}


GLuint initShaders(){
     //shader time!
	char* vertexString = initShaderString("vertexShader.glsl");//dumps contents of vertexShaderl.glsl into vertexString
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);//vertexShader is now designated primary vertex shader
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexString, NULL);//Shader Source is now holding vertex string
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char* fragmentString = initShaderString("fragmentShader.glsl");
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentString, NULL);
	glCompileShader(fragmentShader);
    free(vertexString);
	free(fragmentString);

	GLuint shaderProgram = glCreateProgram();//create a shader program
	glAttachShader(shaderProgram, vertexShader);//link vertexShader to it
	glAttachShader(shaderProgram, fragmentShader);//link fragmentShader to it

    glBindFragDataLocation(shaderProgram, 0, "finalColor");//specify which output is writing to which buffer
    glLinkProgram(shaderProgram);//links openGL to our shader program
	glUseProgram(shaderProgram);//designates shaderProgram as currently active shaders

    return shaderProgram;
}

void loadImage(char* filename){
	int width, height;
	unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
}

void pollInput(GLFWwindow* window, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale){
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			rotation.y += .05f;
		if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			rotation.y -= .05f;
        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			rotation.x += .05f;
		if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			rotation.x += -.05f;
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			translation.x -=.05f;
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			translation.x += .05f;
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			translation.y += .05f;
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			translation.y -= .05f;
		if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			scale *= glm::vec3(0.99f);
		if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			scale *= glm::vec3(1.01f);
}

int main(){
	int time = 0;
	glfwInit();
	//std::this_thread::sleep_for(std::chrono::seconds(0));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr); // Windowed
	//GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr) // Fullscreen
    glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE; //force modern OpenGL Method for checking function availability
	glewInit();
	glEnable(GL_DEPTH_TEST);//depth testing for Z-axis
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float vertices[] = {
	//  Position            Color             Texcoords
		-0.25f,  -0.4f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// Front-Left
        0.25f,  -0.4f, 0.25f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Font-Right
		0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Front-Top

		-0.25f, -0.4f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Back-Left
		0.25f, -0.4f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// Back-Right
		0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Back-Top

		-0.25f, -0.4f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // LS-Left
		-0.25f, -0.4f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// LS-Right
		0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LS-Top

		0.25f, -0.4f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // RS-Left
		0.25f, -0.4f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// RS-Right
		0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // RS-Top

		-0.25f, -0.4f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // BL-Back
		0.25f, -0.4f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// BL-Right
		-0.25f, -0.4f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // BL-left

		0.25f, -0.4f, -0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // BR-Back
		0.25f, -0.4f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// BR-Right
		-0.25f, -0.4f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // BR-left


	};

	GLuint vaoID;//create an ID for a VAO, which stores all the links between attribs and VBOs with raw vertex data
	glGenVertexArrays(1, &vaoID);//make 1 VAO
	glBindVertexArray(vaoID);//start using vao
		//Note: Every time we call glVertexAttribPointer from this point forward, that info will be stored in VAO
        //This means that switching between vertex data/formats is as simple as binding another VAO

	GLuint vboID;//id-to-be for VBO(object that contains all vertex data)
	glGenBuffers(1, &vboID); //Generate 1 buffer(gives vbo a title in OpenGL's context)
	glBindBuffer(GL_ARRAY_BUFFER, vboID);//vboID is now the id of the currently active array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//put vertices in GL_ARRAY_BUFFER(currently identified by vboID)
		//GL_STATIC_DRAW: Uploaded once, drawn many times(World)
		//GL_DYNAMIC_DRAW: Changed/uploaded occasionally, drawn a lot more
		//GL_STREAM_DRAW: chenge almost every time it's drawn (UI)
		//
	GLuint shaderProgram  = initShaders();


    //Time for Textures!
	//GLuint tex;//create ID for texture
	//glGenTextures(1, &tex);//tag texture ID
	//glBindTexture(GL_TEXTURE_2D, tex);//bind tex to current texture. Tex is a 2D image.
	//loadImage("img.png");//load texture from external source
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//setting clamping fo either coord(S,T,R)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//i is for int. fv for float.
    //float bordcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };//red border
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	//set downscale mode to use the two mipmaps that most closely match the size of the pixel being textured and samples with nearest neighbour interpolation
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//set upscale mode to use the mipmap that most closely matches the size of the pixel being textured and samples with nearest neighbour interpolation
    //glGenerateMipmap(GL_TEXTURE_2D);//generate mipmaps from texture


	// Black/white checkerboard
	// float pixels[] = {
	//     0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
	//         1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
	//  };
	//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
	//  //texture type, LOD(0 is base, use for self-mipmap), internal pixel format(on GPU), wdith, height, 5th is always 0, format of pixels in array, array itself
    //  //begins loading at (0,0)

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");//retrieve ref to position input in vertex shader
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8*(sizeof(GLfloat)), 0); //position is 2 floats long, and only data in VBO. Shove the floats into posAttrib.
		//7*(sizeof(GLfloat)) is pace -- take first 3 floats for every 8 floats in vbo
	glEnableVertexAttribArray(posAttrib);//when you render, actually use this input

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "startColor");//retrieve ref to position input in vertex shader
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8*(sizeof(GLfloat)), (const GLvoid *)(3*(sizeof(GLfloat))));
		//2*(sizeof(GLfloat)) starts at index of 4th element
	glEnableVertexAttribArray(colorAttrib);//when you render, actually use this input

	//TEXTURE ATTRIBS GO!
	//GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	//glEnableVertexAttribArray(texAttrib);
	//glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8*(sizeof(GLfloat)), (const GLvoid *)(6*(sizeof(GLfloat))));

	//Time to start working with model/view/projection matrices.
    GLuint modID = glGetUniformLocation(shaderProgram, "model");
	glm::mat4 viewmatrix;//view matrix
	GLuint viewID = glGetUniformLocation(shaderProgram, "view");
	glm::mat4 promatrix;//projection matrix
	GLuint proID = glGetUniformLocation(shaderProgram, "projection");
	promatrix = glm::perspective(57.0f, 16.0f/9.0f, 0.1f, 100.0f);//FoV, aspect ratio, near clipping plane, far clipping plane
	viewmatrix = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); //Camera position, camera target, direction of up

	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1.0f);

	while(!glfwWindowShouldClose(window)){
		glfwSwapBuffers(window);//swap back/front buffers when finished drawing
		glfwPollEvents();//retrieve window events

		glm::mat4 modmatrix;//model matrix

		modmatrix = glm::translate(modmatrix, translation);
        modmatrix = glm::rotate(modmatrix, rotation.y, glm::vec3(0.0, 1.0, 0.0));//matrix to rotate on, value to rotate by(R), axis
        modmatrix = glm::rotate(modmatrix, rotation.x, glm::vec3(1.0, 0.0, 0.0));
        modmatrix = glm::scale(modmatrix, scale);

		glUniformMatrix4fv(modID, 1, GL_FALSE, &modmatrix[0][0]); //4 float vector,use modID, 1 matrix, don't transpose, use data from modmatrix
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewmatrix[0][0]);//this function copies your matrix into OpenGL video memory
		glUniformMatrix4fv(proID, 1, GL_FALSE, &promatrix[0][0]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//bitmask to make rendering behind in Z-axis work
		glDrawArrays(GL_TRIANGLES, 0, 18);
		GLfloat strobeColor = glGetUniformLocation(shaderProgram, "triangleStrobe");
		glUniform1f(strobeColor, (sin(time * 3.141592654/180)));
		time++;
		pollInput(window, translation, rotation, scale);
	}


	glfwTerminate();
}
