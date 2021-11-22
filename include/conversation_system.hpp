#pragma once
#ifndef conversation_system_hpp
#define conversation_system_hpp

#include <string>
#include <unordered_map>

struct ConversationLine;

namespace ConversationSystem
{
    inline constexpr auto kConversationResultPositive = "end_yes";
    inline constexpr auto kConversationResultNegative = "end_no";

    auto start(const std::string &picture, const std::string &conversationFilePath) -> std::string;

    [[nodiscard]] auto loadConversation(const std::string &conversationFilePath)
        -> std::unordered_map<std::string, ConversationLine>;
}  // namespace ConversationSystem

#endif