#include "logger.h"
#ifndef UNIT_TEST
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#endif
#include <vector>
#include <cstdarg>

CLogger* CLogger::_instance;
static const char *TAG = "logger";

CLogger::CLogger()
{
    m_eLogType = eLogType::Info;
    m_funcname = "";
    m_filename = "";
    m_fileline = 0;
    m_valid_funcinfo = false;
}

CLogger::~CLogger()
{
    
}

CLogger* CLogger::Instance(eLogType logtype, const char* funcname, const char* filename, const unsigned long fileline)
{
    if (!_instance) {
        _instance = new CLogger();
    }

    _instance->m_eLogType = logtype;
    if (funcname) {
        _instance->m_valid_funcinfo = true;
        _instance->m_funcname.assign(funcname, strlen(funcname));
        _instance->m_fileline = fileline;

        std::string temp;
        temp.assign(filename, strlen(filename));
        size_t pos = temp.rfind("/");
        if (pos != std::string::npos) {
            _instance->m_filename.assign(temp.substr(pos + 1, temp.length()));
        } else {
            _instance->m_filename.assign("?");
        }
    } else {
        _instance->m_valid_funcinfo = false;
    }

    return _instance;
}

void CLogger::Release()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

void CLogger::Log(const char* msg, ...)
{
    va_list vaArgs;
    va_start(vaArgs, msg);

    va_list vaArgsCopy;
    va_copy(vaArgsCopy, vaArgs);
    const int len = vsnprintf(nullptr, 0, msg, vaArgsCopy);
    va_end(vaArgsCopy);

    std::vector<char> zc(len + 1);
    vsnprintf(zc.data(), zc.size(), msg, vaArgs);
    va_end(vaArgs);

    Process(std::string(zc.data(), len));
}

void CLogger::Process(std::string msg)
{
    std::string fullmsg;
    std::string funcname;
    size_t colons, begin, end;
    char szlog[256]{0,};

    if (m_valid_funcinfo) {
        colons = m_funcname.find("::");
        if (colons != std::string::npos) {
            begin = m_funcname.substr(0, colons).rfind(" ") + 1;
            end = m_funcname.rfind("(") - begin;
        } else {
            begin = m_funcname.substr(0, colons).find(" ") + 1;
            end = m_funcname.rfind("(") - begin;
        }

        if (begin != std::string::npos && end != std::string::npos) {
            funcname = m_funcname.substr(begin, end);
        } else {
            funcname = m_funcname;
        }

        snprintf(szlog, sizeof(szlog), "[%s] %s [%s:%lu]", funcname.c_str(), msg.c_str(), m_filename.c_str(), m_fileline);
    } else {
        snprintf(szlog, sizeof(szlog), "%s", msg.c_str());
    }
    
    switch (m_eLogType) {
	case eLogType::Info:
#ifndef UNIT_TEST
		ESP_LOGI(TAG, "%s", szlog);
#else
        printf("[I] %s\n", szlog);
#endif
		break;
	case eLogType::Warning:
#ifndef UNIT_TEST
        ESP_LOGW(TAG, "%s", szlog);
#else
        printf("[W] %s\n", szlog);
#endif
		break;
	case eLogType::Error:
#ifndef UNIT_TEST
        ESP_LOGE(TAG, "%s", szlog);
#else
        printf("[E] %s\n", szlog);
#endif
		break;
	case eLogType::Debug:
#ifndef UNIT_TEST
        ESP_LOGD(TAG, "%s", szlog);
#else
        printf("[D] %s\n", szlog);
#endif
		break;
	case eLogType::Exception:
#ifndef UNIT_TEST
        ESP_LOGE(TAG, "%s", szlog);
#else
        printf("[E] %s\n", szlog);
#endif
		break;
    default:
#ifndef UNIT_TEST
        ESP_LOGI(TAG, "%s", szlog);
#else
        printf("[I] %s\n", szlog);
#endif
        break;
	}
}
