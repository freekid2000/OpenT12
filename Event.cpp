#include "OpenT12.h"

uint32_t EventTimerUpdate = 0;
void TimerUpdateEvent(void) {
    EventTimerUpdate = millis();
    pages_Tip_Display_timer = EventTimerUpdate;
}

void BoostButton_EventLoop(void) {

}

void TimerEventLoop(void) {
    //事件事件距离计时器
    static uint32_t TimerEventTimer = millis() - EventTimerUpdate;

    //更新BOOST提温事件
    BoostButton_EventLoop();
    static uint32_t BoostEventTimer = 0;
        // if (BoostEvent && TimerEventTimer)
}

void SYS_StateCode_Update(void) {
    //更新状态码
    if (PID_Output > 180) {
        //加热状态
        TempCTRL_Status = TEMP_STATUS_HEAT;
    }else {
        if (TempGap < 10) {
            //温差接近目标值：正常
            TempCTRL_Status = TEMP_STATUS_WORKY;
        }else{
            //进行PID微调：维持
            TempCTRL_Status = TEMP_STATUS_HOLD;
        }
    }

    if (ERROREvent==true) {
        //系统错误
        TempCTRL_Status = TEMP_STATUS_ERROR;
        //强制关闭输出
        PID_Output = 0;
        //暂时消除本次的状态码
        ERROREvent = false;
    }else if (BoostEvent) {
        //快速升温事件
        TempCTRL_Status = TEMP_STATUS_BOOST;
        //短时功率加成
        PID_Output += BoostTemp;
    }else if (SleepEvent) {
        //烙铁进入休眠模式
        TempCTRL_Status = TEMP_STATUS_SLEEP;
        //关闭输出
        PID_Output = 0;
    }else if (ShutdownEvent) {
        //烙铁进入停机模式
        TempCTRL_Status = TEMP_STATUS_OFF;
        //关闭输出
        PID_Output = 0;
    }


    
}