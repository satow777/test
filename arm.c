/*アーム関節の番号は「armpoint」に参考してください*/
#include<Servo.h>
//オブジェクト作成
Servo dodai1;
Servo kansetu1;
Servo kansetu2;
Servo kansetu3;
Servo tume1;

char cflag = 0;
char i = 0;
char cSW = 0;
const int sleep = 0;//各関節アクセス時間(ms)
const int angle = 5; // サーボ標準角度
float OFFSET = 0.8; //オフセット

//ジョイスティック1入力ピン
const int X_PIN_1 = A15;//A15（アナログ)
const int Y_PIN_1 = A14;//A14（アナログ)
const int SW_PIN_1 = 45;//45（デジタル)

//ジョイスティック2入力ピン
const int X_PIN_2 = A13;//A13（アナログ）
const int Y_PIN_2 = A12;//A12（アナログ）
const int SW_PIN_2 = 44;//44（デジタル）

//トグルスイッチ入力ピン
const int tSW_PIN_1 = A1;//A（アナログ）
const int tSW_PIN_2 = A2;//A（アナログ）

//出力
const int idodai1 = 3;
const int ikansetu1 = 5;
const int ikansetu2 = 6;
const int ikansetu3 = 10;
const int itume1 = 13;
const int SW_LED = 34;// スイッチ判定LEDをデジタルピン

int aD1 = 90;
int aK1 = 90;//関節１
int aK2 = 90;//関節2
int aK3 = 90;
int aT1 = 90;

int X_POS_1;            // X軸方向の読み取り値の変数を整数型に
int Y_POS_1;            // Y軸方向の読み取り値の変数を整数型に
int SW_POS_1;

int X_POS_2;            // X軸方向の読み取り値の変数を整数型に
int Y_POS_2;            // Y軸方向の読み取り値の変数を整数型に
int SW_POS_2;

//トグルスイッチ
int tSW_POS_1;
int tSW_POS_2;

void setup()
{
  Serial.begin(9600);//テスト用

  pinMode(X_PIN_1, INPUT);          // A0ピンを入力に(省略可)
  pinMode(Y_PIN_1, INPUT);          // A1ピンを入力に(省略可)
  pinMode(SW_PIN_1, INPUT_PULLUP);

  pinMode(X_PIN_2, INPUT);          // A0ピンを入力に(省略可)
  pinMode(Y_PIN_2, INPUT);          // A1ピンを入力に(省略可)
  pinMode(SW_PIN_2, INPUT_PULLUP);

  //トグルスイッチ
  pinMode(tSW_PIN_1, INPUT);          // A0ピンを入力に(省略可)
  pinMode(tSW_PIN_2, INPUT);          // A1ピンを入力に(省略可)

  pinMode(SW_LED, OUTPUT);

  dodai1.attach(idodai1);
  kansetu1.attach(ikansetu1);
  kansetu2.attach(ikansetu2);
  kansetu3.attach(ikansetu3);
  tume1.attach(itume1);
}

