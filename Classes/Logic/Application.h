/* 
 * File:   Application.h
 * Author: thunderliu
 *
 * Created on 2014年2月4日, 上午3:00
 */

#ifndef __APPLICATION_H__
#define	__APPLICATION_H__


class CLogicApplication
{
protected:
    static CLogicApplication* sm_pSharedApplication;
    
public:
	CLogicApplication();
	virtual ~CLogicApplication();

protected:
    long m_lAnimationIntervalMS;  // micro second
    long m_lLastTimeMS;
    
public:
	void setAnimationInterval(double interval);
	int run();

	static CLogicApplication* sharedApplication();
    virtual long getCurMSec() = 0;
    virtual void SleepForMSec(unsigned int dwMSec) = 0;
    
    virtual bool applicationDidFinishLaunching();
    virtual void applicationTick(float fDt);
    
    M_SYNTHESIZE_READONLY(float, m_fElapsedTimeMS, ElapsedTime);
    M_SYNTHESIZE_BOOL(Bursting);
    
};

class CApplicationLog
{
public:
    static void Log(const char* pFormat, ...);
};

#define LOG(...) CApplicationLog::Log(__VA_ARGS__)

#endif	/* __APPLICATION_H__ */

