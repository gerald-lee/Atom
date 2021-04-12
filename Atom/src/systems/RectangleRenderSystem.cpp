#include "Pch.hpp"
#include "RectangleRenderSystem.hpp"
#include "utils/Log.hpp"
#include "core/AtomEngine.hpp"
#include "core/Types.hpp"
#include "core/AtomEngine.hpp"
#include "components/AllComponents.hpp"

//Temp
#include "ControllerSystem.hpp"

// this is needed in all systems as the engine is in the Application.cpp translation unit 
extern AtomEngine ae;

void RectangleRenderSystem::init() {
	DebugMode = false;

	worriorTimer = glfwGetTime();
	VampireTimer = glfwGetTime();
	laternTimer = glfwGetTime();

	// init shaders
	ColorRecShader = std::make_unique<Shader> ("Atom/res/shaders/ColorRec.shader");
	TextureRecShader = std::make_unique<Shader>("Atom/res/shaders/TextureRec.shader");

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	   // positions   // texCoords
	   -0.5f,  0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  1.0f, 0.0f,

	   -0.5f,  0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  1.0f, 1.0f
	};
	unsigned int quadVBO;

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &RecVAO);
	glBindVertexArray(RecVAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//// number of particles, size of particles, time
	//ParticleConfig pCon(30, 0.3, glm::vec2(0.1f, 2.0f));
	//// spawn center, spawn area size, spawn area shape
	//SpawnConfig sCon(glm::vec2(-0.8f, -0.8), 0.1f, AREA_MODE::SQUARE);
	//// move direction, speed, move pattern
	//MoveConfig mCon(glm::vec2(0.0, 1.0), 0.01f, DIR_MODE::CIRCULAR);
	//particleEffect = new ParticleEffect(sCon, mCon, pCon);

	//particleEffect->Init();
	//particleEffect->Print();

}


void RectangleRenderSystem::update() {

	if (ae.mLevelManager->level > COUNT_INTROS) {
		setBackground("Atom/res/Level_01_background.png");
	}

	if (ae.mInputManager->isKeyTriggered(ATOM_KEYCODE_T)) {
		DebugMode = !DebugMode;
		ae.mIsDebugMode = DebugMode;
	}

	// draw Background 
	// Temp - moving background
	glm::vec2 backgroundPos = ae.mSystemManager->getSystem<ControllerSystem>()->playerPosition;
	draw(backgroundPos, glm::vec2(5.0f), BackgroundAddress);

	//if (ae.mLevelManager->GetCurrentLevelNumber() < 2)
	//	particleEffect->Draw();

	// draw all entities
	drawEntities(DebugMode);

	// animation demo
	//drawAnimation(glm::vec2(0.8, 0.8), glm::vec2(0.4f), "knightSlice", "png", 3, worriorTimer, DebugMode);

	//drawAnimation(glm::vec2(0.8, -0.8), glm::vec2(0.4f), "Vampire", "png", 4, VampireTimer, DebugMode);

	//drawAnimation(glm::vec2(-0.8, -0.8), glm::vec2(0.4f), "latern", "png", 4, laternTimer, DebugMode);


}



void RectangleRenderSystem::drawKeyFrame(glm::vec2 pos, glm::vec2 scale, std::string name, std::string type, int n, bool wireframe) const
{
	std::string address = "Atom/res/animation/" + name + std::to_string(n) + "." + type;
	draw(pos, scale, address, false);
	if (wireframe) draw(pos, scale, address, true);
}



void RectangleRenderSystem::drawAnimation(glm::vec2 pos, glm::vec2 scale, std::string name, std::string type, int n, float &timer, bool wireframe) const
{
	float deltaTime = glfwGetTime() - timer;

	for (int i = 1; i <= n; ++i)
	{
		if (deltaTime < 0.1f * i) {
			drawKeyFrame(pos, scale, name, type, i - 1, wireframe);
			break;
		}
	}
	if (deltaTime > 0.1f * n)
	{
		drawKeyFrame(pos, scale, name, type, n - 1, wireframe);
		timer = glfwGetTime();
	}
}


void RectangleRenderSystem::onEvent(Event& e) {

}


void RectangleRenderSystem::draw(glm::vec2 pos, glm::vec2 scale, glm::vec3 color, bool wireframe, float alpha) const
{

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	ColorRecShader->SetFloat("alpha", alpha);
	ColorRecShader->SetInt("wireframe", wireframe);

	ColorRecShader->SetVec2("pos", pos);
	ColorRecShader->SetVec2("scale", scale);
	ColorRecShader->SetVec3("color", color);

	ColorRecShader->SetVec2("cameraPos", ae.mCameraManager->camera.position);
	// pass view matrix uniform 
	ColorRecShader->SetMat4("projection", ae.mCameraManager->GetProjectionMatrix());

	ColorRecShader->Bind();
	glBindVertexArray(RecVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);

}



void RectangleRenderSystem::draw(glm::vec2 pos, glm::vec2 scale, string texturePath, bool wireframe, float alpha) const
{
	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	TextureRecShader->SetInt("wireframe", wireframe);
	TextureRecShader->SetFloat("alpha", alpha);
	TextureRecShader->SetVec2("pos", pos);
	TextureRecShader->SetVec2("scale", scale);

	TextureRecShader->SetVec2("cameraPos", ae.mCameraManager->camera.position);

	TextureRecShader->SetMat4("projection", ae.mCameraManager->GetProjectionMatrix());

	// load texture
	unsigned int textureID = ae.getOrLoadResource<unsigned int>(texturePath);
	TextureRecShader->SetTexture("tex", textureID);

	TextureRecShader->Bind();
	glBindVertexArray(RecVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void RectangleRenderSystem::drawEntities(bool debugMode)
{

	for (auto& entity : mEntities) {
		if (ae.hasComponent<TagComponent>(entity)) {
			auto& tag = ae.getComponent<TagComponent>(entity);
			if (tag.tag == "player") {
				if (ae.hasComponent<CharacteristicComponent>(entity)) {
					auto& c = ae.getComponent<CharacteristicComponent>(entity);
					if (!c.isBig) {
						if (ae.hasComponent<TransformComponent>(entity)) {
							auto& t = ae.getComponent<TransformComponent>(entity);
							//particleEffect->position = t.position;
						}
					}
				}
			}
		}

		if (ae.hasComponent<RectangleComponent>(entity)) {
			auto& rc = ae.getComponent<RectangleComponent>(entity);

			if (ae.hasComponent<TransformComponent>(entity)) {
				auto& t = ae.getComponent<TransformComponent>(entity);
				float alpha = 1.0f;
				alpha = ae.mLevelManager->level_alpha;

				if (!rc.texturePath.empty())
					draw(glm::vec2{ t.position.x,t.position.y }, t.scale, rc.texturePath, false,alpha);
				else
					draw(glm::vec2{ t.position.x,t.position.y }, t.scale, rc.color, false, alpha);

				if (debugMode) draw(glm::vec2{ t.position.x,t.position.y }, t.scale, rc.color, true, alpha);
			}


		}
	}
}
