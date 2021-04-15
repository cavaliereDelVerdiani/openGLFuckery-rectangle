#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"ShaderClass.h"

void resizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	//inizializza roba di glfw
	if (!glfwInit()) {
		//se inizializza di merda glfw almeno sapremo dov'rera il problema
		std::cout << "failed to initailize GLFW\n";
		glfwTerminate();//elimina tutta la roba inizializzata da glfw
		return 0;
	}
	
	//setta versione
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//non ho idea di che cazzo sia sta sintassi, ma fa fare la roba in mezzo solo se sei su macOS, qiundi serve
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//ora che glfw è settato possiamo creare una finestra
	GLFWwindow* window = glfwCreateWindow(640, 480, "triangolo", NULL, NULL);
	if (!window) {
		//se inizializza di merda la finestra almeno sparemo che è andato storto
		std::cout << "failed to initailize window\n";
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, resizeCallback);
	//ora che abbiamo un contesto ci possiamo inizializzare sopra la roba di opengl, che è gestita tutta da glad quindi quello
	
	//GLADloadproc a = (GLADloadproc)glfwGetProcAddress;
	//if (!gladLoadGLLoader(a)){
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		//glfwGetProcAdress->dove sono le funzioni di openGL da cosare con glad
		//lo castiamo a qualcosa che glad sa leggere(GLADloadproc)
		//e dato questo alla funzione gladLoadGLLoader, vengono cosate le funzioni che ci servono
		//per capirci ancora meno: le due righe commentate sopra sarebbero un altro modo comunque valido di fare sta cosa
		
		//ora, dato il solito check
		std::cout << "inizializzazione glad fallita\n";
		glfwTerminate();
		return 0;
	}

	//settiamo ora, per far funzionare openGL decentemente, il viewPort
	glViewport(0, 0, 640, 480);

	float vertices[]{
		//vertice diviso in
		// x     y    z       r    g    b
		-0.5, -0.5, 1.0,    1.0, 0.0, 0.0,   //basso  sinistra
		 0.5, -0.5, 1.0,    0.0, 1.0, 0.0,   //basso  destra
		 0.5,  0.5, 1.0,    0.0, 0.0, 1.0,   //alto   destra
		-0.5,  0.5, 1.0,    1.0, 1.0, 0.0    //alto   sinistra
	};

	int indices[]{
		0,1,2, //primo triangolo
		0,2,3  //secondo triangolo
	};

	//crea e binda un vertex array object
	unsigned int vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	unsigned int vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);                                            //genera un vertex buffer object(vbo)
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);                               //prendi il vbo appena genertao e mettilo come corrente
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);       //modifica il vbo corrente

	unsigned int elementBufferObject;
	glGenBuffers(1, &elementBufferObject);                                           //genera un element buffer object(ebo)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);                      //prendi il ebo appena genertao e mettilo come corrente
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //modifica l'ebo corrente

	//ora usiamo i dati che abbiamo per mettere belli e tutto gli attributi
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)0);     //attributo 0, posizione, e gli diamo la posizione
	glEnableVertexAttribArray(0);                                                    //abilita l'attributo 0

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)(sizeof(float) * 3)); //attributo 1, colore
	glEnableVertexAttribArray(1);                                                                  //abilita l'attributo 1

	//ora possiamo unbindare il vao
	//glBindVertexArray(0);

	//crea oggetto shader program
	program prog("vertex shader text.txt", "fragment shader text.txt");

	//settiamo il colore dello sfondo a un viola scuro, mi piace
	glClearColor(0.25, 0.0, 0.5, 0.0);
	prog.use();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		//bindiamo e disegnamo il vertex array object
		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &elementBufferObject);
	glDeleteVertexArrays(1, &vertexArrayObject);
	std::cout << "\nanyway here's Wonderwall\n";
	std::cin.ignore(1);
	return 0;
}