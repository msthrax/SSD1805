#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SSD1805_Command_SoftwareReset 0xE2

#define SSD1805_Command_PowerControl_OutputOpAmpBufferOff_InternalVoltageBoosterOff 0x2A
#define SSD1805_Command_PowerControl_OutputOpAmpBufferOn_InternalVoltageBoosterOff  0x2B
#define SSD1805_Command_PowerControl_OutputOpAmpBufferOff_InternalVoltageBoosterOn  0x2E
#define SSD1805_Command_PowerControl_OutputOpAmpBufferOn_InternalVoltageBoosterOn   0x2F

#define SSD1805_Command_PowerControl_OutputOpAmpBufferOn_Mask      0x01
#define SSD1805_Command_PowerControl_InternalVoltageBoosterOn_Mask 0x04

#define SSD1805_Command_InternalGainResistorRatio_BaseAddress 0x20
#define SSD1805_Command_InternalGainResistorRatio_Value1 0x20
#define SSD1805_Command_InternalGainResistorRatio_Value2 0x21
#define SSD1805_Command_InternalGainResistorRatio_Value3 0x22
#define SSD1805_Command_InternalGainResistorRatio_Value4 0x23
#define SSD1805_Command_InternalGainResistorRatio_Value5 0x24
#define SSD1805_Command_InternalGainResistorRatio_Value6 0x25
#define SSD1805_Command_InternalGainResistorRatio_Value7 0x26
#define SSD1805_Command_InternalGainResistorRatio_Value8 0x27

#define SSD1805_Command_LcdBias_PorDefaultBias   0xA2
#define SSD1805_Command_LcdBias_PorAlternateBias 0xA3

#define SSD1805_Command_SetEntireDisplayOn  0xA4
#define SSD1805_Command_SetEntireDisplayOff 0xA5

#define SSD1805_Command_SetNormalDisplay  0xA6
#define SSD1805_Command_SetReverseDisplay 0xA7

#define SSD1805_Command_TurnOffLcdPanel 0xAE
#define SSD1805_Command_TurnOnLcdPanel  0xAF

#define SSD1805_Command_ContrastLevel 0x81
#define SSD1805_Command_ContrastLevel_ValueBaseAddress 0x80

#define SSD1805_Command_IconDisable 0xD0
#define SSD1805_Command_IconEnable  0xD1

#define SSD1805_Command_IndicatorDisplay 0xAC
#define SSD1805_Command_IndicatorEnable  0xAD

#define SSD1805_Command_SegmentRemap_ColumnAddress00Hex 0xA0
#define SSD1805_Command_SegmentRemap_ColumnAddress83Hex 0xA1

#define SSD1805_Command_ComOutputScanDirection_Normal 0xC0
#define SSD1805_Command_ComOutputScanDirection_Remaped 0xC8

#define SSD1805_Command_BoostLevel_BaseAddress 0x84
#define SSD1805_Command_BoostLevel_3X 0x84
#define SSD1805_Command_BoostLevel_4X 0x85
#define SSD1805_Command_BoostLevel_5X 0x86
#define SSD1805_Command_BoostLevel_2X 0x87

#define SSD1805_Command_PageAddress_BaseAddress 0xB0

#define SSD1805_Command_LowerColumnAddress_BaseAddress 0x00
#define SSD1805_Command_HigherColumnAddress_BaseAddress 0x10

#define SSD1805_TOTAL_COLUMN_COUNT 128
#define SSD1805_TOTAL_LINE_COUNT 9

#define LCD_COMMAND 0 // Command
#define LCD_DATA 1 // Data

typedef enum Enum_ComOutputScanDirection {
  ComOutputScanDirection_Normal = SSD1805_Command_ComOutputScanDirection_Normal,
  ComOutputScanDirection_Remaped = SSD1805_Command_ComOutputScanDirection_Remaped
} Type_ComOutputScanDirection;
typedef enum Enum_SegmentRemap {
  SegmentRemap_ColumnAddress00Hex = SSD1805_Command_SegmentRemap_ColumnAddress00Hex,
  SegmentRemap_ColumnAddress83Hex = SSD1805_Command_SegmentRemap_ColumnAddress83Hex
} Type_SegmentRemap;
typedef enum Enum_LcdBias {
  LcdBias_DefaultBias = SSD1805_Command_LcdBias_PorDefaultBias,
  LcdBias_AlternateBias = SSD1805_Command_LcdBias_PorAlternateBias
} Type_LcdBias;
typedef enum Enum_BoostLevel {
  BoostLevel_2x = SSD1805_Command_BoostLevel_2X,
  BoostLevel_3x = SSD1805_Command_BoostLevel_3X,
  BoostLevel_4x = SSD1805_Command_BoostLevel_4X,
  BoostLevel_5x = SSD1805_Command_BoostLevel_5X
} Type_BoostLevel;
typedef enum Enum_DisplayMode {
  DisplayMode_Normal = 0,
  DisplayMode_Reverse = 1
} Type_DisplayMode;

