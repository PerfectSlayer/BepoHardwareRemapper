#include <Usb.h>
#include <hidboot.h>

#define DEBUG
#define modeLED 11

#define NONE 0
#define CTRL 1
#define SHIFT 2
#define ALT 4
#define WIN 8
#define RCTRL 16
#define RSHIFT 32
#define RALT 64
#define RWIN 128


uint8_t minusKeyMap[] = {
// Description:
// - Index: HID input code
// - Documentation: Related qwerty touch
// - Documentation: Related bepo touch
// - Value: Converted azerty input related qwerty input code to produce bepo input
// Exemple: A (bepo) is the A touch (qwerty layout) so index is 4 (from HID code)
//          The value is HID code of the touch A in azerty layout located in qwerty layout: Q
//          So value is 20 (Q from HID code)
// A touch (querty layout) 
    0,    1,    2,    3,   20,   14,   27,   12,   19,    8,
//                          A     B      C    D     E     F 
//                          A     K      X    I     P     E 
   16,    6,    7,   23,   22,   21,    4,   33,   15,   13,
//  G     H     I     J     K     L     M     N     O     P 
//  ,     C     D     T     S     R     Q     '     L     J 
    5,   18,   24,   36,   25,   54,   31,   28,   47,   39,
//  Q     R     S     T     U     V     W     X     Y     Z 
//  B     O     U     è     V     .     é     Y     ^     à 
   32,   32,   32,   34,   45,   39,   46,   35,   55,   50,
//  1     2     3     4     5     6     7     8     9     0 
//  "     "     "     (     )     @     +     -     /     * 
   40,   41,   42,   43,   44,   46,   52,   26,   29,   38,
//                                -     =     [     ]     \-
//                                =     %     Z     W     ç 
   50,   17,   51,   48,   10,   11,    9,   57,   58,   59,
//  <     ;    '      #     M     ,     .                   
//        N    M      $     G     H     F                   
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
   38,  101,  102,  103,  104,  105,  106,  107,  108,  109,
//  <                                                       
//  ç                                                       
};

uint8_t minusKeyModifierMap[] = {
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE,SHIFT, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, RALT,SHIFT, NONE,SHIFT, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE,SHIFT, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
 NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
};

uint8_t capsKeyMap[] = {
    0,    1,    2,    3,   20,   14,   27,   12,   19,    8,
//                          A     B      C    D     E     F 
//                          A     K      X    I     P     E 
   54,    6,    7,   23,   22,   21,    4,   16,   15,   13,
//  G     H     I     J     K     L     M     N     O     P 
//  ;     C     D     T     S     R     Q     ?     L     J 
    5,   18,   24,   36,   25,   55,   31,   28,   56,   39,
//  Q     R     S     T     U     V     W     X     Y     Z 
//  B     O     U     è     V     :     é     Y     !     à 
   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
//  1     2     3     4     5     6     7     8     9     0 
//  1     2     3     4     5     6     7     8     9     0 
   40,   41,   42,   43,   44,   45,   36,   26,   29,   38,
//                                -     =     [     ]     \-
//                                °     %     Z     W     ç 
   50,   17,   51,   32,   10,   11,    9,   57,   58,   59,
//  <     ;    '      #     M     ,     .                   
//        N    M      #     G     H     F                   
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
   38,  101,  102,  103,  104,  105,  106,  107,  108,  109,
//  <                                                       
//  ç                                                       
};

uint8_t capsKeyModifierMap[] = {
SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
 NONE,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
SHIFT,SHIFT,SHIFT, NONE,SHIFT, NONE, NONE,SHIFT, NONE, NONE,
SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT, RALT,SHIFT,SHIFT, NONE,
SHIFT,SHIFT,SHIFT, RALT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
 NONE,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,SHIFT,
};

uint8_t altgrKeyMap[] = {
    0,    1,    2,    3,   20,   31,   46,   47,   30,    8,
//                          A     B     C     D     E     F 
//                                ~     }     ¨     &     € 
   16,    6,    7,   23,   22,   21,    4,   33,   15,   13,
//  G     H     I     J     K     L     M     N     O     P 
//                                                          
   35,   18,   52,   36,   25,   54,   31,   33,   47,   37,
//  Q     R     S     T     U     V     W     X     Y     Z 
//  |           ù                             {           \-
   32,  100,  100,   34,   45,   39,   46,   35,   55,   50,
//  1     2     3     4     5     6     7     8     9     0 
//        <     >     [     ]                               
   40,   41,   42,   42,   37,   46,   52,   26,   29,   38,
//                        spc     -     =     [     ]     \-
//                          _                               
   50,   17,   51,   48,   37,   11,    9,   57,   58,   59,
//  <     ;    '      #     M     ,     .                   
//                                                          
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
   38,  101,  102,  103,  104,  105,  106,  107,  108,  109,
//  <                                                       
//                                                          
};

