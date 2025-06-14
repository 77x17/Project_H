#pragma once

#include "QuestObjective.hpp"

class FinishedObjective : public QuestObjective {
public:
    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;
};