/*
1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão
atual que trabalhamos em aula e o que eles processam?
R: 
GLSL(OpenGL Shading Language) é uma linguagem de programação, feita com o OpenGL para desenvolver shaders.

Vertex Shader e Fragment Shader
- Vertex Shader -> Processa cada vértice separadamente
- Frafment Shader -> Processa cada fragment separadamente

-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   
2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?
R:
São elementos básicos dos quais são construídos outros objetos mais complexos
Dentro de VBO's que compõem um VAO.

-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   
3. Explique o que é VBO e VAO, e como se relacionam (se achar mais fácil, pode fazer um gráfico
representando a relação entre eles).
- Vertex Buffer Object(VBO) serve para armazenar os dados necessários do vértice de forma eficiente, p-odendo ser rapidamente transferido para a gpu acessar rapidamente.
- Vertex Array Object(VAO) é como um container para multiplos VBOs. Serve justamente para comunicar com o OPENGL como usar os dados de um VBO.

-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   
4. Como são feitas as chamadas de desenho na OpenGL? Para que servem as primitivas de desenho?
R:
As chamadas são feitas via contexto, ou seja o OpenGL começa a entender oq queremos desenhar de acordo com as funções que chamamos. Por fim chamamos a função de desenho 
dentro do loop. As primitivas de desenho são formas predefinidas para criar geometrias diferentes.

*/