uint8_t altgrKeyModifierMap[] = {
 RALT, RALT, RALT, RALT, RALT, RALT, RALT,SHIFT, NONE, RALT,
 RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, NONE, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, NONE,SHIFT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, RALT, RALT, NONE, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
 RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT, RALT,
};
/**
 * Store pressed key and its modifier according input key.
 */
class PressedKey {
public:
  /** The input key. */
  uint8_t inputKey;
  /** The pressed key. */
  uint8_t key;
  /** The pressed key modifiers. */
  uint8_t modifier;
public:
  /**
   * Default constructor.
   */
  PressedKey();
  /**
   * Key pressed constructor.
   *
   * @param pInputKey The input key.
   * @param pKey      The pressed key.
   * @param pModifier The pressed key modifiers.
   */
  PressedKey(uint8_t pInputKey, uint8_t pKey, uint8_t pModifier);
};

PressedKey::PressedKey() : inputKey(0), key(0), modifier(NONE) { };
PressedKey::PressedKey(uint8_t pInputKey, uint8_t pKey, uint8_t pModifier) : inputKey(pInputKey), key(pKey), modifier(pModifier) { };

/**
 * Keyboard report parser to remap keys as bépo layout.
 */
class KeyboardBepoRemapper : public KeyboardReportParser {
protected:
  /** Remapper enable status. */
  boolean enabled;
  /** Key buffer to send. */
  uint8_t keyBuffer[8];
  /** Pressed keys. */
  PressedKey pressedKeys[6];
public:
  /**
   * Default constructor.
   */
  KeyboardBepoRemapper();
protected:
  /**
   * Key down callback.
   *
   * @param mod The key down modifiers.
   * @param key The key down.
   */
  virtual void OnKeyDown(uint8_t mod, uint8_t key);
  /**
   * Key up callback.
   *
   * @param mod The key up modifiers.
   * @param key The key up.
   */
  virtual void OnKeyUp(uint8_t mod, uint8_t key);  
  /**
   * Control key change callback.
   *
   * @param before The previous modifiers.
   * @param after  The current modifiers.
   */
  virtual void OnControlKeysChanged(uint8_t before, uint8_t after);
private:
  /**
   * Force a modifier from source to dest.
   *
   * @param source   The source to read modifier value.
   * @param dest     The destination to apply modifier value.
   * @param modifier The modifier to apply.
   */
  const void forceModifier(uint8_t source, uint8_t *dest, uint8_t modifier);
  /**
   * Send buffer keys.
   */
  const void sendKeys();
};

KeyboardBepoRemapper::KeyboardBepoRemapper() : enabled(true) {
  // Initialize key buffer
  keyBuffer[0] = 0;
  keyBuffer[1] = 0;
  keyBuffer[2] = 0;
  keyBuffer[3] = 0;
  keyBuffer[4] = 0;
  keyBuffer[5] = 0;
  keyBuffer[6] = 0;
  keyBuffer[7] = 0;
}

void KeyboardBepoRemapper::OnKeyDown(uint8_t inputMod, uint8_t inputKey) {

#ifdef DEBUG
  // Print buffer
  Serial.print("KeyDown: ");
  Serial.print(inputKey);
  Serial.println("");
#endif

  // Check enable / disable command (CTRL + ALT + TAB)
  if ((inputMod & (CTRL | ALT)) == (CTRL | ALT) && inputKey == 43) {
    // Toggle enabled status
    enabled = !enabled;
    return;
  }
  // Declare key and keyModifier to send
  uint8_t key;
  uint8_t keyModifier;
  // Check if remapper is enabled
  if (enabled) {
    // Define map according modifier
    uint8_t *keyMap;
    uint8_t *keyModifierMap;
    if ((inputMod & RALT) == RALT) {
      keyMap = altgrKeyMap;
      keyModifierMap = altgrKeyModifierMap;
    } else if ((inputMod & SHIFT) == SHIFT) {
      keyMap = capsKeyMap;
      keyModifierMap = capsKeyModifierMap;
    } else {
      keyMap = minusKeyMap;
      keyModifierMap = minusKeyModifierMap;
    }
    // Map key and its modifier
    key = keyMap[inputKey];
    keyModifier = keyModifierMap[inputKey];
  } else {
    // Use input if not enabled
    key = inputKey;
    // No key modifier to apply
    keyModifier = NONE;
  }
  // Look for free slot in key buffer
  for (int index = 0; index < 6; index++) {
    // Check if slot is empty
    if (keyBuffer[2+index] == 0) {
      // Check first slot
      if (index == 0) {
        uint8_t mod = inputMod;
        // Compute merged modifiers (key + user)
        forceModifier(keyModifier, &mod, SHIFT);
        forceModifier(keyModifier, &mod, RALT);
        // Apply modifier
        keyBuffer[0] = mod;
      }
      // Store mapped key
      keyBuffer[2+index] = key;
      // Store key pressed
      PressedKey pressedKey(inputKey, key, keyModifier);
      pressedKeys[index] = pressedKey;
      // Send keys
      sendKeys();
      // Stop looking for
      break;
    }
  }
 
#ifdef DEBUG
  // Print buffer
  Serial.print("KeyDown[converted]: ");
  Serial.print(key);
  Serial.println("");
#endif
}

