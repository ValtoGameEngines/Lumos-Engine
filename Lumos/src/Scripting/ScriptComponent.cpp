#include "lmpch.h"
#include "ScriptComponent.h"
#include "LuaManager.h"
#include "App/Scene.h"
#include "App/Engine.h"

namespace Lumos
{
    ScriptComponent::ScriptComponent(const String& fileName, Scene* scene)
    {
        m_Scene = scene;
        m_FileName = fileName;
        
        Init();
    }

    void ScriptComponent::Init()
    {
        LoadScript(m_FileName);
    }

    ScriptComponent::~ScriptComponent()
    {
        if(m_Env && m_Env["OnRelease"])
            m_Env["OnRelease"]();
    }

    void ScriptComponent::LoadScript(const std::string& fileName)
    {
        String physicalPath;
        if (!VFS::Get()->ResolvePhysicalPath(fileName, physicalPath))
        {
            Debug::Log::Error("Failed to Load Lua script {0}", fileName );
            return;
        }

        m_Env = sol::environment(LuaManager::Instance()->GetState(), sol::create, LuaManager::Instance()->GetState().globals());

        auto loadFileResult = LuaManager::Instance()->GetState().script_file(physicalPath, m_Env, sol::script_pass_on_error);
        if (!loadFileResult.valid())
        {
            sol::error err = loadFileResult;
            Debug::Log::Error("Failed to Execute Lua script {0}" , physicalPath );
            Debug::Log::Error("Error : {0}", err.what());
            m_Errors.push_back(String(err.what()));
        }
                
        if(m_Scene)
            m_Env["CurrentScene"] = m_Scene;
    
        sol::protected_function onInitFunc = m_Env["OnInit"];
          
        if(onInitFunc)
        {
          sol::protected_function_result result = onInitFunc.call();
          if (!result.valid())
          {
              sol::error err = result;
              Debug::Log::Error("Failed to Execute Script Lua Init function" );
              Debug::Log::Error("Error : {0}", err.what());
          }
        }


        m_UpdateFunc = m_Env["OnUpdate"];

    }

    void ScriptComponent::Update(float dt)
    {
        if(m_UpdateFunc)
        {
           sol::protected_function_result result = m_UpdateFunc.call(dt);
           if (!result.valid())
           {
               sol::error err = result;
               Debug::Log::Error("Failed to Execute Script Lua OnUpdate" );
               Debug::Log::Error("Error : {0}", err.what());
           }
        }
    }

    void ScriptComponent::Reload()
    {
        if(m_Env && m_Env["OnRelease"])
            m_Env["OnRelease"]();
        
        Init();
    }

    void ScriptComponent::Load(const String& fileName)
    {
        if(m_Env && m_Env["OnRelease"])
            m_Env["OnRelease"]();
        
        m_FileName = fileName;
        Init();
    }
}
