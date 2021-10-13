#include "conversation_system.hpp"

#include <iterator>

#include "entities.hpp"
#include "files.hpp"
#include "utils.hpp"

namespace ConversationSystem
{
    std::string start(const std::string &picture, const std::string &conversationFilePath)
    {
        const auto conversation = loadConversation(conversationFilePath);
        auto currentLine = conversation.at("start");

        std::string result;

        while (true)
        {
            std::vector<std::string> options;
            std::transform(
                currentLine.jumps.begin(),
                currentLine.jumps.end(),
                std::back_inserter(options),
                [&](const std::string &jump) -> std::string
                { return conversation.at(jump).text; });

            auto selection = Utils::pickOptionFromList(
                Utils::createConversationPrompt(currentLine.who, currentLine.text, picture), options);

            auto selectedLine = conversation.at(currentLine.jumps[selection]);
            if (selectedLine.jumps.size() == 0)
            {
                result = selectedLine.label;
                break;
            }

            currentLine = conversation.at(selectedLine.jumps[0]);
        }

        return result;
    }

    std::unordered_map<std::string, ConversationLine> loadConversation(const std::string &conversationFilePath)
    {
        const auto lines = Files::loadConversationFile(conversationFilePath);

        std::unordered_map<std::string, ConversationLine> conversation;

        for (const auto &line : lines)
        {
            conversation[line.label] = line;
        }

        return conversation;
    }
}  // namespace ConversationSystem