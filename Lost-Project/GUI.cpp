#include "GUI.h"
#include "game.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void GUI::Init() {
}

int GetEnemyLeft() {
	Game& game = Game::getInstance();
	std::vector<uint16_t> enemies = game.tagStore.getEntities(Tag::Enemy);
	int left = 0;
	for (uint16_t e : enemies) {
		if (!game.entityManager.entities[e].isDestroy) left++;
	}
	return left;
}

void RenderUI(float dt) {
	Game& game = Game::getInstance();
    ImGuiIO& io = ImGui::GetIO();

    // ─── WINDOW 1: PLAYER STATS ───────────────────────────────────────────
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280, 0), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.75f);
    ImGui::Begin("Player Stats", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse
    );

	uint16_t player = game.tagStore.getEntity(Tag::Player);
    StatComponent& stat = game.statStore.get(player);
    StatModifierComponent& mods = game.statModifierStore.get(player);

    // health bar — green, thick
    float healthPct = stat.finalHealth / stat.baseHealth;
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "HP");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.2f, 0.1f, 1.0f));
    ImGui::ProgressBar(healthPct, ImVec2(-1, 16),
        (std::to_string((int)stat.finalHealth) + " / " +
            std::to_string((int)stat.baseHealth)).c_str());
    ImGui::PopStyleColor(2);

    // stamina bar — blue, thin
    float staminaPct = stat.currentStamina / stat.stamina;
    ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "SP");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.4f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.2f, 1.0f));
    ImGui::ProgressBar(staminaPct, ImVec2(-1, 8), "");  // thin — no label
    ImGui::PopStyleColor(2);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // final damage
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.1f, 1.0f), "DMG");
    ImGui::SameLine();
    ImGui::Text("%.2f", stat.finalDamage);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // modifier list
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Modifiers (%d)",
        (int)mods.modifiers.size());

    ImGui::BeginChild("ModList", ImVec2(0, 120), true);
    for (auto& m : mods.modifiers) {
        // stat type label
        const char* statName = "?";
        switch (m.statType) {
        case StatType::Health:      statName = "Health";      break;
        case StatType::Damage:      statName = "Damage";      break;
        case StatType::Speed:       statName = "Speed";       break;
        case StatType::RunSpeed:    statName = "Run Speed";   break;
        case StatType::AttackSpeed: statName = "Atk Speed";   break;
        case StatType::DamageReduction: statName = "Def";     break;
        case StatType::Stamina:     statName = "Stamina";     break;
        }

        // modify type symbol
        const char* symbol = "+";
        ImVec4 color = ImVec4(0.4f, 1.0f, 0.4f, 1.0f);  // green = add
        if (m.modifyType == ModifyType::Subtract) {
            symbol = "-";
            color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);    // red = subtract
        }
        else if (m.modifyType == ModifyType::Multiply) {
            symbol = "x";
            color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);    // yellow = multiply
        }

        ImGui::TextColored(color, "[%s] %s %s%.2f",
            m.source.c_str(), statName, symbol, m.value);
    }
    if (mods.modifiers.empty())
        ImGui::TextDisabled("no modifiers");
    ImGui::EndChild();

    ImGui::End();

    // ─── WINDOW 2: WAVE INFO ──────────────────────────────────────────────
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 210, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 0), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.75f);
    ImGui::Begin("Wave", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse
    );

    // wave number — mock value for now
    static int currentWave = 1;
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "WAVE");
    ImGui::SameLine();
    ImGui::Text("%d", game.waveSystem.currentWave);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // enemy count
    int enemyLeft = game.waveSystem.GetEnemyLeftInThisWaveCount();
    if (enemyLeft > 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Enemies");
        ImGui::SameLine();
        ImGui::Text("%d", enemyLeft);

        // visual enemy count bar
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.1f, 0.1f, 1.0f));
        float enemyPct = (float)enemyLeft / game.waveSystem.maxEnemyCountInThisWave;  // assuming max 10 enemies
        ImGui::ProgressBar(enemyPct, ImVec2(-1, 8), "");
        ImGui::PopStyleColor(2);
    }
    //else {
    //    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Wave Clear!");
    //    // next wave button — mock
    //    if (ImGui::Button("Next Wave >>", ImVec2(-1, 0)))
    //        currentWave++;
    //}

    ImGui::End();
}

void GUI::Update(float dt) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

	RenderUI(dt);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
