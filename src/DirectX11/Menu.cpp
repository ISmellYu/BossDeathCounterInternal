#include "Menu.h"
#include <vector>

#include "Boss.h"
#include "imgui_helpers.h"
namespace Menu
{
	static std::vector<std::string> bossNameVector;
	static std::string preview_value;
	static int current_item_index = 0;

	void ShowBossList()
	{
		if (ImGui::BeginCombo("", preview_value.c_str()))
		{
			for (int n = 0; n < bossNameVector.size(); n++)
			{
				const bool is_selected = (current_item_index == n);
				if (ImGui::Selectable(bossNameVector[n].c_str(), is_selected))
				{
					current_item_index = n;
					preview_value = bossNameVector[n];
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::Button("Set", ImVec2(70, 26));

		ImGui::SameLine();
		if (ImGui::Button("Remove", ImVec2(70, 26)))
		{
			if (!bossNameVector.empty())
			{
				// make sure that
				bossNameVector.erase(bossNameVector.begin() + current_item_index);

				if (bossNameVector.size() == current_item_index)
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
						bossNameVector.emplace_back(bossName);
					}
				}
				// ImGui::End();
			}
			ImGui::End();
		}
	}

	void HandlePreviewValue()
	{
		if (bossNameVector.empty())
		{
			preview_value = "";
		}
		else
		{
			preview_value = bossNameVector[current_item_index];
		}
	}

	void ShowMenuBossInfo()
	{
		ImGui::PushFont(customFont);
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Boss name: ").x));
		ImGui::Text("Boss name: ");
		ImGui::SameLine();
		ImGui::Text("Pontyfik");

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Time: ").x));
		ImGui::Text("Time: ");
		ImGui::SameLine();
		ImGui::Text("00:07:12");

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Deaths: ").x));
		ImGui::Text("Deaths: ");
		ImGui::SameLine();
		ImGui::Text("50");


		
		auto s = Dead;

		static ImVec4 textClr;
		static std::string statusText;
		switch (s)
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
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize("Start").x) - (ImGui::CalcTextSize("Pause").x));
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::RGBAtoIV4(0, 90, 0 , 0.5f));
		ImGui::Button("Start", {70, 35});
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, {255, 255, 0, 0.5f});
		ImGui::Button("Pause", {70, 35});
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, {255, 0, 0, 0.5f});
		static bool showEndConfirmation = false;
		if (ImGui::Button("End", {70, 35}))
			showEndConfirmation = !showEndConfirmation;

		ImGui::PopStyleColor();
		ImGui::PopFont();

		
		if (showEndConfirmation)
		{
			if (ImGui::Begin("End confirm", &showEndConfirmation, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
			{
				ImGui::Text("Do you really want to end the boss fight?");
				if (ImGui::Button("Yes"))
				{
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
		auto &x = ImGui::GetIO();
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
		ImGuiWindowFlags flags;
		ImGui::SetWindowOverlayPos(0, &flags);

		if (ImGui::Begin("##DEATHCOUNTER", 0, flags))
		{
			ImGui::PushFont(overlayFont);
			ImGui::Text("0");
			ImGui::PopFont();
			
		}
		ImGui::End();
	}

	void ShowBossInfo()
	{
		ImGuiWindowFlags flags;
		ImGui::SetWindowOverlayPos(1, &flags);

		// ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
		if (ImGui::Begin("##BOSSINFO", 0, flags))
		{
			ImGui::PushFont(overlayFont);
			ImGui::Text("Pontyfik");
			ImGui::Text("00:01:20");
			ImGui::Text("50");
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
