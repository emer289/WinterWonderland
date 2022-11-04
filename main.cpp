#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void bounce();

//bouncing snowman varible
float position = 0.0;
int state = 1;

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
   
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);


    glEnable(GL_DEPTH_TEST);

   //shaders
    Shader snowmanShader("1.11model_loading.vs", "1.model_loading.fs");
    Shader mountainShader("1.model_loading.vs", "1.model_loading.fs");
    Shader wavingShowmanShader("anim_model.vs", "anim_model.fs");
   
   //models
    Model snowManModel("Objects/SnowManBigArms/snowmanBigArms.obj");
    Model mountainModel("Objects/mountain/snowMountainTexture.obj");
    Model hatModel("Objects/Hat/hat.obj");

	Model wavingSnowmanModel("Objects/SarahTheSnowMan/snowman.dae");
	Animation wavingSnowmanAnimation("Objects/SarahTheSnowMan/snowman.dae",&wavingSnowmanModel);
	Animator wavingSnowmanAnimator(&wavingSnowmanAnimation);

    

    //draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        bounce();

        processInput(window);
                
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//// bouncing waving snowman start
        wavingSnowmanAnimator.UpdateAnimation(deltaTime);

        wavingShowmanShader.use();  
        glm::mat4 wsTransform = glm::mat4(1.0f); 
        wsTransform = glm::translate(wsTransform, glm::vec3(0.0f, 0.0f, 0.0f));
	    unsigned int wsTransformLoc = glGetUniformLocation(wavingShowmanShader.ID, "transform");
        glUniformMatrix4fv(wsTransformLoc, 1, GL_FALSE, glm::value_ptr(wsTransform));

		glm::mat4 wsProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 wsView = camera.GetViewMatrix();
		wavingShowmanShader.setMat4("projection", wsProjection);
		wavingShowmanShader.setMat4("view", wsView);

        auto wsTransforms = wavingSnowmanAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < wsTransforms.size(); ++i)
			wavingShowmanShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", wsTransforms[i]);


		// render the loaded model
		glm::mat4 wsModel = glm::mat4(1.0f);
		wsModel = glm::translate(wsModel, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
		wsModel = glm::scale(wsModel, glm::vec3(.25f, .25f, .25f));	// it's a bit too big for our scene, so scale it down
		wavingShowmanShader.setMat4("model", wsModel);
		wavingSnowmanModel.Draw(wavingShowmanShader);

//// bouncing waving snowman end

//// snowman with big arms start

        snowmanShader.use();
        glm::mat4 sTransform = glm::mat4(1.0f); 
        sTransform = glm::translate(sTransform, glm::vec3(position, 0.0f, 0.0f));
        unsigned int sTransformLoc = glGetUniformLocation(snowmanShader.ID, "transform");
        glUniformMatrix4fv(sTransformLoc, 1, GL_FALSE, glm::value_ptr(sTransform));

        // view/projection transformations
        glm::mat4 sProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 sView = camera.GetViewMatrix();
        snowmanShader.setMat4("projection", sProjection);
        snowmanShader.setMat4("view", sView);

        // render the loaded model
        glm::mat4 sModel = glm::mat4(1.0f);
        sModel = glm::translate(sModel, glm::vec3(0.5f, 0.5f, 0.0f)); // translate it down so it's at the center of the scene
        sModel = glm::scale(sModel, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        snowmanShader.setMat4("model", sModel);
       // snowManModel.Draw(snowmanShader);
        
//// snowman with big arms end

//// mountain start  
        // don't forget to enable shader before setting uniforms
        mountainShader.use();

        // view/projection transformations
        glm::mat4 mProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 mView = camera.GetViewMatrix();
        mountainShader.setMat4("projection", mProjection);
        mountainShader.setMat4("view", mView);

        // render the loaded model
        glm::mat4 mModel = glm::mat4(1.0f);
        mModel = glm::translate(mModel, glm::vec3(-0.9f, -0.9f, 0.0f)); // translate it down so it's at the center of the scene
        mModel = glm::scale(mModel, glm::vec3(3.0f, 3.0f, 3.0f));	// it's a bit too big for our scene, so scale it down
        mountainShader.setMat4("model", mModel);
        
        //mountainModel.Draw(mountainShader);

//// mountain end

//// hat start  
       
        glm::mat4 hProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 hView = camera.GetViewMatrix();
        mountainShader.setMat4("projection", hProjection);
        mountainShader.setMat4("view", hView);

        glm::mat4 hModel = glm::mat4(1.0f);
        hModel = glm::translate(hModel, glm::vec3(0.0f, 0.2f, 0.0f)); 
        hModel = glm::scale(hModel, glm::vec3(0.25f, 0.25f, 0.25f));	
        mountainShader.setMat4("model", hModel);
        
        hatModel.Draw(mountainShader);

//// hat end
        
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
void processInput(GLFWwindow *window)
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void bounce(){

     switch(state)
    {
        case 1:
            if(position<.3)
                position+=0.005;
            else    
                state=-1;
            break;
        case -1:
            if(position>=0.0)
                position-=0.005;
            else
                state=1;
            break;

    }
}


// #define STB_IMAGE_IMPLEMENTATION

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include <learnopengl/filesystem.h>
// #include <learnopengl/shader_m.h>
// #include <learnopengl/camera.h>
// #include <learnopengl/model.h>
// #include <learnopengl/entity.h>

// #ifndef ENTITY_H
// #define ENTITY_H

// #include <list> //std::list
// #include <memory> //std::unique_ptr

// class Entity : public Model
// {
// public:
// 	list<unique_ptr<Entity>> children;
// 	Entity* parent;
// };
// #endif


// #include <iostream>

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void processInput(GLFWwindow* window);

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
// 	// glfw: initialize and configure
// 	// ------------------------------
// 	glfwInit();
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
// 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

// 	// glfw window creation
// 	// --------------------
// 	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
// 	if (window == NULL)
// 	{
// 		std::cout << "Failed to create GLFW window" << std::endl;
// 		glfwTerminate();
// 		return -1;
// 	}
// 	glfwMakeContextCurrent(window);
// 	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
// 	glfwSetCursorPosCallback(window, mouse_callback);
// 	glfwSetScrollCallback(window, scroll_callback);

// 	// tell GLFW to capture our mouse
// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

// 	// glad: load all OpenGL function pointers
// 	// ---------------------------------------
// 	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
// 	{
// 		std::cout << "Failed to initialize GLAD" << std::endl;
// 		return -1;
// 	}

// 	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
// 	stbi_set_flip_vertically_on_load(true);

// 	// configure global opengl state
// 	// -----------------------------
// 	glEnable(GL_DEPTH_TEST);

// 	// build and compile shaders
// 	// -------------------------
// 	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

// 	// load entities
// 	// -----------
// 	Model model = Model("Objects/planet/planet.obj");
//     Model modelConeHouse = Model("Objects/ConeHouse/coneHouse.obj");
// 	Entity ourEntity(model);
// 	ourEntity.transform.setLocalPosition({ 0, 0, 0 });
// 	const float scale = 0.75;
// 	ourEntity.transform.setLocalScale({ scale, scale, scale });

// 	{
// 		Entity* lastEntity = &ourEntity;

// 		for (unsigned int i = 0; i < 2; ++i)
// 		{
// 			lastEntity->addChild(modelConeHouse);
// 			lastEntity = lastEntity->children.back().get();

// 			//Set tranform values
// 			lastEntity->transform.setLocalPosition({ 4, 0, 0 });
// 			lastEntity->transform.setLocalScale({ scale, scale, scale });
// 		}

// 	}
// 	ourEntity.updateSelfAndChild();

// 	// draw in wireframe
// 	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// 	// render loop
// 	// -----------
// 	while (!glfwWindowShouldClose(window))
// 	{
// 		// per-frame time logic
// 		// --------------------
// 		float currentFrame = glfwGetTime();
// 		deltaTime = currentFrame - lastFrame;
// 		lastFrame = currentFrame;

// 		// input
// 		// -----
// 		processInput(window);

// 		// render
// 		// ------
// 		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		// don't forget to enable shader before setting uniforms
// 		ourShader.use();

// 		// view/projection transformations
// 		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
// 		glm::mat4 view = camera.GetViewMatrix();
// 		ourShader.setMat4("projection", projection);
// 		ourShader.setMat4("view", view);

// 		// draw our scene graph
// 		Entity* lastEntity = &ourEntity;
// 		while (lastEntity->children.size())
// 		{
// 			ourShader.setMat4("model", lastEntity->transform.getModelMatrix());
// 			lastEntity->pModel->Draw(ourShader);
// 			lastEntity = lastEntity->children.back().get();
//             lastEntity->transform.setLocalRotation({  ourEntity.transform.getLocalRotation().y + 20 * deltaTime, 0.f, 0.f });
// 		}

        
// 		ourEntity.transform.setLocalRotation({ 0.f, ourEntity.transform.getLocalRotation().y + 20 * deltaTime, 0.f });
// 		ourEntity.updateSelfAndChild();

        

// 		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
// 		// -------------------------------------------------------------------------------
// 		glfwSwapBuffers(window);
// 		glfwPollEvents();
// 	}

// 	// glfw: terminate, clearing all previously allocated GLFW resources.
// 	// ------------------------------------------------------------------
// 	glfwTerminate();
// 	return 0;
// }

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// // ---------------------------------------------------------------------------------------------------------
// void processInput(GLFWwindow* window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);

// 	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
// 		camera.ProcessKeyboard(FORWARD, deltaTime);
// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
// 		camera.ProcessKeyboard(BACKWARD, deltaTime);
// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
// 		camera.ProcessKeyboard(LEFT, deltaTime);
// 	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
// 		camera.ProcessKeyboard(RIGHT, deltaTime);
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// // ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
// 	// make sure the viewport matches the new window dimensions; note that width and 
// 	// height will be significantly larger than specified on retina displays.
// 	glViewport(0, 0, width, height);
// }

// // glfw: whenever the mouse moves, this callback is called
// // -------------------------------------------------------
// void mouse_callback(GLFWwindow* window, double xpos, double ypos)
// {
// 	if (firstMouse)
// 	{
// 		lastX = xpos;
// 		lastY = ypos;
// 		firstMouse = false;
// 	}

// 	float xoffset = xpos - lastX;
// 	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

// 	lastX = xpos;
// 	lastY = ypos;

// 	camera.ProcessMouseMovement(xoffset, yoffset);
// }

// // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// // ----------------------------------------------------------------------
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
// 	camera.ProcessMouseScroll(yoffset);
// }