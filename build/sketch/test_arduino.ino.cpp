#include <Arduino.h>
#line 1 "d:\\Cprojects\\arduino\\electric_car\\test_arduino\\test_arduino.ino"
#define TrigPin A0
#define EcoPin A1
class LED
{
public:
    void ledHigh()
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
    }
    void ledLow()
    {
        digitalWrite(LED_BUILTIN, LOW);
        delay(2000);
    }
};

class ultraRed
{
private:
    int OUT = 8; // 红外OUT引脚接到的arduino引脚
public:
    // 假设检测到终点是高电平
    ultraRed()
    {
        pinMode(OUT, INPUT);
    }
    bool hasReachedFinal()
    {
        int sig = digitalRead(OUT);
        return sig == HIGH;
    }
};
class Motor
{
private:
    int IN1 = A5; // 298的控制引脚
    int IN2 = A6;
    int speed = 200;

public:
    Motor()
    {
        pinMode(IN1, OUTPUT);
        pinMode(IN2, OUTPUT);
    }
    void setSpeed(int speed)
    {
        this->speed = speed;
    }
    void forward(int t) // t的单位为毫秒
    {
        analogWrite(IN1, speed);
        digitalWrite(IN2, LOW);
        delay(t);
    }
    void backward(int t)
    {
        digitalWrite(IN1, LOW);
        analogWrite(IN2, speed);
        delay(t);
    }
    void stop(int t)
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        delay(t);
    }
};

class Echo
{
private:
    const float m_MinDis = 10;

public:
    Echo()
    {
        pinMode(TrigPin, OUTPUT);
        pinMode(EcoPin, INPUT);
        digitalWrite(TrigPin, LOW);
    }
    bool Tick()
    {
        send();
        return CanRun();
    }

private:
    void send()
    {
        digitalWrite(TrigPin, HIGH);
        // 维持10毫秒高电平用来产生一个脉冲
        delayMicroseconds(10);
        digitalWrite(TrigPin, LOW);
    }
    bool CanRun()
    {
        float time_ = pulseIn(EcoPin, HIGH);
        float dis = time_ / 58;
        Serial.println(dis);
        if (dis < m_MinDis)
        {
            return true;
        }
        return false;
    }
};

Motor motor;
Echo echo;
ultraRed ur;
bool start = false;
bool stop = false;
#line 115 "d:\\Cprojects\\arduino\\electric_car\\test_arduino\\test_arduino.ino"
void setup();
#line 120 "d:\\Cprojects\\arduino\\electric_car\\test_arduino\\test_arduino.ino"
void loop();
#line 115 "d:\\Cprojects\\arduino\\electric_car\\test_arduino\\test_arduino.ino"
void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (!start)
    {
        start = echo.Tick();
        delay(50);
    }
    else
    {
        if (!stop) // 如果没有接收到停止信号
        {
            // 执行前进的逻辑
            motor.forward(50); // 设置最短前进时间
            stop = ur.hasReachedFinal();
        }
        else // 执行停止逻辑
        {
            motor.backward(500); // 反转500ms以达到刹车的效果
            return;              // 小车停止运动
        }
    }
}

