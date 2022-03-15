#include <SoftwareSerial.h>

#include <Servo.h>
//***********************

//************************
Servo servo1;
Servo servo2;




#define BT_TXD 0
#define BT_RXD 1

//소프트웨어 UART 설정
SoftwareSerial BTSerial(BT_TXD, BT_RXD);

#define ENA_PIN   5 // ENA attach to pin5  
#define ENB_PIN   6// ENB attach to pin6
#define MOTOR_L_1 8 // MOTOR_L_1 attach to pin8
#define MOTOR_L_2 9 // MOTOR_L_1 attach to pin9
#define MOTOR_R_1 10 // MOTOR_R_1 attach to pin10
#define MOTOR_R_2 11 // MOTOR_R_1 attach to pin11

#define SERVO_TURN_PIN 2 //servo1 attach to pin2
#define SERVO_MEASURE_PIN 3 //servo2 attach to pin3
#define SENSOR_IR_L 4 //left obstacle module attach to pin4
#define SENSOR_IR_R 7 //right obstacle module attach to pin7
#define SENSOR_IR_M 12 //radar module attach to pin12

#define FORWARD 0  //define forward=0,car move forward
#define BACK    1 //define back=1,car move back
//set the rotate angle of servo2
#define ANGLE_RIGHT_MAX  130
#define ANGLE_RIGHT_HALF 110
#define ANGLE_LEFT_MAX  50
#define ANGLE_LEFT_HALF  70
#define ANGLE_SERVO2_MIDDLE 90
//set the rotate angle of servo1
#define ANGLE_SERVO1_MIDDLE 85
#define ANGLE_SERVO1_RIGHT_MAX 110
#define ANGLE_SERVO1_LEFT_MAX 50

unsigned char dir1 = 0;
unsigned char dir2 = 0;

#define N_DIS 180
unsigned char array_dis_l[91] = {0};
unsigned char array_dis_r[91] = {0};

unsigned char array_angle[2] = {110, 70};
unsigned char array_angle_r[5] = {30, 60, 90, 120, 150};
unsigned char array_angle_l[5] = {150, 120, 90, 60, 30};
unsigned int array_distance[N_DIS] = {0};
unsigned char flag = 0;
unsigned char testBlueServo=85;

unsigned char goRight = 110;
unsigned char goLeft = 70;
unsigned char goMiddle= 85;
unsigned char myPo=0;

unsigned char control=91;

// Initial command
int command = 0;

// PPWM Durations (in ms)
int on_time = 30;
int off_time = 70;


unsigned char speedl = 250, speedr = 250;   //속도 제어
unsigned char irValue = 0;
unsigned char count_l = 0;
unsigned char count_r = 0;
unsigned char count_0_l = 0;
unsigned char count_1_l = 0;
unsigned char count_0_r = 0;
unsigned char count_1_r = 0;
unsigned char irValueL = 1;
unsigned char irValueR = 1;

int stop_count=0;

