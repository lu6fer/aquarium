#include <UI.h>

UI::UI(uint8_t lcd, uint8_t keypad)
{
  lcd_addr = lcd;
  keypad_addr = keypad;

  /**
   *
   * Keypad config
   *
  **/
  const byte ROWS = 4; //four rows
  const byte COLS = 4; //four columns
  //define the cymbols on the buttons of the keypads
  char keys[ROWS][COLS] = {
  	{'1','2','3','A'},
  	{'4','5','6','B'},
  	{'7','8','9','C'},
  	{'*','0','#','D'}
  };
  byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
  byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad


  lcd = new LiquidCrystal_I2C(lcd_addr, 20, 4);
  kpd = new Keypad_I2C(makeKeymap(keys), rowPins, colPins, ROWS, COLS, keypad_addr);
}

void UI::begin()
{
  lcd->init();
  lcd->backlight();
  bl_status = true;
  bl_time = millis();
  kpd->begin();
}

/**
 *
 * Display menu
 *
**/
void UI::display(const Menu_t &menu)
{
  /* Variable pour le menu */
  byte selectedMenuItem = 0;   // Choix sélectionné
  byte shouldExitMenu = false; // Devient true quand l'utilisateur veut quitter le menu
  Button_t buttonPressed;      // Contient le bouton appuyé

  /* Tant que l'utilisateur ne veut pas quitter pas le menu */
  while(!shouldExitMenu) {

    /* Affiche le menu */
    lcd->clear();
    lcd->print(menu.prompt);
    lcd->setCursor(0, 1);
    lcd->print(menu.items[selectedMenuItem]);

    /* Attend le relâchement du bouton */
    while(readButton() != BP_NONE);

    /* Attend l'appui sur un bouton */
    while((buttonPressed = readButton()) == BP_NONE);

    /* Anti rebond pour le bouton */
    delay(30);

    /* Attend le relâchement du bouton */
    while(readButton() != BP_NONE);

    /* Gére l'appui sur le bouton */
    switch(buttonPressed) {
      case BP_UP: // Bouton haut = choix précédent
        /* S'il existe un choix précédent */
        if(selectedMenuItem > 0) {

          /* Passe au choix précédent */
          selectedMenuItem--;
        }
        break;
      case BP_DOWN: // Bouton bas = choix suivant
        /* S'il existe un choix suivant */
        if(selectedMenuItem < (menu.nbItems - 1)) {

          /* Passe au choix suivant */
          selectedMenuItem++;
        }
        break;
      case BP_CANCEL: // Bouton gauche = sorti du menu
        shouldExitMenu = true;
        break;
      case BP_SELECT: //
      case BP_RIGTH:  // Bouton droit ou SELECT = validation du choix
        menu.callbackFnct(selectedMenuItem);
        break;
    }
  }
}

/**
 *
 * Lcd Backlight timer
 *
**/
void UI::bl_timer()
{
  if (bl_status && (millis() - bl_time >= (bl_duration * 1000))) {
    lcd->noBacklight();
    bl_status = false;
  }
}

/**
 *
 * Read pressed buttons
 * Trigger backlight and return button type
 *
**/
Button_t UI::readButton()
{
  switch (kpd->getState()) {
    case PRESSED:
      lcd->backlight();
      bl_status = true;
      bl_time = millis();
      switch (kpd->getKey()) {
        case 7: return BP_CANCEL; break;
        case 8: return BP_UP; break;
        case 9: return BP_SELECT; break;
        case '*': return BP_LEFT; break;
        case 0: return BP_DOWN; break;
        case '#': return BP_RIGTH; break;
        default: return BP_NONE; break;
      }
    default:
      lcd->noBacklight();
      bl_status = false;
  }
  return BP_NONE;
}