void KeyboardBepoRemapper::OnKeyUp(uint8_t mod, uint8_t key) {

#ifdef DEBUG
  // Print buffer
  Serial.print("KeyUp: ");
  Serial.print(key);
  Serial.println("");
#endif

  // Declare key buffer change status
  boolean changed = false;
  // Look for pressed key
  for (int index = 0; index < 6; index++) {
    // Check pressed key by input key
    if (pressedKeys[index].inputKey == key) {
      // Check first pressed key
      if (index == 0) {
        // Get first pressed key modifiers
        mod = keyBuffer[0];
        // Check first pressed key modifier to remove it
        if ((pressedKeys[index].modifier & SHIFT) == SHIFT) {
          forceModifier(0, &mod, SHIFT);
        }
        if ((pressedKeys[index].modifier & RALT) == RALT) {
          forceModifier(0, &mod, RALT);
        }
        // Update modifiers
        keyBuffer[0] = mod;
      }
      // Create cleared key
      PressedKey clearedKey(0, 0, NONE);
      // Reset pressed key
      pressedKeys[index] = clearedKey;
      // Reset key buffer
      keyBuffer[2+index] = 0;
      // Mark key buffer as changed
      changed = true;
    }
  }
  // Check if key buffer has changed
  if (changed) {
    // Send keys
    sendKeys();
  }
}

void KeyboardBepoRemapper::OnControlKeysChanged(uint8_t before, uint8_t after) {
  // Check first pressed key
  PressedKey firstPressedKey = pressedKeys[0];
  if (firstPressedKey.key != 0 && firstPressedKey.modifier != NONE) {
    // Apply pressed key modifiers
    forceModifier(firstPressedKey.modifier, &after, SHIFT);
    forceModifier(firstPressedKey.modifier, &after, RALT);
  }
  // Apply modifiers
  keyBuffer[0] = after;
  // Send keys
  sendKeys();
}

const void KeyboardBepoRemapper::forceModifier(uint8_t source, uint8_t *dest, uint8_t modifier) {
  if ((*dest & modifier) != (source & modifier)) {
    if ((source & modifier) == 0) {
      *dest-= modifier;
    } else {
      *dest+= modifier;
    }
  }
}

const void KeyboardBepoRemapper::sendKeys() {

#ifdef DEBUG
  // Print key buffer
  Serial.print("SendKeys: ");
  Serial.print(keyBuffer[0]);
  Serial.print(":");
  Serial.print(keyBuffer[2]);
  Serial.print(" ");
  Serial.print(keyBuffer[3]);
  Serial.print(" ");
  Serial.print(keyBuffer[4]);
  Serial.print(" ");
  Serial.print(keyBuffer[5]);
  Serial.print(" ");
  Serial.print(keyBuffer[6]);
  Serial.print(" ");
  Serial.print(keyBuffer[7]);
  Serial.println("");
#endif

  // Set modifier
  Keyboard.set_modifier(keyBuffer[0]);
  // Skip error
  // Set keys
  Keyboard.set_key1(keyBuffer[2]);
  Keyboard.set_key2(keyBuffer[3]);
  Keyboard.set_key3(keyBuffer[4]);
  Keyboard.set_key4(keyBuffer[5]);
  Keyboard.set_key5(keyBuffer[6]);
  Keyboard.set_key6(keyBuffer[7]);
  // Send keys
  Keyboard.send_now();
}

USB Usb;
//USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD> ExtKeyboard(&Usb);

// Keyboard report parset
KeyboardBepoRemapper remapper;

// Main program setup
void setup() {
  // Initialize the digital pin as an output
  pinMode(modeLED, OUTPUT);  
  Keyboard.begin();

#ifdef DEBUG
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Start v2");
#endif

  if (Usb.Init() == -1) {
#ifdef DEBUG
    Serial.println("OSC did not start.");
#else
    delay(1);
#endif
  }

  delay(200);
  ExtKeyboard.SetReportParser(0, (HIDReportParser*)&remapper);
}

// Main program loop
void loop() {
  Usb.Task();
}

