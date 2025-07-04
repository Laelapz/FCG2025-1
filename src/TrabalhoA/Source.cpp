﻿/*
 * Hello Triangle - Código adaptado de:
 *   - https://learnopengl.com/#!Getting-started/Hello-Triangle
 *   - https://antongerdelan.net/opengl/glcontext2.html
 *
 * Adaptado por: Rossana Baptista Queiroz
 *
 * Disciplinas:
 *   - Processamento Gráfico (Ciência da Computação - Híbrido)
 *   - Processamento Gráfico: Fundamentos (Ciência da Computação - Presencial)
 *   - Fundamentos de Computação Gráfica (Jogos Digitais)
 *
 * Descrição:
 *   Este código é o "Olá Mundo" da Computação Gráfica, utilizando OpenGL Moderna.
 *   No pipeline programável, o desenvolvedor pode implementar as etapas de
 *   Processamento de Geometria e Processamento de Pixel utilizando shaders.
 *   Um programa de shader precisa ter, obrigatoriamente, um Vertex Shader e um Fragment Shader,
 *   enquanto outros shaders, como o de geometria, são opcionais.
 *
 * Histórico:
 *   - Versão inicial: 07/04/2017
 *   - Última atualização: 18/03/2025
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Sprite 
{
	GLuint VAO;
	GLuint texID;
	vec3 pos;
	vec3 dimensions;
	float angle;
	float vel; 
	int nAnimations, nFrames;
	int iFrame, iAnimation;
	float ds, dt;

	vec3 position;
	vec3 dimension;
    bool colidiu;
    bool ativo;
};

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupSprite(int nAnimations, int nFrames, float &ds, float &dt);
int loadTexture(string filePath);
void drawSprite(GLuint shaderID, Sprite spr);
bool CheckCollision(Sprite& one, Sprite& two);


// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec2 position;
 layout (location = 1) in vec2 texc;

 uniform mat4 projection;
 uniform mat4 model;

 out vec2 tex_coord;
 void main()
 {
	tex_coord = vec2(texc.s, 1.0-texc.t);
	gl_Position = projection * model * vec4(position, 0.0, 1.0);
 }
 )";

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = R"(
 #version 400
 in vec2 tex_coord;
 out vec4 color;
 uniform sampler2D tex_buff;
 uniform vec2 offset_tex;
 void main()
 {
	 color = texture(tex_buff,tex_coord + offset_tex);
 }
 )";

bool keys[1024];
float FPS = 12.0;
float lastTime = 0.0;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 8);

	
	for(int i=0; i<1024;i++) { keys[i] = false; }

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Falha ao criar a janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	Sprite background, spr1, spr2;

	background.VAO = setupSprite(1,1,background.ds,background.dt);
	background.texID = loadTexture("../assets/tex/1.png");
	background.pos = vec3(400,300,0);
	background.dimensions = vec3(800, 600, 1);
	background.angle = 0.0;

	// Carregando uma textura
	spr1.VAO = setupSprite(12,2,spr1.ds,spr1.dt);
	spr1.texID = loadTexture("../assets/sprites/enemies-spritesheet1.png");
	spr1.pos = vec3(400,300,0);
	spr1.dimensions = vec3(20 * 4, 20 * 4, 1);
	spr1.vel = 1.5;
	spr1.nAnimations = 12;
	spr1.nFrames = 2;
	spr1.angle = 0.0;
	spr1.iAnimation = 9;
	spr1.iFrame = 0;
	spr1.dimension = vec3(-20.0*4, -20.0*4, 1);
	spr1.ativo = true;

	spr2.VAO = setupSprite(1, 1, spr2.ds, spr2.dt);
	spr2.texID = loadTexture("../assets/sprites/microbio.png");
	spr2.pos = vec3(200.0, 300.0, 0.0);
	spr2.dimensions = vec3(32 * 4, 26 * 4, 1);
	spr2.vel = 1.0;
	spr2.angle = 0.0;
	spr1.dimension = vec3(-32.0*4, -26.0*4, 1);
	spr2.ativo = true;

	glUseProgram(shaderID); // Reseta o estado do shader para evitar problemas futuros

	double prev_s = glfwGetTime();	// Define o "tempo anterior" inicial.
	double title_countdown_s = 0.1; // Intervalo para atualizar o título da janela com o FPS.

	float colorValue = 0.0;

	// Ativando o primeiro buffer de textura do OpenGL
	glActiveTexture(GL_TEXTURE0);

	// Criando a variável uniform pra mandar a textura pro shader
	glUniform1i(glGetUniformLocation(shaderID, "tex_buff"), 0);

	// Criação da matriz de projeção paralela ortográfica
	mat4 projection = mat4(1); //matriz identidade
	projection = ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	
	//Envio para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

	//Criação de matriz de transformação do objeto
	mat4 model = mat4(1);
	model = translate(model, vec3(400, 300, 0));
	model = scale(model, vec3(32, 26, 1));

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

	//Habilitando transparência/função de mistura
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Habilitando teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Este trecho de código é totalmente opcional: calcula e mostra a contagem do FPS na barra de título
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

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (keys[GLFW_KEY_LEFT] == true || keys[GLFW_KEY_A] == true)
		{
			spr1.pos.x -= spr1.vel;		
		}
		if (keys[GLFW_KEY_RIGHT] == true || keys[GLFW_KEY_D] == true)
		{
			spr1.pos.x += spr1.vel;		
		}

		glUniform2f(glGetUniformLocation(shaderID, "offset_tex"),0.0,0.0);
		drawSprite(shaderID, background);

		if (CheckCollision(spr2, spr1) == true)
		{
			cout<<"\nColidiu"<<endl;
			spr1.ativo = false;
		}

		if(spr1.ativo)
		{
			float offsetS = spr1.iFrame * spr1.ds;
			float offsetT = spr1.iAnimation * spr1.dt;
			glUniform2f(glGetUniformLocation(shaderID, "offset_tex"),offsetS,offsetT);
			drawSprite(shaderID, spr1);

			float now = glfwGetTime();
			float deltaTime = now - lastTime;
			if (deltaTime >= 1/FPS)
			{
				spr1.iFrame = (spr1.iFrame + 1) % spr1.nFrames;
				lastTime = now;
			}
		}
		
		glUniform2f(glGetUniformLocation(shaderID, "offset_tex"),0.0,0.0);
		drawSprite(shaderID, spr2);

		

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no iniçio deste arquivo
//  A função retorna o identificador do programa de shader
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

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupSprite(int nAnimations, int nFrames, float &ds, float &dt)
{

	ds = 1.0 / (float) nFrames;
	dt = 1.0 / (float) nAnimations;

	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		// x   y   s     t
		-0.5, 0.5, 0.0, dt,
		-0.5,-0.5, 0.0,	0.0,
		 0.5, 0.5, ds, dt,
		-0.5,-0.5, 0.0,	0.0,
		 0.5,-0.5, ds, 0.0,
		 0.5, 0.5, ds,	dt,
	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	// Ponteiro pro atributo 0 - Posição - coordenadas x, y
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Ponteiro pro atributo 2 - Coordenada de textura - coordenadas s,t
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void drawSprite(GLuint shaderID, Sprite spr)
{
	// Neste código, usamos o mesmo buffer de geomtria para todos os sprites
	glBindVertexArray(spr.VAO);				 // Conectando ao buffer de geometria
		
	glBindTexture(GL_TEXTURE_2D, spr.texID); // Conectando ao buffer de textura
	// Criação da  matriz de transformações do objeto
	mat4 model = mat4(1);  // matriz identidade
	model = translate(model, spr.pos);
	model = rotate(model, radians(spr.angle),vec3(0.0,0.0,1.0));
	model = scale(model, spr.dimensions);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));
	// Chamada de desenho - drawcall
	// Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0); // Desnecessário aqui, pois não há múltiplos VAOs
}

int loadTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;

	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

bool CheckCollision(Sprite& one, Sprite& two)
{
    bool collisionX = abs(one.pos.x - two.pos.x) < (one.dimension.x / 2.0f + two.dimension.x / 2.0f);
    bool collisionY = abs(one.pos.y - two.pos.y) < (one.dimension.y / 2.0f + two.dimension.y / 2.0f);

    return collisionX && collisionY;
}