/*
	9. Considere um triângulo cujos vértices possuem, respectivamente, as
	cores vermelho, verde e azul.
		a) Descreva uma possível configuração dos buffers (VBO, VAO) para representá-lo.
		R: Primeiramente precisamos alterar o buffer para conter as informações de RGB, tendo então 6 valores por linha (x, y, z e r, g, b).
		Atualizar o delocamento do glVertexAttribPointer de 3 para 6 elementos e conferir se castou o inicio para o tipo certo.

		b) Como estes atributos seriam identificados no vertex shader?
		R:
		Através das palavras in e out, passando assim a informação do vertex para o fragment
		
		c) Agora implemente!
 */

using namespace std;

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Ex9.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 800;

const GLchar *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec3 position;
 layout (location = 1) in vec3 color;
 out vec3 vColor;
 void main()
 {
	vColor = color;
	gl_Position = vec4(position, 1.0);
 }
 )";

const GLchar *fragmentShaderSource = R"(
 #version 400
 in vec3 vColor;
 out vec4 color;
 void main()
 {
	 color = vec4(vColor,1.0);
 }
 )";

int main()
{
	glfwInit();


	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Falha ao criar a janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}

	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLuint shaderID = setupShader();

	GLuint VAO = setupGeometry();


	glUseProgram(shaderID);

	double prev_s = glfwGetTime();
	double title_countdown_s = 0.1;

	float colorValue = 0.0;
	
	while (!glfwWindowShouldClose(window))
	{
        CheckFrameRate(prev_s, title_countdown_s, window);

        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glBindVertexArray(VAO);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);   
		
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void CheckFrameRate(double &prev_s, double &title_countdown_s, GLFWwindow *window)
{
    {
        double curr_s = glfwGetTime();
        double elapsed_s = curr_s - prev_s;
        prev_s = curr_s;

        title_countdown_s -= elapsed_s;
        if (title_countdown_s <= 0.0 && elapsed_s > 0.0)
        {
            double fps = 1.0 / elapsed_s;

            char tmp[256];
            sprintf(tmp, "Ola Triangulo! -- Johnny\tFPS %.2lf", fps);
            glfwSetWindowTitle(window, tmp);

            title_countdown_s = 0.1;
        }
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupGeometry()
{

	
	GLfloat vertices[] = {
		// x   y     z    r   g    b
		// T0
		-0.5, -0.5, 0.0, 1.0, 0.0, 0.0,  // v0
		 0.5, -0.5, 0.0, 0.0, 1.0, 0.0,  // v1
		 0.0,  0.5, 0.0, 0.0, 0.0, 1.0,	 // v2
	};

	GLuint VBO, VAO;
	
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}
