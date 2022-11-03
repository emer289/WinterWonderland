// #define STB_IMAGE_IMPLEMENTATION
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include <learnopengl/filesystem.h>
// #include <learnopengl/shader_m.h>
// #include <learnopengl/camera.h>
// #include <learnopengl/animator.h>
// #include <learnopengl/model_animation.h>

// #include <iostream>

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void processInput(GLFWwindow *window);
// void bounce();

// float y_position = 0.0;
// int state = 1;

// // settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// // camera
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// float lastX = SCR_WIDTH / 2.0f;
// float lastY = SCR_HEIGHT / 2.0f;
// bool firstMouse = true;

// // timing
// float deltaTime = 0.0f;
// float lastFrame = 0.0f;

// int main()
// {
//     // glfw: initialize and configure
//     // ------------------------------
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     // glfw window creation
//     // --------------------
//     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//     glfwSetCursorPosCallback(window, mouse_callback);
//     glfwSetScrollCallback(window, scroll_callback);

//     // tell GLFW to capture our mouse
//     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

//     // glad: load all OpenGL function pointers
//     // ---------------------------------------
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//     stbi_set_flip_vertically_on_load(true);

//     // configure global opengl state
//     // -----------------------------
//     glEnable(GL_DEPTH_TEST);

//     // build and compile shaders
//     // -------------------------
//     Shader ourShader("1.11model_loading.vs", "1.model_loading.fs");
//     Shader ourMountainShader("1.model_loading.vs", "1.model_loading.fs");

//     // load models
//     // -----------
//     Model ourModel("Objects/SnowManBigArms/snowmanBigArms.obj");
//     Model ourMountain("Objects/mountain/snowMountainTexture.obj");

//     Shader ourShaderAnim("anim_model.vs", "anim_model.fs");
//     // load models
// 	// -----------
// 	Model ourModelAnim("Objects/SarahTheSnowMan/snowman.dae");
// 	Animation danceAnimation("Objects/SarahTheSnowMan/snowman.dae",&ourModelAnim);
// 	Animator animator(&danceAnimation);

    

//     // draw in wireframe
//     //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//     // render loop
//     // -----------
//     while (!glfwWindowShouldClose(window))
//     {
//         // per-frame time logic
//         // --------------------
//         float currentFrame = glfwGetTime();
//         deltaTime = currentFrame - lastFrame;
//         lastFrame = currentFrame;

//         bounce();

//         // input
//         // -----
//         processInput(window);
//         animator.UpdateAnimation(deltaTime);
// //// bouncing snow man
//         // render
//         // ------
//         glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         // create transformations
//         glm::mat4 transformAnim = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//         transformAnim = glm::translate(transformAnim, glm::vec3(0.0f, y_position, 0.0f));

//         // don't forget to enable shader before setting uniforms
// 		ourShaderAnim.use();
//         unsigned int transformLocAnim = glGetUniformLocation(ourShaderAnim.ID, "transform");
//         glUniformMatrix4fv(transformLocAnim, 1, GL_FALSE, glm::value_ptr(transformAnim));

// 		// view/projection transformations
// 		glm::mat4 projectionAnim = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
// 		glm::mat4 viewAnim = camera.GetViewMatrix();
// 		ourShaderAnim.setMat4("projection", projectionAnim);
// 		ourShaderAnim.setMat4("view", viewAnim);

//         auto transformsAnim = animator.GetFinalBoneMatrices();
// 		for (int i = 0; i < transformsAnim.size(); ++i)
// 			ourShaderAnim.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transformsAnim[i]);


// 		// render the loaded model
// 		glm::mat4 modelAnim = glm::mat4(1.0f);
// 		modelAnim = glm::translate(modelAnim, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
// 		modelAnim = glm::scale(modelAnim, glm::vec3(.25f, .25f, .25f));	// it's a bit too big for our scene, so scale it down
// 		ourShaderAnim.setMat4("model", modelAnim);
// 		ourModelAnim.Draw(ourShaderAnim);
// //// bouncing snow man

// ///

//         // create transformations
//         glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//         transform = glm::translate(transform, glm::vec3(y_position, 0.0f, 0.0f));
//        // transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

//         // don't forget to enable shader before setting uniforms
//         ourShader.use();
//         unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
//         glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

//         // view/projection transformations
//         glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//         glm::mat4 view = camera.GetViewMatrix();
//         ourShader.setMat4("projection", projection);
//         ourShader.setMat4("view", view);

