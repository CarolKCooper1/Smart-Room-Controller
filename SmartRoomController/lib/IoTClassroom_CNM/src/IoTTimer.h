#ifndef _IOTTIMER_H_
#define _IOTTIMER_H_

class IoTTimer {

  unsigned int _timerStart, _timerTarget;

  public:
    //IoT Constructor with no parameters
    //IotTimer() {}
    
    void startTimer(unsigned int msec) {
      _timerStart = millis();
      _timerTarget = msec;       
    }

    bool isTimerReady() {
      return ((millis() - _timerStart) >= _timerTarget);
    }
};

#endif // _IOTTIMER_H_