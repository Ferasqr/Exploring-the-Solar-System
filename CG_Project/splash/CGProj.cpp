// This has been adapted from the Vulkan tutorial

#include "CGP_Starter.hpp"


// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(4) float amb;
	alignas(4) float gamma;
	alignas(16) glm::vec3 sColor;
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};
struct OverlayUniformBlock {
	alignas(4) float visible;
};
//Light 
struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec2 UV;
	glm::vec3 norm;
};
struct VertexOverlay {
	glm::vec2 pos;
	glm::vec2 UV;
};
class SolarSystem;
void PlanetArray(SolarSystem* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World, float ObjSpeed);

// MAIN ! 
class SolarSystem : public BaseProject {
protected:
	// Here you list all the Vulkan objects you need:

	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1, DSLOverlay;

	// Pipelines [Shader couples]
	VertexDescriptor VD;
	VertexDescriptor VOverlay;

	Pipeline P1;
	Pipeline POverlay;


	// Models, textures and Descriptors (values assigned to the uniforms)
	Model<Vertex> M1, MG, MGR;
	Model<VertexOverlay> MKey, MSplash;
	Texture T1, TG[11], TKey, TSplash;
	DescriptorSet DS1, DSG[11], DSKey, DSSplash;

	// Other application parameters
	float Ar;
	float ObjSpeed = 0.10f;
	int gameState;
	glm::mat4 GWM[11];
	glm::mat4 GWS;

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "CG Project";
		windowResizable = GLFW_TRUE;
		initialBackgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		// Descriptor pool sizes
		uniformBlocksInPool = 200;
		texturesInPool = 200;
		setsInPool = 200;