//         // render the loaded model
//         glm::mat4 model = glm::mat4(1.0f);
//         model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f)); // translate it down so it's at the center of the scene
//         model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
//         ourShader.setMat4("model", model);
//        // ourModel.Draw(ourShader);
        

// /////mountain 
//         // don't forget to enable shader before setting uniforms
//         ourMountainShader.use();

//         // view/projection transformations
//         glm::mat4 projectionM = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//         glm::mat4 viewM = camera.GetViewMatrix();
//         ourMountainShader.setMat4("projection", projectionM);
//         ourMountainShader.setMat4("view", viewM);

//         // render the loaded model
//         glm::mat4 modelM = glm::mat4(1.0f);
//         modelM = glm::translate(modelM, glm::vec3(-0.9f, -0.9f, 0.0f)); // translate it down so it's at the center of the scene
//         modelM = glm::scale(modelM, glm::vec3(3.0f, 3.0f, 3.0f));	// it's a bit too big for our scene, so scale it down
//         ourMountainShader.setMat4("model", modelM);
        
//         ourMountain.Draw(ourMountainShader);
        



//         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//         // -------------------------------------------------------------------------------
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     // glfw: terminate, clearing all previously allocated GLFW resources.
//     // ------------------------------------------------------------------
//     glfwTerminate();
//     return 0;
// }

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// // ---------------------------------------------------------------------------------------------------------
// void processInput(GLFWwindow *window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         camera.ProcessKeyboard(FORWARD, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         camera.ProcessKeyboard(BACKWARD, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         camera.ProcessKeyboard(LEFT, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         camera.ProcessKeyboard(RIGHT, deltaTime);
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// // ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
//     // make sure the viewport matches the new window dimensions; note that width and 
//     // height will be significantly larger than specified on retina displays.
//     glViewport(0, 0, width, height);
// }

// // glfw: whenever the mouse moves, this callback is called
// // -------------------------------------------------------
// void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
// {
//     float xpos = static_cast<float>(xposIn);
//     float ypos = static_cast<float>(yposIn);

//     if (firstMouse)
//     {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }

//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

//     lastX = xpos;
//     lastY = ypos;

//     camera.ProcessMouseMovement(xoffset, yoffset);
// }

// // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// // ----------------------------------------------------------------------
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     camera.ProcessMouseScroll(static_cast<float>(yoffset));
// }

// void bounce(){

//      switch(state)
//     {
//         case 1:
//             if(y_position<.3)
//                 y_position+=0.005;
//             else    
//                 state=-1;
//             break;
//         case -1:
//             if(y_position>=0.0)
//                 y_position-=0.005;
//             else
//                 state=1;
//             break;

//     }
// }
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <learnopengl/entity.h>

#ifndef ENTITY_H
#define ENTITY_H

#include <list> //std::list
#include <memory> //std::unique_ptr

class Entity : public Model
{
public:
	list<unique_ptr<Entity>> children;
	Entity* parent;
};
#endif


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

	// load entities
	// -----------
	Model model = Model("Objects/planet/planet.obj");
    Model modelConeHouse = Model("Objects/ConeHouse/coneHouse.obj");
	Entity ourEntity(model);
	ourEntity.transform.setLocalPosition({ 0, 0, 0 });
	const float scale = 0.75;
	ourEntity.transform.setLocalScale({ scale, scale, scale });

	{
		Entity* lastEntity = &ourEntity;

		for (unsigned int i = 0; i < 2; ++i)
		{
			lastEntity->addChild(modelConeHouse);
			lastEntity = lastEntity->children.back().get();

			//Set tranform values
			lastEntity->transform.setLocalPosition({ 4, 0, 0 });
			lastEntity->transform.setLocalScale({ scale, scale, scale });
		}

	}
	ourEntity.updateSelfAndChild();

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// draw our scene graph
		Entity* lastEntity = &ourEntity;
		while (lastEntity->children.size())
		{
			ourShader.setMat4("model", lastEntity->transform.getModelMatrix());
			lastEntity->pModel->Draw(ourShader);
			lastEntity = lastEntity->children.back().get();
            lastEntity->transform.setLocalRotation({  ourEntity.transform.getLocalRotation().y + 20 * deltaTime, 0.f, 0.f });
		}

        
		ourEntity.transform.setLocalRotation({ 0.f, ourEntity.transform.getLocalRotation().y + 20 * deltaTime, 0.f });
		ourEntity.updateSelfAndChild();

        

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}