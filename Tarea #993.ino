#define kdb_es_es
#include "DigiKeyboard.h"

void setup() {
  // Configuración inicial, si es necesaria
}

void loop() {
  // Espera 2 segundos al inicio
  DigiKeyboard.delay(2000);
  
  // Abre Fake Windows Update en Microsoft Edge
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(600);
  DigiKeyboard.print("msedge");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(1000);
  DigiKeyboard.print("https");
  DigiKeyboard.sendKeyStroke(55, MOD_SHIFT_RIGHT);
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.print("fakeupdate.net");
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.print("win10ue");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.sendKeyStroke(KEY_F11);
  DigiKeyboard.delay(5000); 

  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(600);
  DigiKeyboard.print("msedge");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(1000);
  DigiKeyboard.print("https");
  DigiKeyboard.sendKeyStroke(55, MOD_SHIFT_RIGHT);
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.print("geekprank.com");
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.print("blank.php");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.sendKeyStroke(KEY_F11);
  DigiKeyboard.delay(5000);

  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(600);
  DigiKeyboard.print("firefox");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(1000);
  DigiKeyboard.print("https");
  DigiKeyboard.sendKeyStroke(55, MOD_SHIFT_RIGHT);
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.print("www.youtube.com");
  DigiKeyboard.sendKeyStroke(45, MOD_ALT_LEFT | MOD_CONTROL_LEFT);
  DigiKeyboard.print("watch");
  DigiKeyboard.sendKeyStroke(45,MOD_SHIFT_RIGHT);
  DigiKeyboard.sendKeyStroke(25);
  DigiKeyboard.sendKeyStroke(39,MOD_SHIFT_RIGHT);
  DigiKeyboard.print("4Lsbg4tq9");
  DigiKeyboard.sendKeyStroke(56);
  DigiKeyboard.sendKeyStroke(20,MOD_SHIFT_RIGHT);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(1000);
  DigiKeyboard.sendKeyStroke(14);
  DigiKeyboard.delay(1000);
  DigiKeyboard.sendKeyStroke(9);
  
  for (;;) {}
}