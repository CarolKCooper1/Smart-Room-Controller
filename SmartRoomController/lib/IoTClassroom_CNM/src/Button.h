#ifndef _BUTTON_H_
#define _BUTTON_H_

class Button {
  int _buttonPin;
  int _prevButtonState;
  bool _pullUp;

  public:
    Button(int buttonPin, bool pullUp=false) {
      _buttonPin = buttonPin;
      _pullUp = pullUp;

      if(pullUp) {
        pinMode(_buttonPin,INPUT_PULLUP);
      }
      else {
        pinMode(_buttonPin,INPUT_PULLDOWN);       
      }
    }

    bool isPressed() {
      bool _buttonState;

      _buttonState = digitalRead(_buttonPin);
      if(_pullUp) {
        _buttonState = !_buttonState;
      }
      return _buttonState;
    }

    bool isClicked() {
      bool _buttonState, _clicked;

      _buttonState = digitalRead(_buttonPin);
      if(_pullUp) {
        _buttonState = !_buttonState;
      }
      if(_buttonState != _prevButtonState) {
        _clicked = _buttonState;
      }
      else {
        _clicked = false;
      }
      _prevButtonState=_buttonState;
      return _clicked;
    }
};

#endif // _BUTTON_H_