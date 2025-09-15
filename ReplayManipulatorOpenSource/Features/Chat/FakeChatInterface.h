#pragma once
#include "Framework/GuiFeatureBase.h"

struct FakeChatMessage
{
    float time = 0.f;
    std::string player;
    std::string message;
    bool sent = false;
};

class FakeChatInterface final : public GuiFeatureBase
{
  public:
    explicit FakeChatInterface(std::shared_ptr<GameWrapper> gw);

    void Render() override;
    void Tick();
    void Clear();

  private:
    void DisplayMessage(const FakeChatMessage& msg) const;

    std::vector<FakeChatMessage> messages_;
};
