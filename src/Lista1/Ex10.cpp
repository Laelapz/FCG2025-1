using namespace std;
 #include <iostream>
 #include <string>
 #include <assert.h>
#include <vector>
 #include <cmath>
 #include <glad/glad.h>
 #include <GLFW/glfw3.h>
 
const float PI = 3.14159;

 void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode); 
 int setupShader();
 int setupGeometry();
 int createCircle(int nPoints, float radius = 0.5, float posX = 0, float posY = 0);
 
 const GLuint WIDTH = 800, HEIGHT = 800;
 
 const GLchar *vertexShaderSource = R"(
  #version 400
  layout (location = 0) in vec3 position;
  void main()
  {
	  gl_Position = vec4(position.x, position.y, position.z, 1.0);
  }
  )";
 
 const GLchar *fragmentShaderSource = R"(
  #version 400
  uniform vec4 inputColor;
  out vec4 color;
  void main()
  {
	  color = inputColor;
  }
  )";
 
 // Função MAIN
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
 
	 // GLAD: carrega todos os ponteiros d funções da OpenGL
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
 
	 int nPoints = 15;

	 GLuint shaderID = setupShader();
	 GLuint VAO1 = setupGeometry();
	 GLuint VAO2 = createCircle(nPoints, .5, 0, .8);
	 GLuint VAO3 = createCircle(nPoints, .2, .3, 0);
	 GLuint VAO4 = createCircle(nPoints, .2, .2, .5);
	 GLuint VAO5 = createCircle(nPoints, .2, -.3, .65);
	 GLuint VAO6 = createCircle(nPoints, .15, -0, 0);

	int nVertices = nPoints + 2;
 
	 GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
 
	 glUseProgram(shaderID); // Reseta o estado do shader para evitar problemas futuros
 
	 double prev_s = glfwGetTime();	// Define o "tempo anterior" inicial.
	 double title_countdown_s = 0.1; // Intervalo para atualizar o título da janela com o FPS.
 
	 float colorValue = 0.0;
	 // Loop da aplicação - "game loop"
	 while (!glfwWindowShouldClose(window))
	 {
		 {
			 double curr_s = glfwGetTime();		// Obtém o tempo atual.
			 double elapsed_s = curr_s - prev_s; // Calcula o tempo decorrido desde o último frame.
			 prev_s = curr_s;					// Atualiza o "tempo anterior" para o próximo frame.
 
			 // Exibe o FPS, mas não a cada frame, para evitar oscilações excessivas.
			 title_countdown_s -= elapsed_s;
			 if (title_countdown_s <= 0.0 && elapsed_s > 0.0)
			 {
				 double fps = 1.0 / elapsed_s; // Calcula o FPS com base no tempo decorrido.
 
				 // Cria uma string e define o FPS como título da janela.
				 char tmp[256];
				 sprintf(tmp, "Ola Triangulo! -- Rossana\tFPS %.2lf", fps);
				 glfwSetWindowTitle(window, tmp);
 
				 title_countdown_s = 0.1; // Reinicia o temporizador para atualizar o título periodicamente.
			 }
		 }
 
		 glfwPollEvents();
 
		 // Limpa o buffer de cor
		 glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		 glClear(GL_COLOR_BUFFER_BIT);
 
		 //Tronco da arvore
		 glBindVertexArray(VAO1); // Conectando ao buffer de geometria
		 glUniform4f(colorLoc, .65, 0.43f, 0.21f, 1.0f); // enviando cor para variável uniform inputColor
		 glDrawArrays(GL_TRIANGLES, 0, 3);
		 glDrawArrays(GL_TRIANGLES, 1, 4);

		 //Buraco na arvore
		 glBindVertexArray(VAO6); // Conectando ao buffer de geometria
		 glUniform4f(colorLoc, .55, 0.33f, 0.11f, 1.0f); // enviando cor para variável uniform inputColor
		 glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

		 //Copa da arvore
		 glBindVertexArray(VAO2); // Conectando ao buffer de geometria
		 glUniform4f(colorLoc, 0.0, 1.0f, 0.0f, 1.0f); // enviando cor para variável uniform inputColor
		 glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

		 
		 glBindVertexArray(VAO3); // Conectando ao buffer de geometria
		 glUniform4f(colorLoc, 0.0, 1.0f, 0.0f, 1.0f); // enviando cor para variável uniform inputColor
		 glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);
		 
		 glBindVertexArray(VAO4); // Conectando ao buffer de geometria
		 glUniform4f(colorLoc, 0.0, 1.0f, 0.0f, 1.0f); // enviando cor para variável uniform inputColor
		 glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

		 glBindVertexArray(VAO5); // Conectando ao buffer de geometria
		 glUniform4f(colorLoc, 0.0, 1.0f, 0.0f, 1.0f); // enviando cor para variável uniform inputColor
		 glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);
 

		 glfwSwapBuffers(window);
	 }
	 
	 glDeleteVertexArrays(1, &VAO1);
	 glDeleteVertexArrays(1, &VAO2);
	 glDeleteVertexArrays(1, &VAO3);
	 glDeleteVertexArrays(1, &VAO4);
	 glDeleteVertexArrays(1, &VAO5);
	 glDeleteVertexArrays(1, &VAO6);
	 
	 glfwTerminate();
	 return 0;
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
	 
	 GLuint shaderProgram = glCreateProgram();
	 glAttachShader(shaderProgram, vertexShader);
	 glAttachShader(shaderProgram, fragmentShader);
	 glLinkProgram(shaderProgram);
	 
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
		 // x   y     z
		 -0.15, -0.8, 0.0,
		 -0.15,  0.6, 0.0,
		  0.15, -0.8, 0.0,
		  0.15,  0.6, 0.0,
	 };
 
	 GLuint VBO, VAO;
	 
	 glGenBuffers(1, &VBO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	 glEnableVertexAttribArray(0);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);
 
	 return VAO;
 }

 int createCircle(int nPoints, float radius, float posX, float posY)
 {
	vector <GLfloat> vertices;

	float angle = 0.0;
	float slice = 2 * PI / (float)nPoints;

	vertices.push_back(posX); // Xc
	vertices.push_back(posY); // Yc
	vertices.push_back(0.0); // Zc

	for (int i = 0; i <= nPoints; i++)
	{
		float x = posX + radius * cos(angle);
		float y = posY + radius * sin(angle);
		float z = 0.0;

		
		vertices.push_back(x); // X
		vertices.push_back(y); // Y
		vertices.push_back(z); // Z

		angle += slice;
	}

	GLuint VBO, VAO;
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}
 