		Ar = 4.0f / 3.0f;
	}

	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		std::cout << "Window resized to: " << w << " x " << h << "\n";
		Ar = (float)w / (float)h;
	}

	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSL1.init(this, {
			// this array contains the binding:
			// first  element : the binding number
			// second element : the type of element (buffer or texture)
			// third  element : the pipeline stage where it will be used
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
			{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});
		DSLOverlay.init(this, {
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
				{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		//Vertex Descriptors
		VD.init(this, {
				  {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
			}, {
			  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
					 sizeof(glm::vec3), POSITION},
			  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
					 sizeof(glm::vec3), NORMAL},
			  {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
					 sizeof(glm::vec2), UV}
			});


		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, &VD, "shaders/PhongVert.spv", "shaders/PhongFrag.spv", { &DSL1 });
		POverlay.init(this, &VOverlay, "shaders/OverlayVert.spv", "shaders/OverlayFrag.spv", { &DSLOverlay });
		POverlay.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_NONE, false);
		
		//P1.init(this, &VD, "shaders/SunShader.spv", { &DSL2 });
		//PS.init(this, &VD, "")

		// Models, textures and Descriptors (values assigned to the uniforms)
		M1.init(this, &VD, "models/Starship2.obj", OBJ);
		MG.init(this, &VD, "models/Sphere.obj", OBJ);
		MGR.init(this, &VD, "models/Saturn_Ring.obj", OBJ);

		T1.init(this, "textures/Spaceship.png");
		TG[0].init(this, "textures/Sun.jpg");
		TG[1].init(this, "textures/Mercury.jpg");
		TG[2].init(this, "textures/Venus.jpg");
		TG[3].init(this, "textures/Earth.jpg");
		TG[4].init(this, "textures/Moon.jpg");
		TG[5].init(this, "textures/Mars.jpg");
		TG[6].init(this, "textures/Jupiter.jpg");
		TG[7].init(this, "textures/Uranus.jpg");
		TG[8].init(this, "textures/Neptune.jpg");
		TG[9].init(this, "textures/Saturn.jpg");
		TG[10].init(this, "textures/Saturn Ring.png");
		TSplash.init(this, "textures/SplashScreen.png");
		// Creates a mesh with direct enumeration of vertices and indices
		MKey.vertices = { {{-0.8f, 0.6f}, {0.0f,0.0f}}, {{-0.8f, 0.95f}, {0.0f,1.0f}},
						 {{ 0.8f, 0.6f}, {1.0f,0.0f}}, {{ 0.8f, 0.95f}, {1.0f,1.0f}} };
		MKey.indices = { 0, 1, 2,    1, 2, 3 };
		MKey.initMesh(this, &VOverlay);

		// Creates a mesh with direct enumeration of vertices and indices
		MSplash.vertices = { {{-1.0f, -0.58559f}, {0.0102f, 0.0f}}, {{-1.0f, 0.58559f}, {0.0102f,0.85512f}},
						 {{ 1.0f,-0.58559f}, {1.0f,0.0f}}, {{ 1.0f, 0.58559f}, {1.0f,0.85512f}} };
		MSplash.indices = { 0, 1, 2,    1, 2, 3 };
		MSplash.initMesh(this, &VOverlay);

		GWS = glm::scale(glm::mat4(1), glm::vec3(0.010f)); //Spaceship scale

		GWM[0] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(10.9f)), glm::vec3(0, 0, 0)); //Sun
		GWM[1] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.0383f)), glm::vec3(0, 0, -1200.980f)); //Mercury
		GWM[2] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.0949f)), glm::vec3(0, 0, -899.660f)); //Venus
		GWM[3] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.10f)), glm::vec3(0, 0, -1178.240f)); //Earth
		GWM[4] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.027f)), glm::vec3(1, 0, -4336.810f)); //Moon
		GWM[5] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.0533f)), glm::vec3(0, 0, -3365.660f)); //Mars
		GWM[6] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(1.1209f)), glm::vec3(0, 0, -545.0230f)); //Jupiter
		GWM[7] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.40f)), glm::vec3(0, 0, -5620.710f)); //Uranus
		GWM[8] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.3883f)), glm::vec3(0, 0, -9097.280f));  //Neptune
		GWM[9] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.95f)), glm::vec3(0, 10, -1189.8480f)); //Saturn
		GWM[10] = glm::translate(glm::scale(glm::mat4(1), glm::vec3(0.95f)), glm::vec3(0, 10, -1189.8480f)); //Saturn's Rings
	}

	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();
		POverlay.create();

		DS1.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});
		for (int i = 0; i < 11; i++) {
			DSG[i].init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &TG[i]},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		}
		DSKey.init(this, &DSLOverlay, {
					{0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
					{1, TEXTURE, 0, &TKey}
			});
		DSSplash.init(this, &DSL1, {
					{0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
					{1, TEXTURE, 0, &TSplash}
			});
		/*DSG[10].init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &TG[10]},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});*/
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		POverlay.cleanup();

		DS1.cleanup();
		DSG[0].cleanup();
		DSG[1].cleanup();
		DSG[2].cleanup();
		DSG[3].cleanup();
		DSG[4].cleanup();
		DSG[5].cleanup();
		DSG[6].cleanup();
		DSG[7].cleanup();
		DSG[8].cleanup();
		DSG[9].cleanup();
		DSG[10].cleanup();

		DSKey.cleanup();
		DSSplash.cleanup();

	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1.cleanup();
		TKey.cleanup();
		TSplash.cleanup();

		M1.cleanup();
		MG.cleanup();
		TG[0].cleanup();
		TG[1].cleanup();
		TG[2].cleanup();
		TG[3].cleanup();
		TG[4].cleanup();
		TG[5].cleanup();
		TG[6].cleanup();
		TG[7].cleanup();
		TG[8].cleanup();
		TG[9].cleanup();
		TG[10].cleanup();
		TSplash.cleanup();
		MSplash.cleanup();

		MGR.cleanup();

		DSL1.cleanup();

		P1.destroy();
		POverlay.destroy();
	}

	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

		P1.bind(commandBuffer);
		M1.bind(commandBuffer);
		DS1.bind(commandBuffer, P1, currentImage);

		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);

		MG.bind(commandBuffer);
		for (int i = 0; i < 11; i++) {
			DSG[i].bind(commandBuffer, P1, currentImage);

			vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MG.indices.size()), 1, 0, 0, 0);
		}

		MGR.bind(commandBuffer);
		DSG[10].bind(commandBuffer, P1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MGR.indices.size()), 1, 0, 0, 0);

		POverlay.bind(commandBuffer);
		MKey.bind(commandBuffer);
		DSKey.bind(commandBuffer, POverlay, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MKey.indices.size()), 1, 0, 0, 0);

		MSplash.bind(commandBuffer);
		DSSplash.bind(commandBuffer, POverlay, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MSplash.indices.size()), 1, 0, 0, 0);


	}



	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();
		static float lastTime = 0.0f;

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::minutes::period>
			(currentTime - startTime).count();
		float deltaT = time - lastTime;
		lastTime = time;


		glm::mat4 ViewPrj;
		glm::mat4 WM;



		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_Z)) {
			ObjSpeed += 0.10f;
		}

		if (glfwGetKey(window, GLFW_KEY_C)) {
			ObjSpeed -= 0.10f;
			if (ObjSpeed < 0.0f) {
				ObjSpeed = 0.0f;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			ObjSpeed = 0.10f;
		}

		PlanetArray(this, Ar, ViewPrj, WM, ObjSpeed);

		UniformBufferObject ubo{};
		// Here is where you actually update your uniforms


		glm::mat4 baseTr = glm::rotate(glm::mat4(1.0f), glm::radians(360.0f) * time, glm::vec3(0.0f, 1.0f, 0.0f));


		OverlayUniformBlock uboKey, uboSplash;
		// updates global uniforms
		GlobalUniformBufferObject gubo{};
		gubo.lightDir = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)); // Set the light direction towards the positive z-axis (or any other direction you desire)
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Set the light color (white in this case)
		gubo.eyePos = glm::vec3(0.0f, 0.0f, 0.0f); // Set the eye position (camera position)

		DS1.map(currentImage, &ubo, sizeof(ubo), 2); // Map the updated uniform buffer to the descriptor set


		ubo.mMat = WM * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0)) * GWS;
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS1.map(currentImage, &ubo, sizeof(ubo), 0);
		DS1.map(currentImage, &gubo, sizeof(gubo), 2);

		uboKey.visible = (gameState == 1) ? 1.0f : 0.0f;
		DSKey.map(currentImage, &uboKey, sizeof(uboKey), 0);

		uboSplash.visible = (gameState == 0) ? 1.0f : 0.0f;
		DSSplash.map(currentImage, &uboSplash, sizeof(uboSplash), 0);
		for (int i = 0; i < 11; i++) {
			ubo.mMat = GWM[i] * baseTr;
			ubo.mvpMat = ViewPrj * ubo.mMat;

			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DSG[i].map(currentImage, &ubo, sizeof(ubo), 0);
			DSG[i].map(currentImage, &gubo, sizeof(gubo), 2);

		}

	}



};

#include "CGP_Logic.hpp"

// This is the main: probably you do not need to touch this!
int main() {

	SolarSystem app;



	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}
