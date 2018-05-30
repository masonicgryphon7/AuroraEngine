#include "Player.h"
#include <random>

Player::Player()
{
}


Player::~Player()
{
	if (jGUI != nullptr)
		delete jGUI;

	/*if (enemyNPC != nullptr)
		delete enemyNPC;

	if (enemyObject != nullptr)
		delete enemyObject;*/

	ImGui_ImplDX11_Shutdown();
	//ImGui::DestroyContext();
}

void Player::Start(HWND * w, ID3D11Device * d, ID3D11DeviceContext * dc, CoreEngine * ce)
{
	this->wnd = w;
	this->device = d;
	this->deviceContext = dc;
	this->coreEngine = ce;

	// Setup style
	ImGui::StyleColorsDark();


	jGUI = new JGUI();

	// BUG SHIT???

	gamemanager.unitLists = std::vector<std::vector<Unit*>>(4, std::vector<Unit*>());
	gamemanager.buildingLists = std::vector<std::vector<Unit*>>(4, std::vector<Unit*>());

	// ADD PLAYER HERO
	coreEngine->camera->getComponent<Camera>()->gameObject->getComponent<PlayerScript>()->instantiate_Player();

	// ADD ENEMY HERO
	enemyObject = gScene.createEmptyGameObject();
	enemyNPC = new NPC();
	enemyNPC->instantiate_NPC();
	enemyObject->addComponent(enemyNPC);

	gamemanager.addBuildings();
	gamemanager.gameState = GAME_STATE::START_STATE;
	gamemanager.gameState = GAME_STATE::MAIN_MENU;

	// END BUG SHIT???
	//AssetManager.AddTexture("troll_made-with-unity.png");
	//AssetManager.addTexture("Assets/troll_made-with-unity.png");
}

GameObject* tempSelectedBugFixRaycast = nullptr;

bool onceKickToMainMenu = false;
float kickToMainMenu = 0.0f;
float gameTime = 600.0f;
float timeSelectToUnits = 0.0f;
Vector2 mousePosUnits;

Unit* heroUnit = nullptr;
float gameOverLerp = 0.0f, notEnoughResourceLerp = 0.0f, ringOfFireAnnounce = 0.0f;
int rof_state = 0;
std::vector<Player::HansLillaGrej> hansLillaGrej;

float unityTroll = 0.0f;
unsigned int stateTroll = 0;
bool includeSplashTroll = false;


