#include "Arduino.h"
#include <digitalWriteFast.h>

#define Encoder2A 2
#define Encoder2B 3
#define p11 11
#define pi  3.141516

float at = 2 * pi;
float wt = 3;
float x0 = 0;
float kt = 14.7 / 100;
float rm = 3.2;
float kemf = kt;
float lm = 0.0048;
float jeff = 0.000048;
float lambda = 25;
float alpha = 0.5;

volatile bool Encoder2ASet;
volatile bool Encoder2BSet;
volatile bool Encoder2APrev;
volatile bool Encoder2BPrev;
float position2 = 0;
float teta0;
float angle2 = 0;
float angle1 = 0;
float angle3 = 0;
float pos21 = 0;
float I2;
float I2n;
float pos1 = 0;
float pos2 = 0;
float fricn1 = 0;

float x1n = 0;
float x1n1 = 0;
float x2n;
float x2n1;
float x2f;
float x2f1;
float x3n;
float x3n1;
float x1d;
float x1d1;
float x2d;
float x2d1;
float fn = 0;
float fn1 = 0;
float u = 0;

float t1 = 0.001;
float tc = 0.001;
float T = 0.001;
float t0 = micros() ;
float t;

float deltap;
float w0 ;
float sn;
float g1;
float g2;
float p1 = -rm / lm;;
float p2 = -kemf / lm;
float p3 = 1 / lm;
float p4 = kt / jeff;
float b;
float bi;
float qlow;
float qmid;
float qhigh;
float id;
float fn11;
float fn12;
float fn13;


float c1 = -0.1383;
float c2 = 0.3006;
float c3 = 1.149;
float c4 = 1.782;
int c5 = 1;

float I01;
float I02;
float I03;
float I04;
float I05;
float I06;
float I07;
float I08;
float I09;
float I10;
float I0;


float cpa2 = 0.7;
float vpa2 = 100.4;
float cpa1 = 0.22;
float vpa1 = 0;

float cpn2 = 0.847;
float vpn2 = 95.5;
float cpn1 = 0.38;
float vpn1 = 0;

float cpr2 = 1.051;
float vpr2 = 92.2;
float cpr1 = 0.54;
float vpr1 = 0;

float velpa1 = 0;
float volpa1 = 0.44;
float velpa2 = 100.5;
float volpa2 = 12;

float velpn1 = 0;
float volpn1 = 0.86;
float velpn2 = 97.74;
float volpn2 = 12.1;
float vn;
float velpr1 = 0;
float volpr1 = 1.26;
float velpr2 = 93.49;
float volpr2 = 12;

float curpn1 = 0.26;
float vopn1 = 0.67;

float curpn2 = 0.43;
float vopn2 = 2.07;

float curpn3 = 0.84;
float vopn3 = 12.1;

float curpa1 = 0.23;
float vopa1 = 0.89;

float curpa2 = 0.3;
float vopa2 = 2.29;

float curpa3 = 0.7;
float vopa3 = 12;

float curpr1 = 0.50;
float vopr1 = 1.12;

float curpr2 = 0.7;
float vopr2 = 2.72;

float curpr3 = 1.06;
float vopr3 = 12;

float vela;
float veln;
float velr;
float distfact;
float dz;
float curvolr;
float curvola;
float curvoln;
float id0;
float I3;
float I4;
float vola;
float voln;
float volr;
float fricn;
float fricnu;


float distfactfu;
float distfactf;
float distfactf1;
float distfactn;
float distfactn1;

int tp;
int cd;
int pwm1;

void setup ()
{
  // Serial.begin(9600);
  pinMode(Encoder2A, INPUT);      // sets pin A as input
  digitalWrite(Encoder2A, LOW);
  pinMode(Encoder2B, INPUT);      // sets pin B as input
  digitalWrite(Encoder2B, LOW);

  attachInterrupt(0, HandleMotorInterrupt2A, CHANGE);
  attachInterrupt(1, HandleMotorInterrupt2B, CHANGE);

  digitalWrite(12, LOW);
  analogWrite(11, LOW);
  delay (500);

  I01 = analogRead(A0);
  I02 = analogRead(A0);
  I03 = analogRead(A0);
  I04 = analogRead(A0);
  I05 = analogRead(A0);
  I06 = analogRead(A0);
  I07 = analogRead(A0);
  I08 = analogRead(A0);
  I09 = analogRead(A0);
  I10 = analogRead(A0);

  I0 = 0.1 * (I01 + I02 + I03 + I04 + I05 + I06 + I07 + I08 + I09 + I10);

}