void setup()
{


  //set below pins as output
  pinMode(ENA_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(MOTOR_L_1, OUTPUT);
  pinMode(MOTOR_L_2, OUTPUT);
  pinMode(MOTOR_R_1, OUTPUT);
  pinMode(MOTOR_R_2, OUTPUT);
  digitalWrite(SENSOR_IR_L, HIGH); //set left obstacle module high
  pinMode(SENSOR_IR_L, INPUT); //set left obstacle module output
  digitalWrite(SENSOR_IR_R, HIGH); //set right obstacle module high
  pinMode(SENSOR_IR_R, INPUT); //set right obstacle module output
 // digitalWrite(SENSOR_IR_M, HIGH); //set radar module high
//  pinMode(SENSOR_IR_M, INPUT); //set radar module output
  servo1.attach(SERVO_TURN_PIN);//servo1 attach to pin2
  servo2.attach(SERVO_MEASURE_PIN);//servo2 attach to pin3

//  Serial.begin(9600);

  //BT 시리얼 셋팅 및 시작
  BTSerial.begin(9600);

}

void CAR_move(unsigned char direction, unsigned char speed_left, unsigned char speed_right)
{
  switch (direction)
  {
    //car move forward with speed 180
    case 0: digitalWrite(MOTOR_L_1, HIGH); digitalWrite(MOTOR_L_2, LOW); //left motor clockwise rotation
      digitalWrite(MOTOR_R_1, HIGH); digitalWrite(MOTOR_R_2, LOW); break; //right motor clockwise rotation
    //car move back with speed 180
    case 1: digitalWrite(MOTOR_L_1, LOW); digitalWrite(MOTOR_L_2, HIGH); //left motor counterclockwise rotation
      digitalWrite(MOTOR_R_1, LOW); digitalWrite(MOTOR_R_2, HIGH); break; //right motor counterclockwise rotation
    default: break;
  }
  analogWrite(ENA_PIN, speed_left); //write speed_left to ENA_PIN,if speed_left is high,allow left motor rotate
  analogWrite(ENB_PIN, speed_right); //write speed_right to ENB_PIN,if speed_right is high,allow right motor rotate
}

// 61 ,131
void loop()
{
  
  if (BTSerial.available() > 0)
  {
    command = BTSerial.read();

    if (command == 'G' || command == 'g') {
        CAR_move(FORWARD, 180, 180);
        delay(2000);
      }

    if (command == 'S' || command == 's') {
        CAR_move(FORWARD, 0, 0);
        delay(2000);
      }      
  }
  else
  {
    reset();
  }
irValueL = digitalRead(SENSOR_IR_L);

  if(digitalRead(SENSOR_IR_L)==0 && digitalRead(SENSOR_IR_R)==0) {
    CAR_move(FORWARD, 0, 0);
    stop_count=stop_count+1;
    if(stop_count >30000)
    {
      CAR_move(BACK,120,120);
      delay(1000);
    }
  }

  else{
    drive(command);
    stop_count=0;
  }


   
}



void reset()
{
  CAR_move(FORWARD, 0, 0);
}

void forward()
{
//   CAR_move(FORWARD, 0, 0);
//   delay(50);
   CAR_move(FORWARD, 160, 160);
   delay(50);
}

void reverse()
{
//   CAR_move(BACK, 0,0);
//   delay(55);
   CAR_move(BACK,160,160);
   delay(45);
}

void left()
{
  CAR_move(FORWARD, 0, 0);
  if(control > 61){
    control=control-5;
  }

  servo1.write(control);
  delay(on_time);
}

void right()
{
  CAR_move(FORWARD, 0, 0);
  if(control < 131){
     control=control+5;
  }
  servo1.write(control);
  delay(on_time);
}

void forward_left()
{
  servo1.write(71);
  CAR_move(FORWARD,160,160);
  delay(50);
}

void forward_right()
{
  servo1.write(111);
  CAR_move(FORWARD,160,160);
  delay(50);
}

void reverse_right()
{
  servo1.write(111);
  CAR_move(BACK,160,160);
  delay(50);
}

void reverse_left()
{
  servo1.write(71);
  CAR_move(BACK,160,160);
  delay(50);  
}


void drive(int command)
{
  switch(command)
  {
    case 48: reset(); break;
    case 49: forward(); break;
    case 50: reverse(); break;
    case 51: right(); break;
    case 52: left(); break;
    case 53: forward_right(); break;
    case 54: forward_left(); break;
    case 55: reverse_right(); break;
    case 56: reverse_left(); break;
    default: break;
  }
}

 /* while (1) {
    char testt;
    // put your main code here, to run repeatedly:
    //데이터가 있으면 모니터프로그램으로 다시 전송
  
    if (Serial.available()) {
      testt = (char)Serial.read();
//      Serial.write(testt);

      if (testt == 'W' || testt == 'w') {
        CAR_move(FORWARD, speedl, speedr);
        delay(2000);
      }

      if (testt == 'E' || testt == 'e') {
        CAR_move(BACK, speedl, speedr);
        delay(1000);
        servo1.write(goRight);
        CAR_move(FORWARD, speedl, speedr);
        delay(1000);
        servo1.write(goLeft);
        CAR_move(FORWARD, speedl, speedr);
        delay(1000);
        servo1.write(goMiddle);
        CAR_move(FORWARD, speedl, speedr);
        delay(1000);
      }

      if (testt == 'Q' || testt == 'q') {
        CAR_move(BACK, speedl, speedr);
        delay(1000);
        servo1.write(goLeft);
        CAR_move(FORWARD, speedl, speedr);
        delay(1000); 
        servo1.write(goRight);
        CAR_move(FORWARD, speedl, speedr);
        delay(1000);
        servo1.write(goMiddle);
        CAR_move(FORWARD, speedl, speedr);
        delay(1000);
      }

      if (testt == 'A' || testt == 'a') {
        testBlueServo=testBlueServo-5; 
        delay(2000);
      }
      
      if (testt == 'D' || testt == 'd') {
        testBlueServo=testBlueServo+5;
        delay(2000);
      }

        if (testt == 'S' || testt == 's') {
        CAR_move(FORWARD,0, 0);
        delay(2000);
      }
      

  //    servo1.write(testBlueServo);
                  
    }

    //모니터 프로그램에서 입력 받은 것을 BT로 전송
    if (Serial.available()) {
      Serial.write(Serial.read());
    }




  }  */
  
  /*while(1)
    {
       Serial.print(digitalRead(SENSOR_IR_M));
       Serial.print(",");
       Serial.print(digitalRead(SENSOR_IR_L));
       Serial.print(",");
       Serial.println(digitalRead(SENSOR_IR_R));
    }*/
 /* while (1)
  {
    irValueL = digitalRead(SENSOR_IR_L);//read the value of left obstacle module
    irValueR = digitalRead(SENSOR_IR_R);//read the value of right obstacle module
    if (irValueL == 0 || irValueR == 0) //if obstacle module detect obstacles,it will out low level
    {
      flag = 1;
    }
    if (flag == 1) //when left obstacle module and right obstacle module detect obstacles
    {
      flag = 0;
      while (1)
      {
        servo1.write(ANGLE_SERVO1_MIDDLE);//servo1 rotate angle is 90
        servo2.write(ANGLE_SERVO2_MIDDLE);//servo2 rotate angle is 90
        delay(50);//delay 100ms
        CAR_move(BACK, speedl, speedr); //car move back with speed 180
        irValue = digitalRead(SENSOR_IR_M);//read the value of radar module
        if (irValue == 1) //if radar module detect obstacles,it will output low level;else,output high level
          break;
      }
    }
    CAR_move(FORWARD, speedl, speedr); //car move forward with speed 180
    for (int i = 60; i < 121; i++)
    {
      servo1.write(ANGLE_SERVO1_MIDDLE);//servo1 rotate angle is 90
      servo2.write(i);
      for (int s = 0; s < 500; s++)
      {
        irValue = digitalRead(SENSOR_IR_M);//read the value of radar module
        if (irValue == 0) //if radar module detect obstacles
        {
          goto turn;
        }
        asm("nop");
      }
    }
    for (int i = 120; i > 60; i--)
    {
      servo1.write(ANGLE_SERVO1_MIDDLE);//servo1 rotate angle is 90
      servo2.write(i);
      for (int s = 0; s < 500; s++)
      {
        irValue = digitalRead(SENSOR_IR_M);//read the value of radar module
        if (irValue == 0) //if radar module detect obstacles
        {
          goto turn;
        }
        asm("nop");
      }
    }
  }   */
/*
turn:
  CAR_move(FORWARD, 0, 0); //car move forward with speed 0
  delay(300);//delay 300
  DIS_Scan();//
  servo1.write(dir1);
  servo2.write(dir2);
  delay(100);
  CAR_move(FORWARD, 180, 180); //car move forward with speed 250
  while (1)
  {
    Serial.println("turn");
    irValueL = digitalRead(SENSOR_IR_L);//read the value of left obstacle module
    irValueR = digitalRead(SENSOR_IR_R);//read the value of right obstacle module
    if (irValueL == 0 || irValueR == 0) ////if obstacle module detect obstacles,it will out low level
    {
      flag = 1;
      break;
    }

    if (dir2 == 0) //if servo2 rotate angle is 0
    {
      irValueL = digitalRead(SENSOR_IR_L);//read the value of left obstacle module
      irValueR = digitalRead(SENSOR_IR_R);//read the value of right obstacle module
      if (irValueL == 0 || irValueR == 0) //if obstacle module detect obstacles,it will out low level
      {
        flag = 1;
        break;
      }
      for (int i = 0; i < 91; i++)
      {
        servo2.write(i);
        for (int j = 0; j < 500; j++)
        {
          delayMicroseconds(1);
          irValueL = digitalRead(SENSOR_IR_L);
          irValueR = digitalRead(SENSOR_IR_R);
          if (irValueL == 0 || irValueR == 0)
          {
            flag = 1;
            break;
          }
        }
        array_dis_r[i] = digitalRead(SENSOR_IR_M);//read the value of radar module
        irValueL = digitalRead(SENSOR_IR_L);
        irValueR = digitalRead(SENSOR_IR_R);
        if (irValueL == 0 || irValueR == 0)
        {
          flag = 1;
          break;
        }
      }
      count_1_r = 0;
      for (int i = 0; i < 91; i++)
      {
        if (array_dis_r[i])
          count_1_r++;
      }
      irValueL = digitalRead(SENSOR_IR_L);
      irValueR = digitalRead(SENSOR_IR_R);
      if (irValueL == 0 || irValueR == 0)
      {
        flag = 1;
        break;
      }
      Serial.print("count_1_r= ");
      Serial.println(count_1_r);
      if (count_1_r >= 89)
        break;
    }

    if (dir2 == 180)
    {
      irValueL = digitalRead(SENSOR_IR_L);
      irValueR = digitalRead(SENSOR_IR_R);
      if (irValueL == 0 || irValueR == 0)
      {
        flag = 1;
        break;
      }

      for (int i = 180; i > 90; i--)
      {
        servo2.write(i);
        for (int j = 0; j < 500; j++)
        {
          delayMicroseconds(1);
          irValueL = digitalRead(SENSOR_IR_L);
          irValueR = digitalRead(SENSOR_IR_R);
          if (irValueL == 0 || irValueR == 0)
          {
            flag = 1;
            break;
          }
        }
        array_dis_l[i - 90] = digitalRead(SENSOR_IR_M);
        irValueL = digitalRead(SENSOR_IR_L);
        irValueR = digitalRead(SENSOR_IR_R);
        if (irValueL == 0 || irValueR == 0)
        {
          flag = 1;
          break;
        }
      }
      count_1_l = 0;
      for (int i = 0; i < 90; i++)
      {
        if (array_dis_l[i])
          count_1_l++;
      }
      irValueL = digitalRead(SENSOR_IR_L);
      irValueR = digitalRead(SENSOR_IR_R);
      if (irValueL == 0 || irValueR == 0)
      {
        flag = 1;
        break;
      }
      Serial.print("count_1_l= ");
      Serial.println(count_1_l);
      if (count_1_l >= 89)
        break;
    }
  }
  CAR_move(FORWARD, 0, 0);
}
*/

/*
void DIS_Scan(void)
{
  unsigned char count_max = 0;
  unsigned char count_0_max = 0;
  servo1.write(ANGLE_SERVO1_MIDDLE);
  servo2.write(0);
  delay(100);
  for (int i = 0; i < 180; i++) //servo2 rotate from 0 to 180
  {
    servo1.write(ANGLE_SERVO1_MIDDLE);
    servo2.write(i);
    delay(10);
    array_distance[i] = digitalRead(SENSOR_IR_M);//read the value of radar module
  }
  count_l = 0; count_0_l = 0;
  for (int i = 91; i < 180; i++)
  {
    if (array_distance[i] == 1) //if radar module havenot detect obstacles between the angle of  90-180
      count_l++;
    else
      count_0_l++;
  }
  count_r = 0; count_0_r = 0;
  for (int i = 0; i < 90; i++)
  {
    if (array_distance[i] == 1) //if radar module havenot detect obstacles between the angle of  0-90
      count_r++;
    else
      count_0_r++;
  }
  //compare the max value of count_l and count_r
  if (count_l >= count_max)
    count_max = count_l;
  if (count_r >= count_max)
    count_max = count_r;
  //compare the max value of count_0_l and count_0_r
  if (count_0_l >= count_0_max)
    count_0_max = count_0_l;
  if (count_0_r >= count_0_max)
    count_0_max = count_0_r;

  if (count_max == count_r) //if between the angle of 90-180,having more angles that radar module havenot detect obstacles
    dir1 = ANGLE_SERVO1_RIGHT_MAX;//servo1 roate to 120
  if (count_max == count_l) //if between the angle of 0-90,having more angles that radar module havenot detect obstacles
    dir1 = ANGLE_SERVO1_LEFT_MAX;//servo1 rotate to 50

  if (count_0_max == count_0_r) //if between the angle of 90-180,having more angles that radar module already detect obstacles
  {
    dir2 = 0;//servo2 rotate to 0
  }
  if (count_0_max == count_0_l) //if between the angle of 0-90,having more angles that radar module already detect obstacles
  {
    dir2 = 180;//servo2 rotate to 180
  }
}  */

