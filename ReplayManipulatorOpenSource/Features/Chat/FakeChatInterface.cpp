#include "pch.h"

#include "FakeChatInterface.h"

FakeChatInterface::FakeChatInterface(std::shared_ptr<GameWrapper> gw) : GuiFeatureBase(std::move(gw), "Chat", DefaultVisibility::kBoth)
{
    gw_->HookEvent("Function TAGame.GameEvent_Soccar_TA.Tick", [this](...) { Tick(); });
}

void FakeChatInterface::Render()
{
    if (ImGui::Button("Add message"))
    {
        messages_.emplace_back();
    }

    if (ImGui::BeginTable("ChatMessages", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Player");
        ImGui::TableSetupColumn("Message");
        ImGui::TableSetupColumn("##Actions");
        ImGui::TableHeadersRow();

        for (size_t i = 0; i < messages_.size(); ++i)
        {
            auto& msg = messages_[i];
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::InputFloat(std::format("##time{}", i).c_str(), &msg.time, 0, 0, "%.2f");
            ImGui::TableSetColumnIndex(1);
            ImGui::InputText(std::format("##player{}", i).c_str(), &msg.player);
            ImGui::TableSetColumnIndex(2);
            ImGui::InputText(std::format("##text{}", i).c_str(), &msg.message);
            ImGui::TableSetColumnIndex(3);
            if (ImGui::Button(std::format("Delete##{}", i).c_str()))
            {
                messages_.erase(messages_.begin() + static_cast<long>(i));
                --i;
            }
        }

        ImGui::EndTable();
    }
}

void FakeChatInterface::Tick()
{
    if (!gw_->IsInReplay())
        return;

    auto replay = gw_->GetGameEventAsReplay();
    if (!replay)
        return;

    float current_time = replay.GetReplayTime();
    for (auto& msg : messages_)
    {
        if (!msg.sent && current_time >= msg.time)
        {
            DisplayMessage(msg);
            msg.sent = true;
        }
    }
}

void FakeChatInterface::DisplayMessage(const FakeChatMessage& msg) const
{
    gw_->Toast(msg.player, msg.message);
}

void FakeChatInterface::Clear()
{
    messages_.clear();
}
