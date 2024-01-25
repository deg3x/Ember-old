#include "editor_pch.h"
#include "Console.h"

#include "logger/Logger.h"
#include "themes/EditorTheme.h"

Console::Console(Editor* owner) : EditorTab(owner)
{
    title  = "Console";
    flags |= ImGuiWindowFlags_NoScrollbar;
}

void Console::Tick()
{
    EditorTab::Tick();

    if (ImGui::Begin(title.c_str(), nullptr, flags))
    {
        DrawCategoryButtons();

        ImGui::Separator();
        
        DrawConsoleContent();
        
        ImGui::End();
    }
}

void Console::DrawCategoryButtons()
{
    bool popColor = false;
    
    if (showInfo)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, EditorTheme::ColorBlue);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, EditorTheme::ColorBlueHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, EditorTheme::ColorBlueActive);
        popColor = true;
    }
    if(ImGui::Button("Info"))
    {
        showInfo = !showInfo;
    }
    if (popColor)
    {
        ImGui::PopStyleColor(3);
        popColor = false;
    }
        
    ImGui::SameLine();

    if (showWarning)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, EditorTheme::ColorBlue);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, EditorTheme::ColorBlueHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, EditorTheme::ColorBlueActive);
        popColor = true;
    }
    if(ImGui::Button("Warning"))
    {
        showWarning = !showWarning;
    }
    if (popColor)
    {
        ImGui::PopStyleColor(3);
        popColor = false;
    }
        
    ImGui::SameLine();

    if (showError)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, EditorTheme::ColorBlue);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, EditorTheme::ColorBlueHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, EditorTheme::ColorBlueActive);
        popColor = true;
    }
    if(ImGui::Button("Error"))
    {
        showError = !showError;
    }
    if (popColor)
    {
        ImGui::PopStyleColor(3);
        popColor = false;
    }
}

void Console::DrawConsoleContent() const
{
    constexpr ImGuiTableFlags tableFlags     = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg;
    constexpr ImGuiTableColumnFlags colFlags = ImGuiTableColumnFlags_None;
    
    if (ImGui::BeginTable("Console Log", 3, tableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Timestamp", colFlags, 0.06f);
        ImGui::TableSetupColumn("Context", colFlags, 0.08f);
        ImGui::TableSetupColumn("Message", colFlags, 0.86f);
        ImGui::TableHeadersRow();
        
        for (const LogEntry& entry : Logger::GetConsoleLog())
        {
            if (entry.category == LogCategory::INFO)
            {
                if (!showInfo)
                {
                    continue;
                }

                ImGui::PushStyleColor(ImGuiCol_Text, EditorTheme::ColorTextInfo);
            }
            else if (entry.category == LogCategory::WARNING)
            {
                if (!showWarning)
                {
                    continue;
                }

                ImGui::PushStyleColor(ImGuiCol_Text, EditorTheme::ColorTextWarning);
            }
            else if (entry.category == LogCategory::ERROR)
            {
                if (!showError)
                {
                    continue;
                }

                ImGui::PushStyleColor(ImGuiCol_Text, EditorTheme::ColorTextError);
            }
            
            std::string timestamp  = "[" + std::to_string(entry.timestamp) + "]";
            std::string currentCtx = "[" + entry.context + "]";
            
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(timestamp.c_str());
            
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(currentCtx.c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(entry.message.c_str());
            
            ImGui::PopStyleColor(1);
        }
        
        ImGui::EndTable();
    }
}
