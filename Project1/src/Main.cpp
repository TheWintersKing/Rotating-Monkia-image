#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define DEBUG 0

#if DEBUG
	#define LOG(x) std::cout << x << "\n"
#else
	#define LOG(x)
#endif


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

static unsigned int CompileShader( unsigned int type , const std::string& source);

struct  ShaderProgramSrc PraserShader(const std::string& file);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct  ShaderProgramSrc
{
	std::string VertexSrc;
	std::string FragmentSrc;
};


int main(void)

{
	GLFWwindow *window;

	float vertices[] = {
		0.5f , 0.5f , 0.0f , 1.0f , 1.0f ,
		0.5f , -0.5f , 0.0f  ,  1.0f , 0.0f ,
		-0.5f , -0.5f , 0.0f  ,  0.0f , 0.0f ,
		-0.5 , 0.5f , 0.0f ,  0.0f , 1.0f ,
	};

	unsigned int indices[] = {
		0 , 1 , 3 ,
		1 , 2 , 3
	};

	unsigned int VBO , VAO , EBO;

	if (!glfwInit())
	{
		std::cout << "Error Bitch"<<std::endl;
		system("PAUSE");
		return -1;
	}

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT , "Helloworld", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		std::cout << "Failed like a bitch"<<std::endl;
		system("PAUSE");
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed bitch GLAD" << std::endl;
		system("PAUSE");
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	glGenVertexArrays(1 , &VAO);

	glGenBuffers (1 , &VBO);

	glGenBuffers (1 , &EBO);

	glBindVertexArray(VAO);

	glBindBuffer (GL_ARRAY_BUFFER , VBO);

	glBufferData (GL_ARRAY_BUFFER , sizeof (vertices) , vertices , GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER , sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer (0 , 3 , GL_FLOAT , GL_FALSE , sizeof (float) * 5 ,  (void*)0);

	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));

	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(1);

	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widht, height, nChannels;

	unsigned char *data = stbi_load("res/Textures/images.jpg" , &widht , &height , &nChannels , 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widht, height, 0, GL_RGB ,GL_UNSIGNED_BYTE,data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	else
	{
		std::cout << "Failed to loaad texture" << std::endl;
	}

	stbi_image_free(data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	ShaderProgramSrc  source = PraserShader("res/Shaders/Shader.shader");

	unsigned int shader = CreateShader(source.VertexSrc, source.FragmentSrc);

	while (!glfwWindowShouldClose(window))
	{	
		processInput(window);
		 
		glClearColor (0.0f , 0.0f , 0.0f , 1.0f);

		glClear (GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUseProgram(shader);
		
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 1.0f ) + 0.1;
		float redValue = (sin(timeValue) / 2.0f) + 0.4;
		float blueValue = (sin(timeValue) / 3.0f) + 0.5;

		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.1f, -0.1f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		int vertexColorLocation = glGetUniformLocation(shader , "ourColor");
		unsigned int transformLoc = glGetUniformLocation(shader, "transform");

		glUniform4f(vertexColorLocation , redValue ,greenValue, blueValue , 1.0f);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		

		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1 , &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	std::cout << "here 2 \n";
	unsigned int program = glCreateProgram ();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER , vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER , fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

static unsigned int CompileShader( unsigned int type , const std::string& source)
{	
	std::cout << "here 1\n";

	int result;

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	glGetShaderiv(id , GL_COMPILE_STATUS , &result);

	
	if (!result)
	{
		int lenght;

		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);

		char* message = (char*)alloca(lenght * sizeof(char));

		glGetShaderInfoLog(id , lenght , &lenght , message);

		std::cout << "Failed to complie" <<
			(type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGEMENT")
			<< "shader" << std::endl;

		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;

	}
	

	return id;
}

static ShaderProgramSrc PraserShader(const std::string& file)
{
	std::ifstream stream(file);

	std::string line;

	std::stringstream ss[2];

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0 , FRAGEMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}

			if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGEMENT;
			}
		}

		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	
	return { ss[0].str() , ss[1].str() };
}