void loop() {
  tc = micros();

  t = (tc - t0) / 1000000.0;

  if (c5 == 1)
  {
    position2 = 0;
    c5 = 0;
  }
  x1d = 6 * pi + at * sin(wt * t)+ 0.5*at*cos(0.5*wt*t);
  x2d = at * wt * cos(wt * t)- 0.25*at*wt*sin(0.5*wt*t);

  x1d1 = 6 * pi + at * sin(wt * (t + T)) + 0.5*at*cos(0.5*wt*(t + T));
  x2d1 = at * wt * cos(wt * (t + T)) - 0.25*at*wt*sin(0.5*wt*(t + T));

  angle2 = position2 * 2 * pi / 1000;

  pos1 = pos2;
  pos2 = angle2;
  deltap = pos2 - pos1;

  x2n1 = x2n;
  x2n = deltap / T;

  x1n1 = x1n;
  x1n = x1n1 + (3 * T / 2) * x2n - (T / 2) * x2n1;

  I2 = analogRead(A0);
  I2n = (I2 - I0 ) * 0.074;

  I3 = analogRead(A3);
  I4 = analogRead(A4);
  vn = (I3 - I4) * 0.015;


  if ((fn > -vopn1) && (fn < vopn1))
  {
    curvoln = (curpn1 / vopn1) * fn;
  }    else if ((fn >= vopn1) && (fn < vopn2)) {
    curvoln = ((curpn2 - curpn1) / (vopn2 - vopn1)) * fn  - ( ((curpn2 - curpn1) / (vopn2 - vopn1)) * vopn2 - curpn2 );
  } else if (fn >= vopn2) {
    curvoln = ((curpn3 - curpn2) / (vopn3 - vopn2)) * fn  - ( ((curpn3 - curpn2) / (vopn3 - vopn2)) * vopn3 - curpn3 );
  } else if ((fn > -vopn2) && (fn < -vopn1)) {
    curvoln = ((curpn2 - curpn1) / (vopn2 - vopn1)) * fn  + ( ((curpn2 - curpn1) / (vopn2 - vopn1)) * vopn2 - curpn2 );
  } else if (fn < -vopn2) {
    curvoln = ((curpn3 - curpn2) / (vopn3 - vopn2)) * fn  + ( ((curpn3 - curpn2) / (vopn3 - vopn2)) * vopn3 - curpn3 );
  }

  if ((fn >= 0) && (fn < vopa1))
  {
    curvola = (curpa1 / vopa1) * fn;
  } else if ((fn >= vopa1) && (fn < vopa2)) {
    curvola = ((curpa2 - curpa1) / (vopa2 - vopa1)) * fn  - ( ((curpa2 - curpa1) / (vopa2 - vopa1)) * vopa2 - curpa2 );
  } else if (fn >= vopa2) {
    curvola = ((curpa3 - curpa2) / (vopa3 - vopa2)) * fn  - ( ((curpa3 - curpa2) / (vopa3 - vopa2)) * vopa3 - curpa3 );
  } else if ((fn < 0) && (fn > -vopr1)) {
    curvola = (curpr1 / vopr1) * fn;
  } else if ((fn > -vopr2) && (fn < -vopr1)) {
    curvola = ((curpr2 - curpr1) / (vopr2 - vopr1)) * fn  + ( ((curpr2 - curpr1) / (vopr2 - vopr1)) * vopr2 - curpr2 );
  } else if (fn < -vopr2) {
    curvola = ((curpr3 - curpr2) / (vopr3 - vopr2)) * fn  + ( ((curpr3 - curpr2) / (vopr3 - vopr2)) * vopr3 - curpr3 );
  }

  if ((fn >= 0) && (fn < vopr1))
  {
    curvolr = (curpr1 / vopr1) * fn;
  } else if ((fn >= vopr1) && (fn < vopr2)) {
    curvolr = ((curpr2 - curpr1) / (vopr2 - vopr1)) * fn  - ( ((curpr2 - curpr1) / (vopr2 - vopr1)) * vopr2 - curpr2 );
  } else if (fn >= vopr2) {
    curvolr = ((curpr3 - curpr2) / (vopr3 - vopr2)) * fn  - ( ((curpr3 - curpr2) / (vopr3 - vopr2)) * vopr3 - curpr3 );
  } else if ((fn < 0) && (fn > -vopa1)) {
    curvolr = (curpa1 / vopa1) * fn;
  } else if ((fn > -vopa2) && (fn < -vopa1)) {
    curvolr = ((curpa2 - curpa1) / (vopa2 - vopa1)) * fn  + ( ((curpa2 - curpa1) / (vopa2 - vopa1)) * vopa2 - curpa2 );
  } else if (fn < -vopr2) {
    curvolr = ((curpa3 - curpa2) / (vopa3 - vopa2)) * fn  + ( ((curpa3 - curpa2) / (vopa3 - vopa2)) * vopa3 - curpa3 );
  }

  // DISTURBANCE & DEADZONE ESTIMATION


  if ((fn > volpr1) or (fn < -volpr1)) // CASE 1
  {

    if (fn >= volpa1)
    {
      vela = ((velpa2 - velpa1) / (volpa2 - volpa1)) * fn  -  (((velpa2 - velpa1) / (volpa2 - volpa1)) * volpa1);
    }    else if (fn <= -volpr1) {
      vela = ((velpr2 - velpr1) / (volpr2 - volpr1)) * fn  +  (((velpr2 - velpr1) / (volpr2 - volpr1)) * volpr1);
    }  else if ((fn > -volpr1) && (fn < volpa1)) {
      vela = 0;
    }

    if (fn >= volpn1)
    {
      veln = ((velpn2 - velpn1) / (volpn2 - volpn1)) * fn  -  (((velpn2 - velpn1) / (volpn2 - volpn1)) * volpn1);
    }    else if (fn <= -volpn1) {
      veln = ((velpn2 - velpn1) / (volpn2 - volpn1)) * fn  +  (((velpn2 - velpn1) / (volpn2 - volpn1)) * volpn1);
    }  else if ((fn > -volpn1) && (fn < volpn1)) {
      veln = 0;
    }

    if (fn >= volpr1)
    {
      velr = ((velpr2 - velpr1) / (volpr2 - volpr1)) * fn  -  (((velpr2 - velpr1) / (volpr2 - volpr1)) * volpr1);
    }    else if (fn <= -volpa1) {
      velr = ((velpa2 - velpa1) / (volpa2 - volpa1)) * fn  +  (((velpa2 - velpa1) / (volpa2 - volpa1)) * volpa1);
    }  else if ((fn > -volpa1) && (fn < volpr1)) {
      velr = 0;
    }

    if ((x2n >= 0) && (x2n <= veln)) // DZ1
    {
      distfact = (x2n - veln) / (veln - velr);  // NEGATIVE = RESISTIVE
      dz = cpn1 + abs(distfact) * (cpr1 - cpn1);
    } else if ((x2n >= 0) && (x2n > veln) ) {
      distfact = (x2n - veln) / (vela - veln); // POSITIVE = ASSISTIVE
      dz = cpn1 - abs(distfact) * (cpn1 - cpa1);
    } else if ((x2n < 0) && (x2n >= -veln)) {
      distfact = (x2n - veln) / (vela - veln) ; // POSITIVE = ASSISTIVE
      dz = cpn1 - abs(distfact) * (cpn1 - cpa1);
    } else if ((x2n < 0) && (x2n < -veln) ) {
      distfact = (x2n - veln) / (veln - velr) ; // NEGATIVE = RESISTIVE
      dz = cpn1 + abs(distfact) * (cpr1 - cpn1);
    }
  } else if ((fn < volpr1) && (fn > -volpr1)) { // CASE 2


    if ((x3n > 0) && (x3n < curvoln)) // DZ2
    {
      distfact = - (x3n - curvoln) / (curvoln - curvola);  // POSITIVE = ASSISTIVE
      dz = cpn1 - abs(distfact) * (cpn1 - cpa1);
    } else if ((x3n > 0) && (x3n > curvoln)) {
      distfact = - (x3n - curvoln)  / (curvolr - curvoln); // NEGATIVE = RESISTIVE
      dz = cpn1 + abs(distfact) * (cpr1 - cpn1);
    } else if ((x3n < 0) &&  (x3n > - curvoln)) {
      distfact = - (x3n - curvoln) / (curvolr - curvoln) ; // NEGATIVE = RESISTIVE
      dz = cpn1 + abs(distfact) * (cpr1 - cpn1);
    } else if ((x3n < 0) && (x3n < - curvola) ) {
      distfact = - (x3n - curvoln) / (curvoln - curvola);  // POSITIVE = ASSISTIVE
      dz = cpn1 - abs(distfact) * (cpn1 - cpa1);
    }
  }

  distfactn1 = distfactn;
  distfactn = distfact;

  distfactfu = distfactf + (3 * T / 2) * w0 * (distfactn - distfact) + (T / 2) * w0 * (distfactn1 - distfactf1);
  distfactf1 = distfactf;
  distfactf = distfactfu;

  if (distfactf > 0) {
    dz = cpn1 - distfactf * (cpn1 - cpa1);
  } else {
    dz = cpn1 + abs(distfactf) * (cpr1 - cpn1);
  }

  dz = 0;

  if (abs(x3n) <= dz)
  {
    fricnu = kt * x3n;
  } else if ( (abs(x3n) > dz)   &&  (distfact > 0) ) {
    fricnu = kt * (curvoln - distfactf * (curvoln - curvola) );
  } else {
    fricnu = kt * (curvoln + distfactf * (curvoln - curvolr) );
  }

  if ((abs(fn))>1)
  {
    fricnu = 0;
  }
  

  fricn1 = fricn;
  fricn = fricnu;

  x3n1 = x3n;
  x3n = I2n;

  sn = x2n - x2d + lambda * (x1n - x1d) ;
  g1 =  x1n + (3 * T / 2) * x2n - (T / 2) * x2n1;
  g2 =  x2n + (3 * T / 2) * (- fricn / jeff) - (T / 2) * (p4 * x3n1 - fricn1 / jeff);

  b = (3 * T / 2) * p4;

  qhigh = 0 - lambda * g1 - g2 + x2d1 + lambda * x1d1;
  qlow = - 0 - lambda * g1 - g2 + x2d1 + lambda * x1d1;

  qmid = (1 - alpha) * qlow + alpha * qhigh;

  id = (1 / b) * qlow;



  bi = (3 * T / 2) * p3;

  fn11 = id - x3n;
  fn12 = - (3 * T / 2) * (p1 * x3n + p2 * x2n) ;
  fn13 = (T / 2) * (p1 * x3n1 + p2 * x2n1 + p3 * fn);
  fn1 = fn;
  fn = (1 / bi) * (fn11 + fn12 + fn13);

  u = 21.74 * fn;
  if ( u >= 0 )
  {
    digitalWrite(12, LOW);
    analogWrite(11, u);
  } else {
    digitalWrite(11, LOW);
    analogWrite(12, -u);
  }


  tp = floor(t*3);
  cd = tp % 3;

  if (cd == 1)
  {
    pwm1 = 40*sin(pi*t);
  } else if (cd ==2) {
    pwm1 = 0;
  } else {
    pwm1 = -50*cos(1.5*pi*t);
  }
  
  if (pwm1 > 0) {
    digitalWrite(10, LOW);
    analogWrite(9, pwm1);

    digitalWrite(6, LOW);
    analogWrite(5, pwm1);
  } else {
    digitalWrite(9, LOW);
    analogWrite(10, -pwm1);

    digitalWrite(5, LOW);
    analogWrite(6, -pwm1);
  }

  //delayMicroseconds(20000);

  t1 = micros();

  T = (t1 - tc) / (1000000.00);

}

