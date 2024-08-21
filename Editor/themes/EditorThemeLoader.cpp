#include "editor_pch.h"
#include "EditorThemeLoader.h"

#include "imgui/imgui.h"

#include "logger/Logger.h"
#include "rapidjson/document.h"
#include "utils/PathBuilder.h"

void EditorThemeLoader::LoadTheme()
{
    std::ifstream inStream(PathBuilder::GetPath("./Data/editor_themes/ember.json"));
    std::string json;

    if (inStream.is_open() && inStream.good())
    {
        std::stringstream strStream;

        strStream << inStream.rdbuf();

        json = strStream.str();
    }
    
    rapidjson::Document document;
    if (document.Parse(json.c_str()).HasParseError())
    {
        Logger::Log(LogCategory::ERROR, "Could not parse editor theme json file", "EditorThemeLoader::LoadTheme");

        inStream.close();
        
        return;
    }

    assert(document.IsObject());

    if (document["Colors"].IsObject())
    {
        rapidjson::Value& colors = document["Colors"].GetObject();

        int index = 0;
        for (rapidjson::Value::ConstMemberIterator iter = colors.MemberBegin(); iter != colors.MemberEnd(); ++iter)
        {
            ImVec4* imguiColors = ImGui::GetStyle().Colors;

            const rapidjson::Value& color = iter->value;
            if (!color.IsArray())
            {
                std::string error =  "Problematic value: ";
                error += iter->name.GetString();
                error += "\n";

                Logger::Log(LogCategory::ERROR, error, "EditorThemeLoader::LoadTheme");
                continue;
            }

            ImVec4 colorVector;
            for (rapidjson::SizeType i = 0; i < color.Size(); i++)
            {
                if (!color[i].IsFloat())
                {
                    std::string error =  "Value is not float: ";
                    error += iter->name.GetString();
                    error += "\n";

                    Logger::Log(LogCategory::ERROR, error, "EditorThemeLoader::LoadTheme");
                    continue;
                }
                
                *(&colorVector.x + i) = color[i].GetFloat();
            }
            
            imguiColors[index] = colorVector;
            index++;
        }
    }
    
    inStream.close();
}