void loop()
{
  X_POS_1 = analogRead(X_PIN_1);
  Y_POS_1 = analogRead(Y_PIN_1);
  SW_POS_1 = digitalRead(SW_PIN_1);

  X_POS_2 = analogRead(X_PIN_2);
  Y_POS_2 = analogRead(Y_PIN_2);
  SW_POS_2 = digitalRead(SW_PIN_2);

  //トグルスイッチ
  tSW_POS_1 = analogRead(tSW_PIN_1);
  tSW_POS_2 = analogRead(tSW_PIN_2);

  if (i == 0)
  {
    dodai1.write(90);
    delay(1000);
    kansetu1.write(90);
    delay(1000);
    kansetu2.write(90);
    delay(1000);
    kansetu3.write(90);
    delay(1000);
    tume1.write(140);
    delay(1000);
    i = 1;
  }

  if (tSW_POS_1 == 1)
  {
    //LED点灯
    digitalWrite(SW_LED, HIGH);

    //ジョイステック1
    if (X_POS_1 > 512 - 512 * OFFSET && X_POS_1 < 512 + 512 * OFFSET && Y_POS_1 > 512 - 512 * OFFSET && Y_POS_1 < 512 + 512 * OFFSET)
    {
      //処理しない範囲
    }
    else
    {
      dodai1_control();
      kansetu1_control();
    }

    //ジョイステック2
    if (X_POS_2 > 512 - 512 * OFFSET && X_POS_2 < 512 + 512 * OFFSET && Y_POS_2 > 512 - 512 * OFFSET && Y_POS_2 < 512 + 512 * OFFSET)
    {
      //処理しない範囲
    }
    else
    {
      kansetu_2_3_control();
    }

    tume1_control();

  }
  else if (tSW_POS_2 == 1) //操作停止
  {
    //LED消灯
    digitalWrite(SW_LED, LOW);
  }

  //モニタ
  Serial.print("aD1: ");     // シリアルモニタにそれぞれの値を出力
  Serial.print(aD1);
  Serial.print("  aK1: ");
  Serial.print(aK1);
  Serial.print("  aK2: ");
  Serial.print(aK2);
  Serial.print("  X_POS_1: ");
  Serial.print(X_POS_2);
  Serial.print("  Y_POS_1: ");
  Serial.println(Y_POS_2);
}


//土台制御（ジョイスティック1X軸）
void dodai1_control(void)
{
  if (X_POS_1 < 512 - 512 * OFFSET) //0
  {
    dodai1.write(aD1);
    aD1 = aD1 + angle;
    if (aD1 > 180)
    {
      aD1 = 180;
    }
  }
  else  if (X_POS_1 > 512 + 512 * OFFSET) //1023
  {
    dodai1.write(aD1);
    aD1 = aD1 - angle;
    if (aD1 < 0)
    {
      aD1 = 0;
    }
  }
}

//関節1制御（ジョイスティック1Y軸）
int kansetu1_control(void)
{
  if (Y_POS_1 < 512 - 512 * OFFSET)//0
  {
    kansetu1.write(aK1);
    aK1 = aK1 - angle;
    if (aK1 < 0)
    {
      aK1 = 0;
    }
  }
  else  if (Y_POS_1 > 512 + 512 * OFFSET)//1023
  {
    kansetu1.write(aK1);
    aK1 = aK1 + angle;
    if (aK1 > 180)
    {
      aK1 = 180;
    }
  }
}


//関節2,3制御（ジョイスティック２Y軸）
void kansetu_2_3_control(void)
{
  switch (cflag)
  {
    case 0:
      if (Y_POS_2 < 512 - 512 * OFFSET)//0
      {
        kansetu2.write(aK2);
        aK2 = aK2 - angle;
        if (aK2 < 0) //サーボ1が回り切ったら次のモーターへ
        {
          cflag = 1;
          aK2 = 0;
        }
      }
      else  if (Y_POS_2 > 512 + 512 * OFFSET)//1023
      {
        kansetu2.write(aK2);
        aK2 = aK2 + angle;
        if (aK2 > 180)
        {
          aK2 = 180;
        }
      }
      break;

    case 1:
      if (Y_POS_2 < 512 - 512 * OFFSET)//0
      {
        kansetu3.write(aK3);
        aK3 = aK3 - angle;
        if (aK3 < 0)
        {
          aK3 = 0;
        }
      }
      else  if (Y_POS_2 > 512 + 512 * OFFSET)//1023
      {
        kansetu3.write(aK3);
        aK3 = aK3 + angle;
        if (aK3 > 90)
        {
          cflag = 0;
          aK3 = 90;
        }
      }
      break;
  }
}

//爪制御（ジョイスティック1押し込み:開く、ジョイスティック2押し込み:閉じる）
void tume1_control(void)
{
  if (SW_POS_1 == 0)
  {
    tume1.write(140);
  }
  else if (SW_POS_2 == 0)
  {
    tume1.write(90);
  }
}