void HandleMotorInterrupt2A() {
  Encoder2ASet = digitalReadFast(Encoder2A);
  deltap = ParseEncoder2();
  position2 += deltap;

  Encoder2APrev = Encoder2ASet;
}

void HandleMotorInterrupt2B() {
  // Test transition;
  Encoder2BSet = digitalReadFast(Encoder2B);
  deltap = ParseEncoder2();
  position2 += deltap;

  Encoder2BPrev = Encoder2BSet;
}

int ParseEncoder2() {
  if (Encoder2APrev && Encoder2BPrev) {
    if (!Encoder2ASet && Encoder2BSet) return 1;
    if (Encoder2ASet && !Encoder2BSet) return -1;
  } else if (!Encoder2APrev && Encoder2BPrev) {
    if (!Encoder2ASet && !Encoder2BSet) return 1;
    if (Encoder2ASet && Encoder2BSet) return -1;
  } else if (!Encoder2APrev && !Encoder2BPrev) {
    if (Encoder2ASet && !Encoder2BSet) return 1;
    if (!Encoder2ASet && Encoder2BSet) return -1;
  } else if (Encoder2APrev && !Encoder2BPrev) {
    if (Encoder2ASet && Encoder2BSet) return 1;
    if (!Encoder2ASet && !Encoder2BSet) return -1;
  }
}
