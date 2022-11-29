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
void processInput(GLFWwindow* window);
void updatey();
//void updatez();
void updatex();
void rotateModel();
void updatexCircle();
unsigned int loadTexture(const char *path);

unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool blinn = false;
bool blinnKeyPressed = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//translation co ord
float y_position = -0.15f;
float z_position = 0.0f;
float x_position = -1.f;
bool movingRight = true;
int state = 1;

float angle = 0.0;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// build and compile shaders
	// -------------------------
	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
    Shader ourShader2("anim_model.vs", "anim_model.fs");
    Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");
    Shader shader("1.advanced_lighting.vs", "1.advanced_lighting.fs");

        float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // load models
    // -----------
    Model ourModel("Objects/mountain/hmmounNOncolourAndR.obj");
   // Model ourModel("Objects/mountain/mountEverything.obj");
    Model ourModel2("Objects/SnowManBigArms/snowmanBigArms.obj");
    Model xmasTree("Objects/XmasTRee/xmasTreeWithoutdecorations.obj");
    Model houseModel("Objects/Obj/Stone House.obj");

    // load models
	// -----------
	Model ourModel3("Objects/flyingBirdWithHATT/BIRDHAT.dae");
	Animation danceAnimation("Objects/flyingBirdWithHATT/BIRDHAT.dae",&ourModel3);
	Animator animator(&danceAnimation);

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
   

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    
    // vector<std::string> faces
    // {
    //     "/Users/emermurphy/Documents/GitHub/LearnOpenGl/Textures/skybox/right.jpg",
    //     "/Users/emermurphy/Documents/GitHub/LearnOpenGl/Textures/skybox/left.jpg",
    //     "/Users/emermurphy/Documents/GitHub/LearnOpenGl/Textures/skybox/top.jpg",
    //     "/Users/emermurphy/Documents/GitHub/LearnOpenGl/Textures/skybox/bottom.jpg",
    //     "/Users/emermurphy/Documents/GitHub/LearnOpenGl/Textures/skybox/back.jpg",
    //     "/Users/emermurphy/Documents/GitHub/LearnOpenGl/Textures/skybox/front.jpg",
        
    // };
    vector<std::string> faces
    {
        "Textures/Forest/rightPosx.jpg",
        "Textures/Forest/leftNegx.jpg",
        "Textures/Forest/topPosy.jpg",
        "Textures/Forest/bottomNegy.jpg",
        "Textures/Forest/frontNegz.jpg",
        "Textures/Forest/backPosz.jpg",
        
        
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    ourShader.use();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    float planeVertices[] = {
        // positions            // normals         // texcoords
         0.64f, -0.58f,  1.24f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f, //a
        -0.05f, -0.58f,  1.24f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, //b
        -0.05f, -0.56f, 0.75f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f, //c

         0.64f, -0.58f,  1.24f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f, //d
        -0.05f, -0.56f, 0.75f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f, //e
         0.64f, -0.58f, 0.75f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f  //f
    };
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int floorTexture = loadTexture("Textures/wood.png");
    
    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    // lighting info
    // -------------
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        updatey();
        //updatez();
        updatex();
        updatexCircle();

        //rotateModel();
		// input
		// -----
		processInput(window);
		animator.UpdateAnimation(deltaTime);
		
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
        ourShader.use();
        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("viewPos", camera.Position);
        

        // // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        
        // mountain 
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.6f, -0.6f, -2.5f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model,3.0f,glm::vec3(0,1,0));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        // // snowman
        // glm::mat4 model2 = glm::mat4(1.0f);
        // model2 = glm::translate(model2, glm::vec3(-0.6f, y_position,z_position+1.0f)); // translate it down so it's at the center of the scene
        // model2 = glm::rotate(model2,1.4f,glm::vec3(0,0,1));//rotation x = 0.0 degrees
        // //model2 = glm::rotate(model2,0.9f,glm::vec3(1,0,0));//rotation x = 0.0 degrees
        // model2 = glm::rotate(model2,-angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
        // model2 = glm::scale(model2, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
        // ourShader.setMat4("model", model2);
        // ourModel2.Draw(ourShader);


        //xmasTree
        glm::mat4 modelxmasTree = glm::mat4(1.0f);
        modelxmasTree = glm::translate(modelxmasTree, glm::vec3(0.1f, -0.55f,1.0f)); // translate it down so it's at the center of the scene
        modelxmasTree = glm::rotate(modelxmasTree,0.0f,glm::vec3(1,0,0));//rotation x = 0.0 degrees
        modelxmasTree = glm::scale(modelxmasTree, glm::vec3(0.0003f, 0.0003f, 0.0003f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelxmasTree);
        xmasTree.Draw(ourShader);

       //xmasTree2
        glm::mat4 modelxmasTree2 = glm::mat4(1.0f);
        modelxmasTree2 = glm::translate(modelxmasTree2, glm::vec3(-0.3f, -0.55f,1.6f)); // translate it down so it's at the center of the scene
        modelxmasTree2 = glm::rotate(modelxmasTree2,0.0f,glm::vec3(1,0,0));//rotation x = 0.0 degrees
        modelxmasTree2 = glm::scale(modelxmasTree2, glm::vec3(0.0006f, 0.0006f, 0.0006f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelxmasTree2);
        xmasTree.Draw(ourShader);


        //xmasTree3
        glm::mat4 modelxmasTree3 = glm::mat4(1.0f);
        modelxmasTree3 = glm::translate(modelxmasTree3, glm::vec3(0.6f, -0.55f,1.6f)); // translate it down so it's at the center of the scene
        modelxmasTree3 = glm::rotate(modelxmasTree3,0.0f,glm::vec3(1,0,0));//rotation x = 0.0 degrees
        modelxmasTree3 = glm::scale(modelxmasTree3, glm::vec3(0.0006f, 0.0006f, 0.0006f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelxmasTree3);
        xmasTree.Draw(ourShader);

        // // house
        glm::mat4 hModel = glm::mat4(1.0f);
        hModel = glm::translate(hModel, glm::vec3(0.3f, -0.6f, 1.0f)); // translate it down so it's at the center of the scene
        hModel = glm::scale(hModel, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", hModel);
        houseModel.Draw(ourShader);

        ourShader2.use();
        ourShader2.setMat4("projection", projection);
        ourShader2.setMat4("view", view);

       
       //the bird
        auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader2.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		//the bird
		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, glm::vec3(x_position, 0.5f, z_position)); // translate sit down so it's at the center of the scene
        model3 = glm::rotate(model3,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model3 = glm::scale(model3, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model3);
		ourModel3.Draw(ourShader2);

        //the bird2
		glm::mat4 model4 = glm::mat4(1.0f);
		model4 = glm::translate(model4, glm::vec3(x_position, 0.2f, 0.5f)); // translate it down so it's at the center of the scene
        model4 = glm::rotate(model4,1.4f,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model4 = glm::scale(model4, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model4);
		ourModel3.Draw(ourShader2);


        // be sure to activate shader when setting uniforms/drawing objects
        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setFloat("material.shininess", 32.0f);

        // directional light
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[0].constant", 1.0f);
        ourShader.setFloat("pointLights[0].linear", 0.09f);
        ourShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[1].constant", 1.0f);
        ourShader.setFloat("pointLights[1].linear", 0.09f);
        ourShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[2].constant", 1.0f);
        ourShader.setFloat("pointLights[2].linear", 0.09f);
        ourShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[3].constant", 1.0f);
        ourShader.setFloat("pointLights[3].linear", 0.09f);
        ourShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        // ourShader.setVec3("spotLight.position", camera.Position);
        // ourShader.setVec3("spotLight.direction", camera.Front);
        // ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        // ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        // ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        // ourShader.setFloat("spotLight.constant", 1.0f);
        // ourShader.setFloat("spotLight.linear", 0.09f);
        // ourShader.setFloat("spotLight.quadratic", 0.032f);
        // ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        // ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));     

        
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // world transformation
        model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);

        // draw blinn phonge
        shader.use();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setInt("blinn", blinn);
        // floor
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cubea
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default



		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    glDeleteVertexArrays(1, &skyboxVAO);
  
    glDeleteBuffers(1, &skyboxVBO); 

    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

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

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed) 
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) 
    {
        blinnKeyPressed = false;
    }
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

void updatey(){
    if(y_position < -0.42f){
        y_position = -0.15f;
    }else{
        y_position-=0.0005;
    }
     
     
}

// void updatez(){
//     if(y_position < -0.42f){
//         z_position = -0.45f;
//     }else{
//      z_position+=0.0005;
//     }
// }

void updatexCircle(){

    if(movingRight){
        z_position = sqrt(1-(x_position*x_position));
    }else{
        z_position = -sqrt(1-(x_position*x_position));
    }
    
    
}

void updatex(){
    if(x_position >= 1.0f){
        movingRight=false;
    }else if(x_position <= -1.0f){
        movingRight=true;
    }
    
    if(movingRight){
        x_position +=0.005;
    }else{
        x_position -=0.005;
    }
}

// void rotateBird(){
//     if(y_position < -0.42f){
//         //do nothing
//     }else{
//      angle += 0.08;
//     }
// }

void rotateModel(){
    if(y_position < -0.42f){
        //do nothing
    }else{
     angle += 0.08;
    }
}


unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}