#include "C_Shader.h"
#include "C_Buffer.h"
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
	
const short int WIDTH = 1080;
const short HEIGHT = 720;
const float BGcolor[4] = { 0.7f , 0.75f , 0.85f , 1.0f };

const float vertices[] = {
	// points || Color || Position
		0.0f ,  0.5f , 0.0f ,	 0.0f ,  1.0f,  0.0f ,
		0.5f ,  0.0f , 0.0f ,	 1.0f ,  0.0f,  0.0f ,
		-0.5f, -0.5f , 0.0f ,	 0.0f ,  0.0f,  1.0f ,
		0.7f ,  0.7f , 0.0f ,	 1.0f ,  1.0f,  1.0f ,
		// ab baar baar points ko redeclare karke space kyu use kare ab hum indexed buffer use karenge 
};

unsigned int IndexedVertices[] = {
	0,1,2,0,3,1,
};

bool ErrorLog();
void UserInput(GLFWwindow* window);

int main() {

	if (!glfwInit()) {
		std::cout<<"Failed to initialize GLFW \n";
	}

	//specify to show which gl version is we using and core or waht level
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creates window of size with 1st null value shouws to fullscreen or not , 2nd is mystery
	GLFWwindow *Apple = glfwCreateWindow(WIDTH, HEIGHT, "crimeMaster GOGO", nullptr, nullptr);
	if (!Apple) {
		glfwTerminate();
		std::runtime_error("Failed to create GLFW window");
	}

	//used for FrameLimit
	//glfwSwapInterval(1);

	//make our all context region to screen
	glfwMakeContextCurrent(Apple);

	//loads glad for usage 
	//opengl provides some function in driver of gpu which i used by gpu but for us to use that func we need their address so glad gives us address of func if we dont use glad we need to use windows pointer to get func add.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout<<"Failed to initialize GLAD";
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	//step2 - make window size responsive
	// HINT
	//glfwSetFramebufferSizeCallback(window, resize_window);

	glClearColor(BGcolor[0], BGcolor[1], BGcolor[2], BGcolor[3]);
	//specifys clear colour remember not background collor but main colour of window

	
	C_Buffer cache;
	cache.createBuffer(vertices,4, IndexedVertices, 2);
	cache.linkvertArray(0,3,6*sizeof(float),0); //for vertices
	cache.linkvertArray(1,3,6*sizeof(float),3); //for color
	
	C_Shader newShader;
	newShader.activate();

	//float initial_color = 0.5f;
	////takes location of gpu stored variable that is her vec4 by checking name of var
	//int U_color = glGetUniformLocation(newShader.GPUcode, "U_color");
	//if (U_color == -1) {
	//	std::cout << "Failed to find uniform location for 'U_color'  \n ";
	//}
	//glUniform4f(U_color, initial_color, 0.0f, 1 - initial_color, 1.0f);

	


	while (!glfwWindowShouldClose(Apple)) {

		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(cache.vertexArrayScript);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//drawarray sirf vertecis data ko draw karta hai but agar elemnt specify kiya tab gpu indexed buffer read karke uske vertices draw karta hai

		glfwSwapBuffers(Apple);
		//swaps loaded buffer with present buffer
		UserInput(Apple);
		glfwPollEvents();
		if (ErrorLog()) { std::cerr << "An OpenGL error occurred!" << std::endl; }
	}
	glfwDestroyWindow(Apple);
	cache.revoke();
	newShader.revoke();

	glfwTerminate();
	return 0;
}

void UserInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}
bool ErrorLog() {
	GLenum error = glGetError();
	//fetch error and store for further use
	if (error != GL_NO_ERROR) {
		std::cout << "Main-Error Log - 0x" << error << "\n";
		return true;
	}
	else return false;
}
