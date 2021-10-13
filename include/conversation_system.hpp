#pragma once
#ifndef conversation_system_hpp
#define conversation_system_hpp

#include <string>
#include <unordered_map>

struct ConversationLine;

namespace ConversationSystem
{
    std::string start(const std::string &picture, const std::string &conversationFilePath);

    std::unordered_map<std::string, ConversationLine> loadConversation(const std::string &conversationFilePath);
}  // namespace ConversationSystem

#endif