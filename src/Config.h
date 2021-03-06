namespace Config
{
    std::wstring GetConfigPath()
    {
        wchar_t path[MAX_PATH];
        ::GetModuleFileName(NULL, path, MAX_PATH);
        ::PathRenameExtension(path, _T(".ini"));
        return std::wstring(path);
    }

    std::wstring ini_path;

    int GetInt(const wchar_t *section, const wchar_t *name, int default_value = 0 )
    {
        return ::GetPrivateProfileInt(section, name, default_value, ini_path.c_str());
    }

    void SetInt(const wchar_t *section, const wchar_t *name, int value = 0 )
    {
        wchar_t buffer[256];
        ::wsprintf(buffer, L"%d", value);
        ::WritePrivateProfileString(section, name, buffer, ini_path.c_str());
    }

    std::wstring GetStr(const wchar_t *section, const wchar_t *name)
    {
        wchar_t buffer[256];
        ::GetPrivateProfileString(section, name, L"", buffer, sizeof(buffer), ini_path.c_str());
        return buffer;
    }
    
    void SetStr(const wchar_t *section, const wchar_t *name, const wchar_t *value)
    {
        ::WritePrivateProfileString(section, name, value, ini_path.c_str());
    }

    void SetString(const wchar_t *section, const wchar_t *value)
    {
        ::WritePrivateProfileSection(section, value, ini_path.c_str());
    }

    void Switch(const wchar_t *section, const wchar_t *name)
    {
        SetInt(section, name, !GetInt(section, name));
    }
    
    bool IsExclude(const wchar_t *section, HWND hwnd)
    {
        std::wstring path = GetHwndProcess(hwnd);
        if(!path.empty())
        {
            #define MAX_INI_SIZE 32767
            wchar_t exclude_program[MAX_INI_SIZE];
            ::GetPrivateProfileSectionW(section, exclude_program, MAX_INI_SIZE, ini_path.c_str());
            wchar_t *program = exclude_program;
            while (program && *program)
            {
                if(isEndWith(path.c_str(), program))
                {
                    return true;
                }
                program += wcslen(program) + 1;
            }
        }

        return false;
    }

    int SuperKey = 0;
    void ParseSuperKey()
    {
        // 读取超级键配置
        SuperKey = GetInt(L"Basic", L"SuperKey", 1);
        if(SuperKey)
        {
            switch(SuperKey)
            {
                case 1:
                    SuperKey = VK_MENU;
                    break;
                case 2:
                    SuperKey = VK_CONTROL;
                    break;
                case 3:
                    SuperKey = VK_SHIFT;
                    break;
                case 4:
                    SuperKey = VK_LWIN;
                    break;
                case 5:
                    SuperKey = VK_LBUTTON;
                    break;
                case 6:
                    SuperKey = VK_RBUTTON;
                    break;
                default:
                    SuperKey = VK_MENU;
                    break;
            }
        }
    }

    void Init()
    {
        ini_path = GetConfigPath();

        ParseSuperKey();
    }
}