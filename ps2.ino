// Nhập các thư viện
#include <PS2X_lib.h> // Thư viện PS2X cho tay cầm PS2
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Classes
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
PS2X ps2x;

// PS2
#define PS2_DAT 12
#define PS2_CMD 13
#define PS2_SEL 15
#define PS2_CLK 14

// DC Motors
#define SHOOTER_MOTOR_PIN_A 10
#define SHOOTER_MOTOR_PIN_B 11
#define LEFT_WHEEL_MOTOR_PIN_A 12
#define LEFT_WHEEL_MOTOR_PIN_B 13
#define RIGHT_WHEEL_MOTOR_PIN_A 14
#define RIGHT_WHEEL_MOTOR_PIN_B 15

// Speed
//#define TOP_SPEED 4095
#define TOP_SPEED 2048
#define NORMAL_SPEED 2048


void setup()
{
  initilizeRobot();
}

void loop()
{
  robotFunction();
  delay(50); // Đặt độ trễ để tránh bị quá tải do vòng lặp
}

// Initilization
void initilizeRobot()
{
  Serial.begin(115200);
  initilizeMotors();
  setupPS2controller();
  Serial.println("Hoan thanh thiet lap");
}

void initilizeMotors()
{
  Wire.begin(); // Bắt đầu giao tiếp I2C
  pwm.begin(); // Khởi chạy drive của PWM
  pwm.setOscillatorFrequency(27000000); // Thiết lập tần số giao động
  pwm.setPWMFreq(1600); // Thiết lập tần số giao động là 1600Hz
  Wire.setClock(400000);
  for (int i = 8; i < 15; i++) // Đặt giá trị PWM ban đầu của các DC Motors là 0
  {
    pwm.setPWM(i, 0, 0);
  }
  pwm.setPWMFreq(50); // Đặt tần số PWM cho motor
}

void setupPS2controller()
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }
}

// Function
void robotFunction()
{
  ps2x.read_gamepad(0, 0); // Đọc trạng thái của tay cầm PS2
  manualControl(); // Xử lý các tín hiệu từ driver đến robot
}

void manualControl()
{
  buttonsHandler();
  movementHandler();
}

void buttonsHandler()
{
  crossButtonHandler();
}

void crossButtonHandler()
{
  if (ps2x.Button(PSB_CROSS)) // Nút xanh dương (nút X) bật / Quay thả hạt
  {
    int motorSpeed = TOP_SPEED;  // Tốc độ tối đa cho động cơ
    pwm.setPWM(SHOOTER_MOTOR_PIN_A, 0, motorSpeed); // Điều khiển motor bắn hạt
    pwm.setPWM(SHOOTER_MOTOR_PIN_B, 0, 0); // Điều khiển motor phía đối diện (dừng)

    unsigned long startMillis = millis();  // Lấy thời gian bắt đầu khi bấm nút
    while (millis() - startMillis < 50) {  // Quay trong 50ms
      // Động cơ quay trong thời gian ngắn rồi dừng lại
    }

    // Dừng động cơ sau 500ms
    pwm.setPWM(SHOOTER_MOTOR_PIN_A, 0, 0);
    pwm.setPWM(SHOOTER_MOTOR_PIN_B, 0, 0);
  }
}


void movementHandler()
{
  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
  {
    int leftJoystick = ps2x.Analog(PSS_LY); // Đọc giá trị trục y của joystick trái
    int rightJoystick = ps2x.Analog(PSS_RY); // Đọc giá trị trục y của joystick phải

    if (leftJoystick > 128)
    {
      // Chạy động cơ hướng về phía sau
      int speed = map(leftJoystick, 128, 255, 0, TOP_SPEED);
      pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_A, 0, speed);
      pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_B, 0, 0);
    }
    else if (leftJoystick < 128)
    {
      // Chạy động cơ hướng về phía trước
      int speed = map(leftJoystick, 128, 0, 0, TOP_SPEED);
      pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_A, 0, 0);
      pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_B, 0, speed);
    }
    else
    {
      // Dừng động cơ
      pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_A, 0, 0);
      pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_B, 0, 0);
    }

    if (rightJoystick > 128)
    {
      // Chạy động cơ hướng về phía sau
      int speed = map(rightJoystick, 128, 255, 0, TOP_SPEED);
      pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_A, 0, speed);
      pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_B, 0, 0);
    }
    else if (rightJoystick < 128)
    {
      // Chạy động cơ hướng về phía trước
      int speed = map(rightJoystick, 128, 0, 0, TOP_SPEED);
      pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_A, 0, 0);
      pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_B, 0, speed);
    }
    else
    {
      // Dừng động cơ
      pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_A, 0, 0);
      pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_B, 0, 0);
    }
  }
  if (!ps2x.Button(PSB_L1) && !ps2x.Button(PSB_R1)) {
    pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_A, 0, 0);
    pwm.setPWM(LEFT_WHEEL_MOTOR_PIN_B, 0, 0);
    pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_A, 0, 0);
    pwm.setPWM(RIGHT_WHEEL_MOTOR_PIN_B, 0, 0);
}

}
