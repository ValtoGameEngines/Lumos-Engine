// for the license, see the end of the file
#pragma once

#include <set>
#include <map>

#include <entt/entt.hpp>
#include <imgui.h>
#include <IconFontCppHeaders/IconsFontAwesome5.h>

namespace MM {

template <class Component, class EntityType>
void ComponentEditorWidget(entt::basic_registry<EntityType>& registry, EntityType entity) {}

template <class Component, class EntityType>
void ComponentAddAction(entt::basic_registry<EntityType>& registry, EntityType entity)
{
    registry.template emplace<Component>(entity);
}

template <class Component, class EntityType>
void ComponentRemoveAction(entt::basic_registry<EntityType>& registry, EntityType entity)
{
    registry.template remove<Component>(entity);
}

template<typename EntityType>
class ImGuiEntityEditor {
	private:
        using Registry = entt::basic_registry<EntityType>;


        struct ComponentInfo {
            using Callback = std::function<void(Registry&, EntityType)>;
            std::string name;
            Callback widget, create, destroy;
        };
    
    private:
        using ComponentTypeID = ENTT_ID_TYPE;

        std::map<ComponentTypeID, ComponentInfo> component_infos;

        bool entityHasComponent(Registry& registry, EntityType& entity, ComponentTypeID type_id)
        {
            ComponentTypeID type[] = { type_id };
            return registry.runtime_view(std::cbegin(type), std::cend(type)).contains(entity);
        }
    
	public:
    
        template <class Component>
        ComponentInfo& registerComponent(const ComponentInfo& component_info)
        {
            auto index = entt::type_info<Component>::id();
            auto [it, insert_result] = component_infos.insert_or_assign(index, component_info);
            assert(insert_result);
            return std::get<ComponentInfo>(*it);
        }

        template <class Component>
        ComponentInfo& registerComponent(const std::string& name, typename ComponentInfo::Callback widget)
        {
            return registerComponent<Component>(ComponentInfo{
                name,
                widget,
                ComponentAddAction<Component, EntityType>,
                ComponentRemoveAction<Component, EntityType>,
            });
        }

        template <class Component>
        ComponentInfo& registerComponent(const std::string& name)
        {
            return registerComponent<Component>(name, ComponentEditorWidget<Component, EntityType>);
        }
    
		// calls all the ImGui functions
		// call this every frame
		void RenderImGui(Registry& registry, typename Registry::entity_type& e)
		{
			if (e != entt::null)
			{
				std::map<ComponentTypeID, ComponentInfo> has_not;
                for (auto& [component_type_id, ci] : component_infos)
                {
                    if (entityHasComponent(registry, e, component_type_id))
                    {
                        ImGui::PushID(component_type_id);

                        std::string label = ci.name;
                        
                        bool open = ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap);

                        ImGui::SameLine(ImGui::GetWindowWidth() - 22.0f);

                        if (ImGui::Button((ICON_FA_COG"##" + label).c_str()))
                            ImGui::OpenPopup(("Remove Component" + label).c_str());

                        bool removed = false;
                        if (ImGui::BeginPopup(("Remove Component" + label).c_str(), 3))
                        {
                            if (ImGui::Selectable(("Remove##" + label).c_str()))
                            {
                                ci.destroy(registry, e);
                                removed = true;
                            }
                            ImGui::EndPopup();
                            if (removed)
                            {
                                ImGui::PopID();
                                continue;
                            }
                        }
                        if(open)
                        {
                            ImGui::PushID("Widget");
                            ci.widget(registry, e);
                            ImGui::PopID();
                        }
                        ImGui::PopID();
                    }
                    else
                    {
                        has_not[component_type_id] = ci;
                    }
                }

				if (!has_not.empty()) 
				{
					if (ImGui::Button("+ Add Component")) 
					{
						ImGui::OpenPopup("add component");
					}

					if (ImGui::BeginPopup("add component")) 
					{
						ImGui::TextUnformatted("Available:");
						ImGui::Separator();
                        
                        for (auto& [component_type_id, ci] : has_not)
                        {
                            ImGui::PushID(component_type_id);
                            if (ImGui::Selectable(ci.name.c_str()))
                            {
                                ci.create(registry, e);
                            }
                            ImGui::PopID();
                        }

						ImGui::EndPopup();
					}
				}
			}
		}
};

} // MM

// MIT License

// Copyright (c) 2019 Erik Scholz

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

