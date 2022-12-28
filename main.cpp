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

#include<cstdio>
#include<ctime>



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
void renderQuad();
void updatexSnowMan();
void updatexCircleBob();

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

//translation co ord
float y_position_bob = -0.15f;
float z_position_bob = 0.0f;
float x_position_bob = -1.0f;
bool movingRight_bob = true;

float angle = 90.0f * (M_PI/180.0f);

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

    clock_t start = clock();
	// build and compile shaders
	// -------------------------
	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
    Shader ourShader2("anim_model.vs", "anim_model.fs");
    Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");
    Shader shader("1.advanced_lighting.vs", "1.advanced_lighting.fs");
    Shader shaderFloor("1.advanced_lighting.vs", "1.advanced_lighting.fs");

    Shader shaderBrick("4.normal_mapping.vs", "4.normal_mapping.fs");

    // load textures
    // -------------
    unsigned int diffuseMap = loadTexture("Textures/brickwall.jpg");
    unsigned int normalMap  = loadTexture("Textures/brickwall_normal.jpg");

    // shader configuration
    // --------------------
    shaderBrick.use();
    shaderBrick.setInt("diffuseMap", 0);
    shaderBrick.setInt("normalMap", 1);


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
    Model santaBottom("Objects/SantaBottom/SantaBottom.obj");
    Model santaBody("Objects/SantaBottom/santaHeadAndco4.0.obj");

    // load models
	// -----------
	Model ourSanta("Objects/mountain/DancingBob3.1.dae");
	Animation santaAnimation("Objects/mountain/DancingBob3.0.dae",&ourSanta);
	Animator animatorSanta(&santaAnimation);

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
    unsigned int redTexture = loadTexture("Objects/SantaBottom/colorRed.png");
    
    // shader configuration
    // --------------------
    shader.use();
    

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
        rotateModel();
        updatexSnowMan();
        updatexCircleBob();
		// input
		// -----
		processInput(window);
		
		
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
       
        //santaBody
        glBindTexture(GL_TEXTURE_2D, redTexture);
        glm::mat4 modelSanta3 = glm::mat4(1.0f);
        modelSanta3 = glm::translate(modelSanta3, glm::vec3(0.4f, -0.4f, 1.0f)); // translate it down so it's at the center of the scene
        modelSanta3 = glm::scale(modelSanta3, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelSanta3);
        santaBody.Draw(ourShader);

    
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
        //ourModel.Draw(ourShader);

        //santaBottom
        glm::mat4 modelSanta = glm::mat4(1.0f);
        modelSanta = glm::translate(modelSanta, glm::vec3(0.69f, 0.25f, 1.0f)); // translate it down so it's at the center of the scene
        modelSanta = glm::rotate(modelSanta,3.0f,glm::vec3(0,0,1));
        modelSanta = glm::rotate(modelSanta,1.4f,glm::vec3(0,1,0));
        modelSanta = glm::scale(modelSanta, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelSanta);
        santaBottom.Draw(ourShader);

        


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

        //xmasTree4
         glm::mat4 modelxmasTree4 = glm::mat4(1.0f);
        modelxmasTree4 = glm::translate(modelxmasTree4, glm::vec3(0.7f, -0.55f,-0.4f)); // translate it down so it's at the center of the scene
        modelxmasTree4 = glm::rotate(modelxmasTree4,0.0f,glm::vec3(1,0,0));//rotation x = 0.0 degrees
        modelxmasTree4 = glm::scale(modelxmasTree4, glm::vec3(0.0006f, 0.0006f, 0.0006f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", modelxmasTree4);
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


       
       if(!blinnKeyPressed){
       //the bird

        animator.UpdateAnimation(deltaTime);

        auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader2.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		//the bird
		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, glm::vec3(x_position, ((0.25f)*sin(x_position)), z_position)); // translate sit down so it's at the center of the scene
        model3 = glm::rotate(model3,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model3 = glm::scale(model3, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model3);
		ourModel3.Draw(ourShader2);

        //the bird2
		glm::mat4 model4 = glm::mat4(1.0f);
		model4 = glm::translate(model4, glm::vec3(x_position-0.2f, ((0.25f)*sin(x_position)), z_position-0.2f)); // translate it down so it's at the center of the scene
        model4 = glm::rotate(model4,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model4 = glm::scale(model4, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model4);
		ourModel3.Draw(ourShader2);

        //the bird3
		glm::mat4 model5 = glm::mat4(1.0f);
		model5 = glm::translate(model5, glm::vec3(x_position-0.2f, ((0.25f)*sin(x_position)), z_position+0.2f)); // translate it down so it's at the center of the scene
        model5 = glm::rotate(model5,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model5 = glm::scale(model5, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model5);
		ourModel3.Draw(ourShader2);

        //the bird4
		glm::mat4 model6 = glm::mat4(1.0f);
		model6 = glm::translate(model6, glm::vec3(x_position-0.2f, ((0.25f)*sin(x_position)), z_position-0.3f)); // translate it down so it's at the center of the scene
        model6 = glm::rotate(model6,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model6 = glm::scale(model6, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model6);
		ourModel3.Draw(ourShader2);

        //the bird5
		glm::mat4 model7 = glm::mat4(1.0f);
		model7 = glm::translate(model7, glm::vec3(x_position-0.2f, ((0.25f)*sin(x_position)), z_position+0.3f)); // translate it down so it's at the center of the scene
        model7 = glm::rotate(model7,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model7 = glm::scale(model7, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model7);
		ourModel3.Draw(ourShader2);

        //the bird6
		glm::mat4 model8 = glm::mat4(1.0f);
		model8 = glm::translate(model8, glm::vec3(x_position+0.2f, ((0.25f)*sin(x_position)), z_position-0.3f)); // translate it down so it's at the center of the scene
        model8 = glm::rotate(model8,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model8 = glm::scale(model8, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model8);
		ourModel3.Draw(ourShader2);

        //the bird7
		glm::mat4 model9 = glm::mat4(1.0f);
		model9 = glm::translate(model9, glm::vec3(x_position+0.2f, ((0.25f)*sin(x_position)), z_position+0.3f)); // translate it down so it's at the center of the scene
        model9 = glm::rotate(model9,angle,glm::vec3(0,1,0));//rotation x = 0.0 degrees
		model9 = glm::scale(model9, glm::vec3(.1f, .1f, .1f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", model9);
		ourModel3.Draw(ourShader2);
       }else{
        animatorSanta.UpdateAnimation(deltaTime);
        //bob
        auto transforms = animatorSanta.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader2.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		//bob
		glm::mat4 modelSanta5 = glm::mat4(1.0f);
		//modelSanta5 = glm::translate(modelSanta5, glm::vec3(x_position_bob, ((-0.3f)*sin(x_position_bob)), z_position_bob)); // translate sit down so it's at the center of the scene
		//modelSanta5 = glm::translate(modelSanta5, glm::vec3(x_position_bob, -0.1f*sin(x_position_bob),-0.0f)); // translate sit down so it's at the center of the scene
        modelSanta5 = glm::translate(modelSanta5, glm::vec3(1.05f, -0.3f,-0.4f)); // translate sit down so it's at the center of the scene

       // modelSanta5 = glm::rotate(modelSanta5,angle,glm::vec3(0,1,0));
        modelSanta5 = glm::scale(modelSanta5, glm::vec3(0.07f, 0.07f, 0.07f));	// it's a bit too big for our scene, so scale it down
		ourShader2.setMat4("model", modelSanta5);
		ourSanta.Draw(ourShader2);
       }

        //brick
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        shaderBrick.use();
        shaderBrick.setMat4("projection", projection);
        shaderBrick.setMat4("view", view);
        // render normal-mapped quad
        glm::mat4 modelBrick = glm::mat4(1.0f);
        modelBrick = glm::translate(modelBrick, glm::vec3(0.3f, -0.6f, 1.4f)); 
        modelBrick = glm::rotate(modelBrick, glm::radians( -100.0f), glm::normalize(glm::vec3(1.0, 0.0, 0.0))); // rotate the quad to show normal mapping from multiple directions
        modelBrick = glm::scale(modelBrick, glm::vec3(0.35f, 0.35f, 0.35f));
        shaderBrick.setMat4("model", modelBrick);
        shaderBrick.setVec3("viewPos", camera.Position);
        shaderBrick.setVec3("lightPos", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        renderQuad();


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
        shaderFloor.use();
        shaderFloor.setInt("texture1", 0);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        shaderFloor.setMat4("projection", projection);
        shaderFloor.setMat4("view", view);
        // set light uniforms
        shaderFloor.setVec3("viewPos", camera.Position);
        shaderFloor.setVec3("lightPos", lightPos);
        shaderFloor.setInt("blinn", blinn);
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
    if(y_position_bob < -0.3f){
        y_position_bob = -0.15f;
    }else{
        y_position_bob-=0.0005;
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
        z_position = sqrt(9-(x_position*x_position));
    }else{
        z_position = -sqrt(9-(x_position*x_position));
    }
    
    
}

void updatexCircleBob(){

    if(movingRight_bob){
        z_position_bob = sqrt(0.1f-((x_position_bob-0.6)*(x_position_bob-0.6))) + 1.6f;
        
    }else{
        z_position_bob = -(sqrt(0.1f-((x_position_bob-0.6)*(x_position_bob-0.6))) + 1.6f );
    }
    
    
}


void updatex(){
    if(x_position >= 3.0f){
        movingRight=false;
    }else if(x_position <= -3.0f){
        movingRight=true;
    }
    
    if(movingRight){
        x_position +=0.005;
    }else{
        x_position -=0.005;
    }
}



void updatexSnowMan(){
    // if(x_position_bob >= 1.0f){
    //     movingRight_bob = false;
    // }else if(x_position_bob <= 0.75f){
    //     movingRight_bob = true;
    // }
    
    // if(movingRight_bob){
        x_position_bob +=0.005;
    // }else{
    //     x_position_bob -=0.005;
    // }
}

// void rotateBird(){
//     if(y_position < -0.42f){
//         //do nothing
//     }else{
//      angle += 0.08;
//     }
// }

void rotateModel(){
    // if(y_position < -0.42f){
    //     //do nothing
    // }else{
    //  angle += 0.08;
    // }

if(x_position >= 3.0f || x_position <= -3.0f){

}else{
    if(angle >= (360 * (M_PI/180.0f))){
        angle = 0.0f;
    }else{
        angle += ((0.15f) * (M_PI/180.0f));
    }
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

// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
        glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);  
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


        float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}