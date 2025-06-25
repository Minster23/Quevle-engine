#include <core/interface/interface.hpp>

using namespace QuavleEngine;

void interface::consolePanel(){
    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    for (int i = 0; i < message.size(); i++){
        if(message[i].type == "Warning"){
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", message[i].text.c_str());
        }else if(message[i].type == "Debug"){
            ImGui::TextColored(ImVec4(236.0f / 255.0f, 1.0f, 0.0f, 1.0f), "%s", message[i].text.c_str());
        }
        else if(message[i].type == "Info"){
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 29.0f / 255.0f, 1.0f), "%s", message[i].text.c_str());
        }else{
            ImGui::Text("Kayaknya error");
        }
    }
    ImGui::End();
}

void interface::inputDebug(const std::string& typeOfDebug, const std::string& text){
    messageDebug newMassage;
    newMassage.text = text;
    newMassage.type = typeOfDebug;
    message.push_back(newMassage);
}