#include "Debug.h"
void Player::Update()
{
	ImGui_ImplDX11_NewFrame();

	Vector2 screen = Input.GetDesktopResolution();

	Camera* cam = coreEngine->camera->getComponent<Camera>();

	PlayerScript* playerScript = cam->gameObject->getComponent<PlayerScript>();

	ImGui::SetNextWindowPos(ImVec2(-4, -1));
	ImGui::SetNextWindowSize(ImVec2(screen.x, screen.y), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSizeConstraints(ImVec2(screen.x - minx, screen.y - miny), ImVec2(screen.x, screen.y));
	ImGui::SetNextWindowBgAlpha(-1.0f);
	ImGui::Begin("", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

	jGUI->SetDrawList(ImGui::GetWindowDrawList());
	jGUI->UpdateRuntimeEditor(false);

	/*std::stringstream fps;
	fps << std::fixed << std::setprecision(0) << "FPS: " << ImGui::GetIO().Framerate;

	jGUI->SetText("FPSCounter", fps.str());*/

	if (gamemanager.gameState != GAME_STATE::MAIN_MENU)
	{
		jGUI->SetActive("MainMenu", false, true);

		if (gamemanager.gameState != GAME_STATE::GAME_OVER_MENU && gamemanager.gameState != GAME_STATE::WIN_MENU)
		{
			if (gamemanager.unitLists[1][0] != nullptr && gamemanager.unitLists[2][0] != nullptr)
			{
				if (gamemanager.unitLists[1][0]->getHealthPoints() <= 0)
				{
					gamemanager.gameState = GAME_STATE::GAME_OVER_MENU;
				}

				if (gamemanager.unitLists[2][0]->getHealthPoints() <= 0)
				{
					gamemanager.gameState = GAME_STATE::WIN_MENU;
				}
			}
		}

		if (heroUnit == nullptr) // optimize? temporary in case we want smth else
			for (size_t i = 0; i < gScene.getSceneObjects().size(); i++)
			{
				GameObject* go = gScene.getSceneObjects().at(i);

				if (go->getComponent<Unit>())
				{
					Unit* unit = gScene.getSceneObjects().at(i)->getComponent<Unit>();
					// TYPES
					{
						Type type = unit->getType();
						std::stringstream typeStream;

						typeStream << std::fixed << std::setprecision(0);

						// HERO
						if (type == Type::Hero)
						{
							if (unit->gameObject->tag != 1) continue;

							if (heroUnit == nullptr)
								heroUnit = unit;

							// HP
							float hero_hp = unit->getHealthPoints();
							float hero_maxHP = unit->getMaxHealthPoints();
							typeStream << hero_hp << "/" << hero_maxHP;
							jGUI->SetText("InGame_HeroHealth", typeStream.str());

							// GOLD
							jGUI->SetText("InGame_HeroGoldCounter", std::to_string(unit->getResources()) + " G");

							// STATS
							jGUI->SetText("InGame_HeroStatsAttack", "Atk: " + std::to_string(unit->getAttackPoints()));
							jGUI->SetText("InGame_HeroStatsDefence", "Def: " + std::to_string(unit->getDefencePoints()));
						}

						// BANK
						if (type == Type::Bank)
						{

						}
					}
				}
			}

		gameTime -= Time.getDeltaTime();
		float min = floor(gameTime / 60);
		int sec = std::roundf(gameTime);
		int sss = sec % 60;

		std::stringstream stGT;
		stGT << std::fixed << std::setprecision(0) << min << ":" << sss;

		jGUI->SetText("InGame_Timer", stGT.str());

		// HERO
		if (heroUnit != nullptr && gamemanager.gameState != GAME_STATE::GAME_OVER_MENU)
		{
			std::stringstream typeStream;

			typeStream << std::fixed << std::setprecision(0);

			// Hans lilla 'request' grej ;) 
			if (heroUnit->isCollectingGold && heroUnit->goldMineCollecting != nullptr)
			{
				heroUnit->isCollectingGold = false;

				Vector3 v3 = Vector3(0, 0, 0);

				if (heroUnit != nullptr)
				{
					if (heroUnit->goldMineCollecting != nullptr)
					{
						v3 = Vector3(jGUI->WorldToScreen(cam, heroUnit->goldMineCollecting->transform.getPosition(), DirectX::XMMatrixIdentity()));
						std::random_device rd;
						std::mt19937_64 generator{ rd() };
						std::uniform_int_distribution<> dist{ -30, 30 };

						hansLillaGrej.push_back(HansLillaGrej(dist(generator), Vector3(heroUnit->goldMineCollecting->transform.getPosition())));
					}
				}
			}

			// HP
			float hero_hp = heroUnit->getHealthPoints();
			float hero_maxHP = heroUnit->getMaxHealthPoints();
			typeStream << hero_hp << "/" << hero_maxHP;
			jGUI->SetText("InGame_HeroHealth", typeStream.str());

			// GOLD
			jGUI->SetText("InGame_HeroGoldCounter", std::to_string(heroUnit->getResources()) + " G");

			// STATS
			jGUI->SetText("InGame_HeroStatsAttack", "Atk: " + std::to_string(heroUnit->getAttackPoints()));
			jGUI->SetText("InGame_HeroStatsDefence", "Def: " + std::to_string(heroUnit->getDefencePoints()));
		}

		for (std::vector<Player::HansLillaGrej>::iterator it = hansLillaGrej.begin(); it != hansLillaGrej.end();)
		{
			if (it->timer <= 0.0f)
				it = hansLillaGrej.erase(it);
			else
			{
				it->timer -= Time.getDeltaTime() / 2;

				Vector3 v3 = Vector3(jGUI->WorldToScreen(cam, it->originalPos.asXMVECTOR(), DirectX::XMMatrixIdentity()));

				jGUI->Button(
					Vector2(v3.x + it->offset, (v3.y - 75) - jGUI->Lerp(100, 0, it->timer)),
					Vector2(v3.x + it->offset, (v3.y - 75) - jGUI->Lerp(100, 0, it->timer)),
					nullptr,
					"+20G",
					1.0f,
					ImColor(0, 0, 0, 0),
					ImColor(255, 255, 0, (int)jGUI->Lerp(0, 255, it->timer)));

				++it;
			}
		}


		if (playerScript->isSelectingHolding == 1)
		{
			jGUI->Box(playerScript->mousePosRelative, Input.mousePosition, ImColor(0, 120, 215));
			jGUI->BoxFilled(playerScript->mousePosRelative, Input.mousePosition, ImColor(0, 120, 215, 128));
		}

		if (Input.GetKeyUp(KeyCode::RightMouse))
			if (playerScript->SelectedUnits.size() > 0)
			{
				timeSelectToUnits = 1.0f;
				mousePosUnits = Input.mousePosition;
			}

		if (timeSelectToUnits > 0.0f)
		{
			timeSelectToUnits -= Time.getDeltaTime() * 5;
			jGUI->Circle(mousePosUnits, 50 * timeSelectToUnits, 24, 3.0f * timeSelectToUnits, ImColor(0, 255, 0));
		}
		else
			timeSelectToUnits = 0.0f;

		//jGUI->SetActive("InGame_NotEnoughResource", false); // since it's not needed yet

		// SELECTED UNIT

		if (tempSelectedBugFixRaycast != nullptr)
		{
			if (playerScript->SelectedUnits.size() == 0)
			{
				playerScript->SelectedUnits.push_back(tempSelectedBugFixRaycast);
				tempSelectedBugFixRaycast = nullptr;
			}
		}

		if (playerScript->SelectedUnits.size() == 0)
		{
			jGUI->SetActive("InGame_MiddleButton", false, true);
			jGUI->SetActive("InGame_SelectedLabel", false);
		}
		else if (playerScript->SelectedUnits.size() == 1)
		{
			jGUI->SetActive("InGame_SelectedLabel", true);
			Unit* unit = playerScript->SelectedUnits.at(0)->getComponent<Unit>();

			std::string typeName;

			if (unit != nullptr)
			{

				if (unit->gameObject->tag == 0) // Neutral
				{
					jGUI->SetColour("InGame_SelectedUnitBorder", ImColor(64, 64, 64, 255));
				}
				else if (unit->gameObject->tag == 1) // Friendly
				{
					jGUI->SetColour("InGame_SelectedUnitBorder", ImColor(0, 0, 255, 255));
				}
				else if (unit->gameObject->tag == 2) // Enemy
				{
					jGUI->SetColour("InGame_SelectedUnitBorder", ImColor(255, 0, 0, 255));
				}

				switch (unit->getType())
				{
				case Type::Hero:
					typeName = "Hero";
					jGUI->SetActive("InGame_Middle", false, true);
					jGUI->SetActive("InGame_MiddleInfo", true);
					jGUI->SetText("InGame_MiddleInfo", "Atk: " + std::to_string(unit->getAttackPoints()) + "   Def: " + std::to_string(unit->getDefencePoints()) + "   HP: " + std::to_string(unit->getHealthPoints()) + "/" + std::to_string(unit->getMaxHealthPoints()));
					break;

				case Type::Soldier:
					typeName = "Soldier";
					jGUI->SetActive("InGame_Middle", false, true);
					jGUI->SetActive("InGame_MiddleInfo", true);
					jGUI->SetText("InGame_MiddleInfo", "Atk: " + std::to_string(unit->getAttackPoints()) + "   Def: " + std::to_string(unit->getDefencePoints()) + "   HP: " + std::to_string(unit->getHealthPoints()) + "/" + std::to_string(unit->getMaxHealthPoints()));
					break;

				case Type::Worker:
					typeName = "Worker";
					jGUI->SetActive("InGame_Middle", false, true);
					jGUI->SetActive("InGame_MiddleInfo", true);
					jGUI->SetText("InGame_MiddleInfo", "Atk: " + std::to_string(unit->getAttackPoints()) + "   Def: " + std::to_string(unit->getDefencePoints()) + "   HP: " + std::to_string(unit->getHealthPoints()) + "/" + std::to_string(unit->getMaxHealthPoints()));
					break;

				case Type::Barrack: // BARRACK
					typeName = "Barrack";
					jGUI->SetActive("InGame_MiddleInfo", false);
					if (unit->gameObject->tag == 1)
					{
						jGUI->SetActive("InGame_MiddleButton0", true);
						jGUI->SetText("InGame_MiddleButton0", "Soldier");
						if (jGUI->HasClicked("InGame_MiddleButton0"))
						{
							if (heroUnit->getResources() >= 100)
							{
								unit->ReceiveOrder(OPTIONS::Option0);
								tempSelectedBugFixRaycast = unit->gameObject;
								int res = heroUnit->getResources();
								res -= 100;
								heroUnit->setResources(res);
							}
							else
								notEnoughResourceLerp = 1.0f;
						}
					}
					else
					{
						jGUI->SetActive("InGame_MiddleButton0", false);
						jGUI->SetActive("InGame_MiddleInfo", true);
						jGUI->SetText("InGame_MiddleInfo", "Conquer This Barrack First");
					}
					break;

				case Type::Bank: // BANK
					typeName = "Bank";
					jGUI->SetActive("InGame_MiddleInfo", false);
					if (unit->gameObject->tag == 1)
					{
						jGUI->SetActive("InGame_MiddleButton0", true);
						jGUI->SetText("InGame_MiddleButton0", "Worker");
						if (jGUI->HasClicked("InGame_MiddleButton0"))
						{
							if (heroUnit->getResources() >= 60)
							{
								unit->ReceiveOrder(OPTIONS::Option0);
								tempSelectedBugFixRaycast = unit->gameObject;
								int res = heroUnit->getResources();
								res -= 60;
								heroUnit->setResources(res);
							}
							else
								notEnoughResourceLerp = 1.0f;
						}
					}
					else
					{
						jGUI->SetActive("InGame_MiddleButton0", false);
						jGUI->SetActive("InGame_MiddleInfo", true);
						jGUI->SetText("InGame_MiddleInfo", "Conquer This Bank First");
					}
					break;

				case Type::GoldMine:
					typeName = "Gold Mine";
					jGUI->SetActive("InGame_MiddleInfo", true);
					jGUI->SetText("InGame_MiddleInfo", "Gold Available: " + std::to_string(unit->getResources()));
					break;

				default:
					typeName = std::to_string(unit->getType());
					jGUI->SetActive("InGame_Middle", false, true);
				}
			}

			jGUI->SetText("InGame_SelectedLabel", typeName);
		}
		else
		{
			jGUI->SetActive("InGame_MiddleInfo", false);
			jGUI->SetActive("InGame_SelectedLabel", true);
			jGUI->SetActive("InGame_Middle", false, true);
			jGUI->SetText("InGame_SelectedLabel", "Multiple");
		}

		if (gamemanager.gameState == GAME_STATE::MEDIUM_CIRCLE_STATE)
		{
			if (rof_state != 1)
			{
				rof_state = 1;
				ringOfFireAnnounce = 1.0f;
			}
		}
		else if (gamemanager.gameState == GAME_STATE::SMALL_CIRCLE_STATE)
		{
			if (rof_state != 2)
			{
				rof_state = 2;
				ringOfFireAnnounce = 1.0f;
			}
		}

		if (ringOfFireAnnounce <= 0.0f)
		{
			if (notEnoughResourceLerp > 0.0f)
				notEnoughResourceLerp -= Time.getDeltaTime() / 5;
			else
				notEnoughResourceLerp = 0.0f;

			jGUI->SetText("InGame_NotEnoughResource", "NOT ENOUGH RESOURCES");
			jGUI->SetAlpha("InGame_NotEnoughResource", notEnoughResourceLerp, true);
		}
		else
		{
			if (ringOfFireAnnounce > 0.0f)
				ringOfFireAnnounce -= Time.getDeltaTime() / 7;
			else
				ringOfFireAnnounce = 0.0f;

			jGUI->SetText("InGame_NotEnoughResource", "!!! RING OF FIRE CLOSING IN !!!");
			jGUI->SetAlpha("InGame_NotEnoughResource", notEnoughResourceLerp, true);
		}

		if (gamemanager.gameState == GAME_STATE::GAME_OVER_MENU)
		{
			if (gameOverLerp < 1.0f)
				gameOverLerp += Time.getDeltaTime();

			if (!onceKickToMainMenu)
			{
				onceKickToMainMenu = true;
				kickToMainMenu = 5.0f; // timer of 5 seconds
			}

			kickToMainMenu -= Time.getDeltaTime();

			if (kickToMainMenu <= 0.0f)
			{
				gamemanager.gameState = GAME_STATE::MAIN_MENU;
			}

			jGUI->SetText("GameOverScreen", "GAME OVER");
			jGUI->SetImage("GameOverScreen", "LoseScreen");
			jGUI->SetColour("GameOverScreen", ImColor(255, 0, 0));
			jGUI->SetActive("GameOverScreen", true);
			jGUI->SetAlpha("InGame", jGUI->Lerp(1, 0, gameOverLerp), true, true);
			jGUI->SetAlpha("GameOverScreen", jGUI->Lerp(0, 1, gameOverLerp), true);
		}
		else if (gamemanager.gameState == GAME_STATE::WIN_MENU)
		{
			if (gameOverLerp < 1.0f)
				gameOverLerp += Time.getDeltaTime();

			if (!onceKickToMainMenu)
			{
				onceKickToMainMenu = true;
				kickToMainMenu = 5.0f; // timer of 5 seconds
			}

			kickToMainMenu -= Time.getDeltaTime();

			if (kickToMainMenu <= 0.0f)
			{
				gamemanager.gameState = GAME_STATE::MAIN_MENU;
			}

			jGUI->SetText("GameOverScreen", "YOU WON");
			jGUI->SetImage("GameOverScreen", "WinScreen");
			jGUI->SetColour("GameOverScreen", ImColor(0, 255, 0));
			jGUI->SetActive("GameOverScreen", true);
			jGUI->SetAlpha("InGame", jGUI->Lerp(1, 0, gameOverLerp), true, true);
			jGUI->SetAlpha("GameOverScreen", jGUI->Lerp(0, 1, gameOverLerp), true);
		}

		if (Input.GetKeyDown(KeyCode::C))
			gamemanager.gameState = GAME_STATE::MAIN_MENU;

		// HEALTH BAR -- RENDER LAST SINCE IT'S NOT IMPORTANT
		for (size_t i = 0; i < coreEngine->objectsToRender->size(); i++)
		{
			GameObject* go = coreEngine->objectsToRender->at(i);
			Vector3 v3 = Vector3(jGUI->WorldToScreen(cam, go->transform.getPosition(), DirectX::XMMatrixIdentity()));

			if (go->getComponent<Unit>())
			{
				Unit* unit = go->getComponent<Unit>();

				if (unit->getType() == Type::GoldMine) continue;

				// HEALTH FOR EACH
				{
					float hp = unit->getHealthPoints() / unit->getMaxHealthPoints();
					float hp1 = unit->getHealthPoints();
					float hp2 = unit->getMaxHealthPoints();

					hp = hp1 / hp2;

					float hpBarLengthX = 30;
					float hpBarLengthY = 2.5f;
					float yOffset = 75.0f;

					float hpV1 = hpBarLengthX * hp;
					float hpV2 = jGUI->Lerp(-hpBarLengthX, 0, hp);
					float hpVF = hpV1 + hpV2;

					jGUI->BoxFilled(Vector2(v3.x - hpBarLengthX, (v3.y - hpBarLengthY) - yOffset), Vector2(v3.x + hpBarLengthX, (v3.y + hpBarLengthY) - yOffset));
					jGUI->BoxFilled(
						Vector2(v3.x - hpBarLengthX, (v3.y - hpBarLengthY) - yOffset),
						Vector2(v3.x + hpVF, (v3.y + hpBarLengthY) - yOffset),
						ImColor((int)jGUI->Lerp(255, 0, hp), (int)jGUI->Lerp(0, 255, hp), 0));
				}
			}
		}
	}
	else // MAIN MENU
	{
		heroUnit = nullptr;
		gameTime = 600;
		kickToMainMenu = 0.0f;
		onceKickToMainMenu = false;

		jGUI->SetActive("GameOverScreen", false);
		jGUI->SetAlpha("InGame", 1, true, true);
		jGUI->SetActive("MainMenu", true, true);
		if (jGUI->HasClicked("MainMenu_BeginGame"))
		{
			jGUI->ForceReloadJGUI();
			playerScript->SelectedUnits.clear();
			jGUI->SetActive("InGame_MiddleButton", false, true);
			jGUI->SetActive("InGame_SelectedLabel", false);
			gamemanager.unitLists = std::vector<std::vector<Unit*>>(4, std::vector<Unit*>());
			gamemanager.buildingLists = std::vector<std::vector<Unit*>>(4, std::vector<Unit*>());

			// ADD PLAYER HERO
			playerScript->instantiate_Player();

			// ADD ENEMY HERO
			enemyObject = gScene.createEmptyGameObject();
			enemyNPC = new NPC();
			enemyNPC->instantiate_NPC();
			enemyObject->addComponent(enemyNPC);

			gamemanager.addBuildings();
			gamemanager.gameState = GAME_STATE::START_STATE;
		}

		if (jGUI->HasClicked("MainMenu_ExitGame"))
		{
			coreEngine->IS_RUNNING = false;
		}
	}

	/*for (const auto& log : Debug.m_logger)
	{
	if ((log.level == 0) || (log.level == 1) || (log.level == 2))
	{
	ImGui::TextUnformatted(log.nfo.c_str());
	ImGui::Separator();
	}
	}*/

	// UNITY TROLL SPLASH SCREEN
	{
		if (includeSplashTroll)
		{
			if (stateTroll == 0) // start troll mode
			{
				if (unityTroll >= 5.f)
				{
					stateTroll = 1; // fade out troll mode
					unityTroll = 0.0f;
				}
				else
					unityTroll += Time.getDeltaTime();

				jGUI->BoxFilled(0.0f, 0.0f, 1.0f, 1.0f, ImColor(34, 44, 55));
			}
			else if (stateTroll == 1)
			{
				if (unityTroll >= 3.f)
				{
					stateTroll = 2; // fade out troll mode
					unityTroll = 0.0f;
				}
				else
					unityTroll += Time.getDeltaTime();


				float xMin = jGUI->Lerp(811.5f, 781.5f, (unityTroll / 3));
				float yMin = jGUI->Lerp(430.5f, 400.5f, (unityTroll / 3));

				float xMax = jGUI->Lerp(1108.5f, 1138.5f, (unityTroll / 3));
				float yMax = jGUI->Lerp(649.5f, 679.5f, (unityTroll / 3));

				int alpha = (int)jGUI->Lerp(0, 255, (unityTroll / 3));

				jGUI->BoxFilled(0.0f, 0.0f, 1.0f, 1.0f, ImColor(34, 44, 55));
				jGUI->Image(Vector2(xMin, yMin), Vector2(xMax, yMax), AssetManager.getTexture("troll_made_with_unity")->getTexture(), ImColor(255, 255, 255, alpha));
			}
			else if (stateTroll == 2)
			{
				if (unityTroll < 1.0f)
					unityTroll += Time.getDeltaTime();
				else
				{
					unityTroll = 10.0f;
					stateTroll = 3; // exit troll mode
					coreEngine->createTerrain();
					//gamemanager.addBuildings();
				}

				int alpha = (int)jGUI->Lerp(255, 0, unityTroll);

				jGUI->BoxFilled(0.0f, 0.0f, 1.0f, 1.0f, ImColor(34, 44, 55));
				jGUI->Image(Vector2(781.5f, 400.5f), Vector2(1138.5f, 679.5f), AssetManager.getTexture("troll_made_with_unity")->getTexture(), ImColor(255, 255, 255, alpha));
			}
		}
		else
		{
			if (stateTroll == 0)
			{
				stateTroll = 3; // exit troll mode
				coreEngine->createTerrain();
				//gamemanager.addBuildings();
			}
		}
	}

	coreEngine->camera->getComponent<Camera>()->width = screen.x;
	coreEngine->camera->getComponent<Camera>()->height = screen.y;
	coreEngine->camera->getComponent<Camera>()->aspectRatio = (screen.x / screen.y);

	coreEngine->SetViewport(screen.x, screen.y);//(WIDTH / 2, HEIGHT / 2);

	POINT mouse;
	GetCursorPos(&mouse);
	Input.fullscreenMousePosition = Vector2(mouse.x, mouse.y);

	deviceContext->OMSetRenderTargets(1, &coreEngine->gBackbufferRTV, coreEngine->m_depthStencilView);

	// CLEAR START
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	deviceContext->ClearRenderTargetView(coreEngine->gBackbufferRTV, clearColor);
	deviceContext->ClearDepthStencilView(coreEngine->m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);
	//deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // set the primitive topology

	// CLEAR END
	coreEngine->renderManager->Render(coreEngine->camera, coreEngine->objectsToRender, screen.x, screen.y);
	coreEngine->SetViewport();

	Input.mousePosition = Input.GetMousePosition();

	Debug.Clear(); // because the log can fill up, so clear that shit!
	jGUI->ClearRenderer();
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
