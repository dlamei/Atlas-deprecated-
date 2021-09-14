#include "atlpch.h"
#include "AtlasLog.h"

#include "Atlas/ImGui/AtlasTheme.h"

#include <string>
#include <deque>

namespace Atlas {

	std::array<ImVec4, (size_t)Utils::LogLevel::NONE> m_Colors;
	std::deque<std::string> m_Buffer;
	std::deque<Utils::LogLevel> m_LogLevels;

	ImGuiTextFilter m_Filter;
	int m_LineCount = 0;

	AtlasLog::AtlasLog()
	{
		m_AutoScroll = true;
		m_Colors[(size_t)Utils::LogLevel::ATL_TRACE] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_Colors[(size_t)Utils::LogLevel::ATL_INFO]	 = Atlas::ATL_LIGHT_GREEN_COL;
		m_Colors[(size_t)Utils::LogLevel::ATL_WARN]	 = Atlas::ATL_LIGHT_YELLOW_COL;
		m_Colors[(size_t)Utils::LogLevel::ATL_ERROR] = Atlas::ATL_LIGHT_RED_COL;
	}

	void AtlasLog::Clear()
	{
		m_Buffer.clear();
		m_LogLevels.clear();
		m_LineCount = 0;
	}

	void AtlasLog::AddLog(const std::string& log, Utils::LogLevel logLevel)
	{
		m_Buffer.push_back(log);
		m_LogLevels.push_back(logLevel);
		m_LineCount++;

		if (m_Buffer.size() > MAX_LOG_MESSAGES)
		{
			m_Buffer.pop_front();
			m_LogLevels.pop_front();
			m_LineCount = MAX_LOG_MESSAGES;
		}
	}

	void AtlasLog::Draw(const char* title, bool* p_open)
	{
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
			ImGui::EndPopup();
		}

		if (ImGui::Button("Options")) ImGui::OpenPopup("Options");

		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		ImGui::SameLine();
		m_Filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear) Clear();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		if (m_Filter.IsActive())
		{
		for (int line_no = 0; line_no < m_LineCount; line_no++)
			{
				if (m_Filter.PassFilter(m_Buffer[line_no].c_str()))
				{
					ImGui::PushStyleColor(ImGuiCol_Text, m_Colors[(size_t)m_LogLevels[line_no]]);
					ImGui::TextWrapped(m_Buffer[line_no].c_str());
					ImGui::PopStyleColor();
				}
			}


		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(m_LineCount);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, m_Colors[(size_t)m_LogLevels[line_no]]);
					ImGui::TextWrapped(m_Buffer[line_no].c_str());
					ImGui::PopStyleColor();
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}

}
