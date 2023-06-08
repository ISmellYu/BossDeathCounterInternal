#include "Menu.h"
#include <vector>

#include "Boss.h"
#include "imgui_helpers.h"
#include "imgui_notify.h"
#include "State.h"

namespace Menu
{
	static std::string preview_value;
	static int current_item_index = 0;

	void ShowBossList()
	{
		if (ImGui::BeginCombo("", preview_value.c_str()))
		{
			for (int n = 0; n < State::currentGame->bosses.size(); n++)
			{
				const bool is_selected = (current_item_index == n);
				if (ImGui::Selectable(State::currentGame->bosses[n]->bossName.c_str(), is_selected))
				{
					current_item_index = n;
					preview_value = State::currentGame->bosses[n]->bossName;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		if (ImGui::Button("Set", ImVec2(70, 26)))
		{
			if (!State::currentGame->bosses.empty())
			{
				State::currentGame->SetCurrentBoss(State::currentGame->bosses[current_item_index]->bossName);
				ImGui::InsertNotification({ImGuiToastType_Info, 4000, "Boss set!"});
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Remove", ImVec2(70, 26)))
		{
			if (!State::currentGame->bosses.empty())
			{
				// make sure that
				State::currentGame->RemoveBoss(State::currentGame->bosses[current_item_index]->bossName);
				ImGui::InsertNotification({ImGuiToastType_Info, 4000, "Boss removed!"});
				if (State::currentGame->bosses.size() == current_item_index)
				{
					current_item_index--;
				}
			}
		}
	}

	void ShowBossAdder(bool* p_open)
	{
		if (*p_open)
		{
			if (ImGui::Begin("Add boss", p_open, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static char bossName[50]{};
				ImGui::Text("Boss name");
				ImGui::SameLine();
				ImGui::InputText("", bossName, 50);
				ImGui::SameLine();
				if (ImGui::Button("Add", ImVec2(70, 26)))
				{
					if (bossName[0] != '\0')
					{
						State::currentGame->bosses.emplace_back(std::make_shared<Boss>(bossName));
						ImGui::InsertNotification({ImGuiToastType_Success, 4000, "Boss added!"});
					}
				}
				// ImGui::End();
			}
			ImGui::End();
		}
	}

	void HandlePreviewValue()
	{
		if (State::currentGame->bosses.empty())
		{
			preview_value = "";
		}
		else
		{
			if (current_item_index == -1)
				current_item_index = 0;
			preview_value = State::currentGame->bosses[current_item_index]->bossName;
		}
	}

	void ShowMenuBossInfo()
	{
		std::shared_ptr<Boss> currentBoss = State::currentGame->currentBoss;
		if (currentBoss == nullptr)
			return;
		ImGui::PushFont(customFont);
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Boss name: ").x));
		ImGui::Text("Boss name: ");
		ImGui::SameLine();
		ImGui::Text(currentBoss->bossName.c_str());

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Time: ").x));
		ImGui::Text("Time: ");
		ImGui::SameLine();
		auto timestamp = currentBoss->GetElapsedTime();
		ImGui::Text("%02d:%02d:%02d", timestamp.GetHours(), timestamp.GetMinutes(), timestamp.GetSeconds());

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Deaths: ").x));
		ImGui::Text("Deaths: ");
		ImGui::SameLine();
		ImGui::Text("%d", currentBoss->deaths);

		static ImVec4 textClr;
		static std::string statusText;
		switch (currentBoss->state)
		{
		case NotStarted:
			{
				textClr = {128, 128, 128, 255};
				statusText = "Waiting for start...";
				break;
			}
		case Started:
			{
				textClr = {0, 255, 0, 255};
				statusText = "Started";
				break;
			}
		case Paused:
			{
				textClr = {255, 255, 0, 255};
				statusText = "Paused";
				break;
			}
		case Dead:
			{
				textClr = {255, 0, 0, 255};
				statusText = "Dead";
				break;
			}
		case None:
			{
				textClr = {0, 0, 0, 255};
				statusText = "WTF";
				break;
			}
		}
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("State: ").x));
		ImGui::Text("State: ");
		ImGui::SameLine();
		ImGui::TextColored(textClr, statusText.c_str());


		// Buttons
		ImGui::SetCursorPosX(
			ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Start").x) - (ImGui::CalcTextSize("Pause").x));
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::RGBAtoIV4(0, 90, 0, 0.5f));
		if (ImGui::Button("Start", {70, 35}))
		{
			if (currentBoss->state == NotStarted)
			{
				if (currentBoss->StartBoss())
				{
					ImGui::InsertNotification({ImGuiToastType_Success, 4000, "Boss started!"});
				}
				else
				{
					ImGui::InsertNotification({
						ImGuiToastType_Error, 4000, "Something went wrong when tried to start the boss!"
					});
				}
			}
			else
			{
				ImGui::InsertNotification({ImGuiToastType_Error, 4000, "Cant start boss!"});
			}
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, {255, 255, 0, 0.5f});
		ImGui::Button("Pause", {70, 35});
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, {255, 0, 0, 0.5f});
		static bool showEndConfirmation = false;
		if (ImGui::Button("End", {70, 35}))
		{
			if (currentBoss->state == Paused || currentBoss->state == Started)
			{
				showEndConfirmation = !showEndConfirmation;
			}
			else
			{
				ImGui::InsertNotification({ImGuiToastType_Error, 4000, "Cant end boss!"});
			}
		}


		ImGui::PopStyleColor();
		ImGui::PopFont();


		if (showEndConfirmation)
		{
			if (ImGui::Begin("End confirm", &showEndConfirmation,
			                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration |
			                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
			                 ImGuiWindowFlags_NoSavedSettings))
			{
				ImGui::Text("Do you really want to end the boss fight?");
				if (ImGui::Button("Yes"))
				{
					if (currentBoss->state == Paused || currentBoss->state == Started)
					{
						if (currentBoss->EndBoss())
						{
							ImGui::InsertNotification({ImGuiToastType_Success, 4000, "Boss ended!"});
						}
						else
						{
							ImGui::InsertNotification({
								ImGuiToastType_Error, 4000, "Something went wrong when tried to end the boss!"
							});
						}
					}
					else
					{
						ImGui::InsertNotification({ImGuiToastType_Error, 4000, "Cant end boss!"});
					}
					showEndConfirmation = !showEndConfirmation;
				}

				ImGui::SameLine();
				if (ImGui::Button("No"))
				{
					showEndConfirmation = !showEndConfirmation;
				}
			}
			ImGui::End();
		}
	}

	void ShowMenu()
	{
		auto& x = ImGui::GetIO();
		auto d = ImGui::GetMainViewport();
		// ImGui::ShowDemoWindow();
		ImGui::Begin("BossDeathCounter");
		HandlePreviewValue();
		ShowBossList();


		static bool showBossAdder = false;
		if (ImGui::Button("Add boss"))
			showBossAdder = !showBossAdder;

		ImGui::Separator();
		ShowMenuBossInfo();

		ShowBossAdder(&showBossAdder);
		ImGui::End();
	}

	void ShowDeathCounter()
	{
		if (State::currentGame == nullptr)
			return;

		ImGuiWindowFlags flags;
		ImGui::SetWindowOverlayPos(0, &flags);

		if (ImGui::Begin("##DEATHCOUNTER", nullptr, flags))
		{
			ImGui::PushFont(overlayFont);
			ImGui::Text("%d", State::currentGame->deaths);
			ImGui::PopFont();
		}
		ImGui::End();
	}

	void ShowBossInfo()
	{
		std::shared_ptr<Boss> currentBoss = State::currentGame->currentBoss;
		if (currentBoss == nullptr)
			return;
		ImGuiWindowFlags flags;
		ImGui::SetWindowOverlayPos(1, &flags);

		// ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
		if (ImGui::Begin("##BOSSINFO", nullptr, flags))
		{
			ImGui::PushFont(overlayFont);
			ImGui::Text("%s", currentBoss->bossName.c_str());
			auto timestamp = currentBoss->GetElapsedTime();
			ImGui::Text("%02d:%02d:%02d", timestamp.GetHours(), timestamp.GetMinutes(), timestamp.GetSeconds());
			ImGui::Text("%d", currentBoss->deaths);
			ImGui::PopFont();
		}
		ImGui::End();
	}

	void ShowOverlay()
	{
		ShowDeathCounter();
		ShowBossInfo();
	}
}
