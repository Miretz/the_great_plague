#pragma once
#ifndef conversation_system_hpp
#define conversation_system_hpp

#include <string>
#include <unordered_map>

struct ConversationLine;

namespace ConversationSystem
{
    const std::string RESULT_POSITIVE = "end_yes";
    const std::string RESULT_NEGATIVE = "end_no";
    
    std::string start(const std::string &picture, const std::string &conversationFilePath);

    std::unordered_map<std::string, ConversationLine> loadConversation(const std::string &conversationFilePath);
}  // namespace ConversationSystem

#endif