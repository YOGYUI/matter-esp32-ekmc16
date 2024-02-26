#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXLEN_LOG_MSG  1024

typedef enum
{
    Info = 0,
	Warning,
	Error,
	Debug,
	Exception
} eLogType;

class CLogger
{
public:
    CLogger();
    virtual ~CLogger();

public:
    /**
     * @brief 객체 인스턴스 호출 메서드
     * @param[in] logtype 로그 타입
     * @param[in] funcname 함수 이름
     * @param[in] filename 파일 이름
     * @param[in] fileline 라인 넘버
     * @return CLogger* 
     */
    static CLogger* Instance(eLogType logtype, const char* funcname, const char* filename, const unsigned long fileline);

    /**
     * @brief 
     */
    static void Release();

    /**
     * @brief 로그 기록 메서드
     * @param[in] msg 포맷 문자열
     * @param[in] ... arguments 
     */
    void Log(const char* msg, ...);

private:
    bool m_valid_funcinfo;
    static CLogger* _instance;
    eLogType m_eLogType;
    std::string m_funcname;
    std::string m_filename;
    unsigned long m_fileline;

    /**
     * @brief 실제로 콘솔 등에 로그를 기록하는 메서드
     * @param[in] msg 
     */
    void Process(std::string msg);
};

/**
 * @brief 
 * @param logtype 
 * @param funcname 
 * @param filename 
 * @param fileline 
 * @return CLogger* 
 */
inline CLogger* _GetLogger(eLogType logtype, const char* funcname, const char* filename, const unsigned long fileline) {
    return CLogger::Instance(logtype, funcname, filename, fileline);
}

/**
 * @brief 
 */
inline void ReleaseLogger() {
    CLogger::Release();
}

#define GetLoggerBase() _GetLogger(eLogType::Info, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define GetLogger(n) _GetLogger(n, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define GetLoggerM(n) _GetLogger(n, nullptr, nullptr, 0)

#ifdef __cplusplus
};
#endif

#endif