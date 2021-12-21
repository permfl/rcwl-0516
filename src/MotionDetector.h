
#include <Arduino.h>

namespace pmf {
  /**
   * @brief Detect motion from RCWL-0516
   *
   * Call the method onMotionStart() when motion is detected
   * and onMotionEnd() 'delay' after motion is ended.
   */
  class MotionDetector
  {
  public:
    /**
     * @brief Set the delay between two consecutive motion detections.
     * @param delay The delay in seconds.
     */
    void setDelay(unsigned long delay)
    {
      m_delay = delay * 1000;
    }

    /**
     * @brief Set the pin for reading motion
     * @param pin Id of the digital pin connected to OUT on the sensor
     */
    void setPin(int pin)
    {
      m_pin = pin;
    }

    /** @brief Update the state of the motion detector */
    void update()
    {
      const auto has_motion = digitalRead(m_pin) == HIGH;
      if (m_triggered && !has_motion)
      {
        onPostTrigger();
        return;
      }

      if (has_motion)
      {
        if (!m_triggered)
        {
          // Only call on first trigger
          onMotionStart();
          m_triggered = true;
        }

        // Update time of motion as long as motion is detected
        // This is to prevent stopping the motion trigger m_delay ms
        // after the first detection of motion and not after m_delay
        // ms after the last detection of motion
        m_trigger_time = millis();
      }
    }

  protected:
    /** Called the first time motion is detcted */
    virtual void onMotionStart() const = 0;

    /** Called after m_delay ms after the last detection of motion */
    virtual void onMotionEnd() const = 0;

  private:
    /**
     * Called on every update if we have triggered on motion
     *
     * Checks if m_delay milliseconds has passed since motion was detected,
     * if so, stops the triggering and calls onMotionEnd()
     */
    void onPostTrigger()
    {
      const auto t = timeSinceTrigger();

      if (t >= m_delay)
      {
        m_triggered = false;
        m_trigger_time = 0;

        onMotionEnd();
      }
    }

    /**
     * @return The number if milliseconds since motion was triggered.
     * Zero if motion has not been triggered
     */
    unsigned long timeSinceTrigger()
    {
      if (!m_triggered)
      {
        return 0;
      }

      const auto current = millis();
      if (current < m_trigger_time)
      {
        return current + (0xFFFFFFFF - m_trigger_time);
      }

      return current - m_trigger_time;
    }

    unsigned long m_delay;
    int m_pin;

    bool m_triggered{ false };
    unsigned long m_trigger_time{ 0 };
  };
} // namespace pmf
