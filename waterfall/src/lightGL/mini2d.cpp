#include <ktMini/box2d.hpp>

#include<windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"

#pragma warning(disable:4244)

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void castos_position_callback(GLFWwindow* window, double xpose, double ypose);
void processInput(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 562;
unsigned int SCR_HEIGHT = 562;
int drawcall = 0;
int datachange = 0;
int rightButton = 0;
int mousechange = 0;
double wph = 1.;// 16. / 9.;

kaktus::vec2 mouse = kaktus::vec2();
float mouseR = 1.5;
int mgb = 0;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL_M#", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, castos_position_callback);

	framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSwapInterval(1);
	// build and compile shader zprogram
	// ------------------------------------
	Shader viewShader("t0.vs", "t0.fs");
	unsigned int VBO, VAO,EBO, textures;
	unsigned int VAOl,VBOl, EBOl;
	unsigned int VAOc, VBOc;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, box::End * 2 * sizeof(float), box::TAllData._Elems, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, box::eboSize1 * sizeof(int), box::ebo1._Elems, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	//va0111111111111111111111111111111111111111
	glGenVertexArrays(1, &VAOl);
	glBindVertexArray(VAOl);

	glGenBuffers(1, &VBOl);
	glBindBuffer(GL_ARRAY_BUFFER, VBOl);
	glBufferData(GL_ARRAY_BUFFER, box::End * 2 * sizeof(float), box::AllData._Elems, GL_STATIC_DRAW);

	glGenBuffers(1, &EBOl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOl);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, box::End * 2 * sizeof(int), box::ebo2._Elems, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &VAOc);
	glBindVertexArray(VAOc);

	float cr[64];
	float theta;
	for (int i = 0; i < 32; i++)
	{
		theta = i;
		theta /= 32;
		theta *= kaktus::TAU;
		cr[2 * i] = cos(theta);
		cr[2 * i + 1] = sin(theta);
	}
	glGenBuffers(1, &VBOc);
	glBindBuffer(GL_ARRAY_BUFFER, VBOc);
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(float), cr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	

	glGenTextures(1, &textures);
	glBindTexture(GL_TEXTURE_2D, textures);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("./img/ok1.png", &width, &height, &nrChannels, 0);
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

	viewShader.use();
	glUniform1i(glGetUniformLocation(viewShader.ID, "texture1"), 0);
	drawcall = 1;
	double time = 0;
	//draw init
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_ALWAYS);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
	}
	while (!glfwWindowShouldClose(window))
	{
		time = glfwGetTime() + 0.016;
		processInput(window);
		if (drawcall)
		{
			glUniform1i(glGetUniformLocation(viewShader.ID, "drawtype"), 1);//2
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, box::eboSize1, GL_UNSIGNED_INT, 0);
			glUniform1i(glGetUniformLocation(viewShader.ID, "drawtype"), 2);
			glBindVertexArray(VAOl);
			glDrawElements(GL_LINES, box::End*2, GL_UNSIGNED_INT, 0);
			glUniform1i(glGetUniformLocation(viewShader.ID, "drawtype"), 3);
			glUniform1i(glGetUniformLocation(viewShader.ID, "mouse"), rightButton);
			glUniform1i(glGetUniformLocation(viewShader.ID, "size"), box::touchNum + rightButton);
			//glUniform3f(glGetUniformLocation(viewShader.ID, "clist"), mouse.x, mouse.y, mouseR);
			glUniform3fv(glGetUniformLocation(viewShader.ID, "clist"),11,(float*)box::touchpoint);//1
			glBindVertexArray(VAOc);
			glDrawArraysInstanced(GL_LINE_LOOP, 0, 32, box::touchNum + rightButton);
			glfwSwapBuffers(window);//17 cpuT
			drawcall = 1;
		}
		glfwPollEvents();//1 cpuT
		if (datachange )
		{
			int offset = box::gbs[datachange - 1];
			int length = box::gbs[datachange] - offset;
			//printf("%d,%d,%d\n", datachange, offset, length);
			glBufferSubData(GL_ARRAY_BUFFER, offset * 2 * sizeof(float),length * 2 * sizeof(float), &box::TAllData._Elems[offset]);
		}
		box::mouseTest = rightButton;
		if (mousechange)
		{
			
			printf("\033[H\033[J");
			for (auto v : box::loop())
				printf("%d,", v);
			printf("\033[1EXY:%.2f %.2f %.2f", mouse.x, mouse.y, mouseR);
			printf("\tmg:%d", mgb);
			if (mgb)
				printf("R:%.2f", box::R0[mgb - 1]);
			printf("\n");
		}
		mousechange = 0;
		datachange = 0;
		time -= glfwGetTime();
		Sleep((unsigned long)max(0, time * 1000));
	}
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	drawcall = 1;
	double k = width;
	k /= height;
	if (k > wph)
	{
		width = (int)round(wph * height);
	}
	else
	{
		height = (int)round(1. * width / wph);
	}
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		rightButton = 1;//zuojian
		//box::touchNum += 1;
		box::touchpoint[0] = { mouse.x,mouse.y ,mouseR };
		mousechange = 1;
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		//box::touchNum -= 1;
		box::touchpoint[0] = {1000,1000,0};
		rightButton = 0;
		mousechange = 1;
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (rightButton)
	{
		mouseR += yoffset * 0.1;
		mouseR = max(mouseR,1);
		mouseR = min(mouseR, 200);
		box::touchpoint[0].z = mouseR;
	}
	else
	{
		int gb = box::testpoint(mouse, 1.5);
		if (gb != 0)
		{
			box::ChangeR(gb-1, yoffset * 0.1);
			datachange = gb;
		}
		mgb = gb;
	}
	mousechange = 1;
}
void castos_position_callback(GLFWwindow* window, double xpose, double ypose)
{
	double wh = min(SCR_HEIGHT, SCR_WIDTH);
	int w, h;
	glfwGetWindowSize(window,&w, &h);
	ypose = h - ypose;
	mouse.x = 2 * (xpose / wh)-1;
	mouse.y = 2 * (ypose / wh) - 1;
	mouse *= 105;
	mousechange = 1;
	box::touchpoint[0].x = mouse.x;
	box::touchpoint[0].y = mouse.y;
}