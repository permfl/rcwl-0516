# Motion Detecton using RCWL-0516

The `MotionDetector` listens to changes in motions from the RCWL-0516 chip. The `OUT` pin on the chip
must be connected to some digital input pin. 

Once motion is detected, the virtual `onMotionStart()` is called. The `onMotionEnd()` method 
is called a given number of seconds after motion is no longer detected.


## License
```text
Copyright 2021 Per Magne Florvaag

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```


## Example
```c++

#include <Arduino.h>
#include <MotionDetector.h>


class SimpleDetector : public pmf::MotionDetector
{
  protected:
    void onMotionStart() const override {
      Serial.println("Motion detected");
    }

    void onMotionEnd() const override {
      Serial.println("Motion ended");
    }
};

constexpr auto MOTION_PIN = 2;
constexpr auto DELAY = 2;
SimpleDetector motion_detector;

void setup() {
  Serial.begin(9600);

  motion_detector.setDelay(DELAY);
  motion_detector.setPin(MOTION_PIN);
}

void loop() {
  motion_detector.update(); 
}
```