typedef struct Ssd1805SpiDriver_Struct {
  void (*Ssd1805ChipSelectLow)(void);
  void (*Ssd1805ChipSelectHigh)(void);
  void (*Ssd1805ResetLow)(void);
  void (*Ssd1805ResetHigh)(void);
  void (*Ssd1805DataCommandLow)(void);
  void (*Ssd1805DataCommandHigh)(void);
  void (*Ssd1805SpiWriteByte)(uint8_t);
  void (*Ssd1805SpiWrite)(uint8_t*,uint16_t);
  uint8_t (*Ssd1805SpiReadByte)(void);
  void (*Ssd1805Delay)(uint32_t);
} Ssd1805SpiDriver_Typedef;

void SSD1805_WriteOneData(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t data);
void SSD1805_WriteOneCommand(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t data);
void SSD1805_WriteData(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t* data, uint16_t length);
void SSD1805_WriteCommand(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t* data, uint16_t length);

/*
  Initialize internal status registers.

  Issuing this command causes some of the chip’s internal status registers to be initialized:
  Read-Modify-Write mode is off
  Static indicator is turned OFF
  Display start line register is cleared to 0
  Column address counter is cleared to 0
  Page address is cleared to 0
  Normal scan direction of the COM outputs
  Internal gain resistors Ratio is set to 4
  Contrast control register is set to 20h
*/
void SSD1805_PerformSoftwareReset(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver);

/*
  X0=0: normal display (POR)
  X0=1: entire display on
*/
void SSD1805_SetEntireDisplayOn(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state);

/*
  X0=0: normal display (POR)
  X0=1: reverse display
*/
void SSD1805_SetNormalReverseDisplayOn(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state);

/*
  X0=0: turns off LCD panel (POR)
  X0=1: turns on LCD panel
*/
void SSD1805_TurnOnOffLcdPanel(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state);

/*
  X0=0: turns off the output op-amp buffer (POR)
  X0=1: turns on the output op-amp buffer
  X2=0: turns off the internal voltage booster (POR)
  X2=1: turns on the internal voltage booster
  
  This command turns on/off the various power circuits associated with the chip. There are two related power
  sub-circuits could be turned on/off by this command. Internal voltage booster is used to generate the positive
  voltage supply (VOUT) from the voltage input (VCI - VSS1). An external positive power supply is required if this
  option is turned off. Output op-amp buffer is the internal divider for dividing the different voltage levels from
  the internal voltage booster, VOUT. External voltage sources should be fed into this driver if this circuit is turned
  off.
*/
void SSD1805_PowerControl(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool OutputOpAmpBuffer, bool InternalVoltageBooster);

/*
  This command is to enable any one of the eight internal resistor sets for different gains when using internal
  resistor network (IRS pin pulled high). In other words, this command is used to select which contrast curve
  from the eight possible selections. Please refer to Functional Block Descriptions section for detail calculation
  of the LCD driving voltage.

  Feedback gain of the internal regulated DC-DC
  converter for generating VOUT increases as X2X1X0
  increased from 000b to 111b. After POR, X2X1X0 =
  100b.
*/
void SSD1805_InternalGainResistorRatio(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t ratio);

/*
  Select contrast level from 64 contrast steps. Contrast
  increases (VOUT decreases) as X5X4X3X2X1X0 is
  increased from 000000b to 111111b. X5X4X3X2X1X0 =
  100000b after POR
  
  This command adjusts the contrast of the LCD panel by changing the LCD driving voltage, VOUT, provided by
  the On-Chip power circuits. VOUT is set with 64 steps (6-bit) in the contrast control register by a set of
  compound commands. See Figure 8 for the contrast control flow.
*/
void SSD1805_SetContrastLevel(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t level);

/*
  X0 = 0: icon is off.
  X0 = 1: icon is on. (POR)
  
  This command enables or disables the icon. It should be noticed that the default setting (POR) will enable the icon.
*/
void SSD1805_EnableIcon(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state);

/*
  Set the DC-DC multiplying factor from 2X to 5X.
  X1X0:
  00: 3X
  01: 4X
  10: 5X
  11: 2X
  
  Remarks: The POR default boosting level is determined
  by hardware selection pin, B0 & B1.
*/
void SSD1805_BoostLevel(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_BoostLevel boostLevel);

/*
  X0=0: column address 00h is mapped to SEG0 (POR)
  X0=1: column address 83h is mapped to SEG0
  Refer to Table 5 on page 18 for example.
  
  This command changes the mapping between the display data column addresses and segment drivers. It
  allows flexibility in mechanical layout of LCD glass design. Please refer to Table 5 on Page 15 for example
*/
void SSD1805_SegmentRemap(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_SegmentRemap segmentRemap);

/*
  X0=0: POR default bias:
  32 MUX mode = 1/8
  54 MUX mode = 1/8
  64 MUX mode = 1/9
  68 MUX mode = 1/9
  X0=1: alternate bias:
  32 MUX mode = 1/6
  54 MUX mode = 1/6
  64 MUX mode = 1/7
  68 MUX mode = 1/7
  For other bias ratio settings, see “Set 1/4 Bias Ratio”
  and “Set Bias Ratio” in Extended Command Set.
  
  This command is used to select a suitable bias ratio required for driving the particular LCD panel in use. The
  selectable values of this command for 68/64 MUX are 1/9 or 1/7, 54/32 MUX are 1/8 or 1/6. For other bias
  ratio settings, extended commands should be used
*/
void SSD1805_LcdBias(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_LcdBias lcdBias);

/*
  This command sets the scan direction of the COM output allowing layout flexibility in LCD module assembly.
  See Table 5 on Page 17 for the relationship between turning on or off of this feature. In addition, the display
  will have immediate effect once this command is issued. That is, if this command is sent during normal
  display, the graphic display will have vertical flipping effect.
*/
void SSD1805_ComOutputScanDirection(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_ComOutputScanDirection comOutputScanDirection);

/*
  This command turns on or off the static indicator driven by the M and MSTAT pins.
  When the “Set Indicator On” command is sent, the second command byte “Indicator Display Mode” must be
  followed. However, the “Set Indicator Off” command is a single byte command and no second byte command
  is required.
  The status of static indicator also controls whether standby mode or sleep mode will be entered, after issuing
  the power save compound command. See “Set Power Save Mode” later in this section.
*/
void SSD1805_Indicator(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool enable);

/*
  * Set Lower Column Address:
  Set the lower nibble of the column address register
  using X3X2X1X0 as data bits. The lower nibble of column
  address is reset to 0000b after POR.
  
  This command specifies the lower nibble of the 8-bit column address of the display data RAM. The column
  address will be increased by each data access after it is pre-set by the MCU.
  
  * Set Higher Column Address:
  Set the higher nibble of the column address register
  using X3X2X1X0 as data bits. The higher nibble of
  column address is reset to 0000b after POR.
  
  This command specifies the higher nibble of the 8-bit column address of the display data RAM. The column
  address will be increased by each data access after it is pre-set by the MCU
  
  * Set Page Address:
  Set GDDRAM Page Address (0-8) for read/write using X3X2X1X0.

  This command enters the page address from 0 to 8 to the RAM page register for read/write operations.
  Please refer to Table 5 on Page 17 for detail mapping
*/
void SSD1805_SetPosition(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t column, uint8_t row);

void SSD1805_PutC(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char character, uint8_t* font);
void SSD1805_PutS_1X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char* text, uint8_t* font_1x_6x8, uint8_t x, uint8_t line, size_t length);
void SSD1805_PutS_2X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char* str, uint8_t* font_2x_10x16, uint8_t x, uint8_t line, size_t length);
void SSD1805_PutS_4X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char* str, uint8_t* font_4x_21x32, uint8_t x, uint8_t line, size_t length);
void SSD1805_ClearLine_1X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t line);
void SSD1805_ClearScreen(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver);
void SSD1805_Bitmap(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t* bitmapData);

void SSD1805_Icon_Antena(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool antenaState, uint8_t antenaLevel);
void SSD1805_Icon_Power(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool powerState);
void SSD1805_Icon_Battery(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool batteryState, uint8_t batteryLevel);
void SSD1805_Icon_Key(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool keyState);
void SSD1805_Icon_PP(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool ppState);
void SSD1805_Icon_USB(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool usbState);
void SSD1805_Icon_Ethernet(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool ethernetState);
void SSD1805_Icon_Phone(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool phoneState);
void SSD1805_Icon_Call(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool callState);
void SSD1805_Icon_Lock(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool lockState);
void SSD1805_Icon_Refresh1(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool refreshState) ;
void SSD1805_Icon_Refresh2(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool refreshState);
void SSD1805_Icon_Refresh(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool refresh1State, bool refresh2State);

/*
  Initialization process
*/
void SSD1805_Initialise(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver);

void LcdCharacter(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char character);
void LcdClear(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver);
void LcdClear_line(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char l);
void Lcd_set_pos_pix(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char c, char r);
void Lcd_set_pos(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char c, char r);
void lcd_crlf(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver);
void Lcd_drawColumn(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char sline, char value);
void LcdInitialise(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver);
void LcdString(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char *characters);
void LcdString_neg(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char *characters, uint8_t neg);
void LcdString_f(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, unsigned int func);
void DrawVlin (Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char y0, char y1);
void draw_progbar(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, int value,int row);
void draw_bitmap(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, const unsigned char *bitmap,unsigned char X_size,unsigned char Y_size,unsigned char X_start,unsigned char Y_start);

void SSD1805_SetEntireDisplayOn(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state);
void SSD1805_PowerControl(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool OutputOpAmpBuffer, bool InternalVoltageBooster);
