#include "ssd1805.h"

#define M12864LRF028_Antena 4
#define M12864LRF028_Antena_Level1 6
#define M12864LRF028_Antena_Level2 8
#define M12864LRF028_Antena_Level3 10
#define M12864LRF028_Antena_Level4 12
#define M12864LRF028_Power 24
#define M12864LRF028_Battery 29
#define M12864LRF028_Battery_Level1 26
#define M12864LRF028_Battery_Level2 27
#define M12864LRF028_Battery_Level3 28
#define M12864LRF028_Battery_Level4 25
#define M12864LRF028_Key 53
#define M12864LRF028_PP 61
#define M12864LRF028_USB 69
#define M12864LRF028_Ethernet 77
#define M12864LRF028_Phone 88
#define M12864LRF028_Call 98
#define M12864LRF028_Lock 112
#define M12864LRF028_Refresh1 120
#define M12864LRF028_Refresh2 124

const uint8_t ssd1805ClearBuffer[SSD1805_TOTAL_COLUMN_COUNT] = { 0 };

void SSD1805_WriteOneData(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t data){
  ssd1805SpiDriver->Ssd1805DataCommandHigh();
  ssd1805SpiDriver->Ssd1805ChipSelectLow();
  ssd1805SpiDriver->Ssd1805SpiWriteByte(data);
  ssd1805SpiDriver->Ssd1805ChipSelectHigh();
}
void SSD1805_WriteOneCommand(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t data){
  ssd1805SpiDriver->Ssd1805DataCommandLow();
  ssd1805SpiDriver->Ssd1805ChipSelectLow();
  ssd1805SpiDriver->Ssd1805SpiWriteByte(data);
  ssd1805SpiDriver->Ssd1805ChipSelectHigh();
}
void SSD1805_WriteData(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t* data, uint16_t length) {
  ssd1805SpiDriver->Ssd1805DataCommandHigh();
  ssd1805SpiDriver->Ssd1805ChipSelectLow();
  ssd1805SpiDriver->Ssd1805SpiWrite(data, length);
  ssd1805SpiDriver->Ssd1805ChipSelectHigh();
}
void SSD1805_WriteCommand(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t* data, uint16_t length) {
  ssd1805SpiDriver->Ssd1805DataCommandLow();
  ssd1805SpiDriver->Ssd1805ChipSelectLow();
  ssd1805SpiDriver->Ssd1805SpiWrite(data, length);
  ssd1805SpiDriver->Ssd1805ChipSelectHigh();
}
//------------------------------------------------------------------------------------------
void SSD1805_PerformSoftwareReset(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver) {
  SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_SoftwareReset);
}
void SSD1805_SetEntireDisplayOn(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state) {
  if(state)
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_SetEntireDisplayOn);
	else
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_SetEntireDisplayOff);
}
void SSD1805_SetNormalReverseDisplayOn(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state) {
  if(state)
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_SetEntireDisplayOn);
	else
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_SetEntireDisplayOff);
}
void SSD1805_TurnOnOffLcdPanel(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state) {
  if(state)
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_TurnOnLcdPanel);
	else
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_TurnOffLcdPanel);
}
void SSD1805_PowerControl(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool OutputOpAmpBuffer, bool InternalVoltageBooster) {
  uint8_t tempCommand = SSD1805_Command_PowerControl_OutputOpAmpBufferOff_InternalVoltageBoosterOff;
  if(OutputOpAmpBuffer)
    tempCommand |= SSD1805_Command_PowerControl_OutputOpAmpBufferOn_Mask;
  if(InternalVoltageBooster)
    tempCommand |= SSD1805_Command_PowerControl_InternalVoltageBoosterOn_Mask;
  SSD1805_WriteOneCommand(ssd1805SpiDriver, tempCommand);
}
void SSD1805_InternalGainResistorRatio(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t ratio) {
  if(ratio < 0x07)
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_InternalGainResistorRatio_BaseAddress + ratio);
}
void SSD1805_SetContrastLevel(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t level) {
  if(level < 64) {
    uint8_t tempCommand[] = { SSD1805_Command_ContrastLevel, SSD1805_Command_ContrastLevel_ValueBaseAddress + level };
    SSD1805_WriteCommand(ssd1805SpiDriver, tempCommand, 2);
  }
}
void SSD1805_EnableIcon(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool state) {
  if(state)
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_IconEnable);
	else
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_IconDisable);
}
void SSD1805_BoostLevel(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_BoostLevel boostLevel) {
  SSD1805_WriteOneCommand(ssd1805SpiDriver, boostLevel);
}
void SSD1805_SegmentRemap(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_SegmentRemap segmentRemap) {
  SSD1805_WriteOneCommand(ssd1805SpiDriver, segmentRemap);
}
void SSD1805_LcdBias(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_LcdBias lcdBias) {
  SSD1805_WriteOneCommand(ssd1805SpiDriver, lcdBias);
}
void SSD1805_ComOutputScanDirection(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, Type_ComOutputScanDirection comOutputScanDirection) {
  SSD1805_WriteOneCommand(ssd1805SpiDriver, comOutputScanDirection);
}
void SSD1805_Indicator(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool enable) {
  if(enable) {
    uint8_t tempCommand[] = { SSD1805_Command_IndicatorEnable, 3 };
    SSD1805_WriteCommand(ssd1805SpiDriver, tempCommand, 2);
  }
  else
    SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_IndicatorDisplay);
}
void SSD1805_SetPosition(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t column, uint8_t row) {
  SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_PageAddress_BaseAddress | (row & 0x0F));
  SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_HigherColumnAddress_BaseAddress | ((column >> 4) & 0x0F));
  SSD1805_WriteOneCommand(ssd1805SpiDriver, SSD1805_Command_LowerColumnAddress_BaseAddress | (column & 0x0F));
}
void SSD1805_ClearLine_1X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t line) {
  if(line < SSD1805_TOTAL_LINE_COUNT) {
    SSD1805_SetPosition(ssd1805SpiDriver, 0, line);
    SSD1805_WriteData(ssd1805SpiDriver, (uint8_t*)ssd1805ClearBuffer, SSD1805_TOTAL_COLUMN_COUNT);
  }
}
void SSD1805_ClearScreen(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver) {
  for(uint8_t lineCounter = 0; lineCounter < SSD1805_TOTAL_LINE_COUNT; lineCounter++) {
    SSD1805_SetPosition(ssd1805SpiDriver, 0, lineCounter);
    SSD1805_WriteData(ssd1805SpiDriver, (uint8_t*)ssd1805ClearBuffer, SSD1805_TOTAL_COLUMN_COUNT);
  }
}
void SSD1805_PutC(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char character, uint8_t* font) {
	if(character > 32 || character < 127)
    SSD1805_WriteData(ssd1805SpiDriver, &font[((character - 32) * 6)], 6);
}
void SSD1805_PutS_1X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char* str, uint8_t* font_1x_6x8, uint8_t x, uint8_t line, size_t length) {
  SSD1805_SetPosition(ssd1805SpiDriver, x, line);
	for(size_t ssd1805_for_i = 0 ; ssd1805_for_i < length ; ssd1805_for_i++)
		SSD1805_PutC(ssd1805SpiDriver, str[ssd1805_for_i], font_1x_6x8);
}
void SSD1805_PutS_2X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char* str, uint8_t* font_2x_10x16, uint8_t x, uint8_t line, size_t length) {	
	SSD1805_SetPosition(ssd1805SpiDriver, x, line);
	for(size_t ssd1805_for_i = 0 ; ssd1805_for_i < length ; ssd1805_for_i++) {
		if (str[ssd1805_for_i] < 32 || str[ssd1805_for_i] > 127) str[ssd1805_for_i] = ' ';				
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 0, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 2, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 4, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 6, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 8, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 10, 1);		
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 12, 1);		
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 14, 1);		
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 16, 1);		
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 18, 1);	
	}
	
  SSD1805_SetPosition(ssd1805SpiDriver, x, line + 1);
	for(size_t ssd1805_for_i = 0 ; ssd1805_for_i < length ; ssd1805_for_i++)	{		
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 1, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 3, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 5, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 7, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 9, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 11, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 13, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 15, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 17, 1);
		SSD1805_WriteData(ssd1805SpiDriver, &font_2x_10x16[((str[ssd1805_for_i] - 32) * 20)] + 19, 1);
	}
}
void SSD1805_PutS_4X(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, char* str, uint8_t* font_4x_21x32, uint8_t x, uint8_t line, size_t length) {	
	SSD1805_SetPosition(ssd1805SpiDriver, x, line + 0);
	for(size_t ssd1805_for_i = 0 ; ssd1805_for_i < length; ssd1805_for_i++) {
		if (str[ssd1805_for_i] < 32 || str[ssd1805_for_i] > 127) str[ssd1805_for_i] = ' ';		
		for(int opop = 0 ; opop < 21 ; opop++)
			SSD1805_WriteData(ssd1805SpiDriver, &font_4x_21x32[((str[ssd1805_for_i] - 32) * 84 + (opop*4 + 0))], 1);		
	}
	
	SSD1805_SetPosition(ssd1805SpiDriver, x, line + 1);
	for(size_t ssd1805_for_i = 0 ; ssd1805_for_i < length; ssd1805_for_i++)
		for(int opop = 0 ; opop < 21 ; opop++)
			SSD1805_WriteData(ssd1805SpiDriver, &font_4x_21x32[((str[ssd1805_for_i] - 32) * 84 + (opop*4 + 1))], 1);

	SSD1805_SetPosition(ssd1805SpiDriver, x, line + 2);
	for(size_t ssd1805_for_i = 0 ; ssd1805_for_i < length; ssd1805_for_i++)
		for(int opop = 0 ; opop < 21 ; opop++)
			SSD1805_WriteData(ssd1805SpiDriver, &font_4x_21x32[((str[ssd1805_for_i] - 32) * 84 + (opop*4 + 2))], 1);

	SSD1805_SetPosition(ssd1805SpiDriver, x, line + 3);
	for(size_t ssd1805_for_i = 0 ; ssd1805_for_i < length; ssd1805_for_i++)
		for(int opop = 0 ; opop < 21 ; opop++)
			SSD1805_WriteData(ssd1805SpiDriver, &font_4x_21x32[((str[ssd1805_for_i] - 32) * 84 + (opop*4 + 3))], 1);
}
void SSD1805_Bitmap(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, uint8_t* bitmapData) {
	for(int ssd1805_for_i = 0 ; ssd1805_for_i < SSD1805_TOTAL_LINE_COUNT - 1; ssd1805_for_i++) {
		SSD1805_SetPosition(ssd1805SpiDriver, 0, ssd1805_for_i);
		for(int ssd1805_for_j = 0 ; ssd1805_for_j < SSD1805_TOTAL_COLUMN_COUNT; ssd1805_for_j++)
      SSD1805_WriteData(ssd1805SpiDriver, &bitmapData[SSD1805_TOTAL_COLUMN_COUNT * ssd1805_for_i + ssd1805_for_j], 1);
	}
}
void SSD1805_Icon_Antena(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool antenaState, uint8_t antenaLevel) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Antena, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, antenaState ? 1 : 0);
  
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Antena_Level1, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, antenaLevel > 0 ? 1 : 0);
  
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Antena_Level2, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, antenaLevel > 1 ? 1 : 0);
  
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Antena_Level3, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, antenaLevel > 2 ? 1 : 0);
  
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Antena_Level4, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, antenaLevel > 3 ? 1 : 0);
}
void SSD1805_Icon_Power(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool powerState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Power, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, powerState ? 1 : 0);
}
void SSD1805_Icon_Battery(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool batteryState, uint8_t batteryLevel) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Battery, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, batteryState ? 1 : 0);
    
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Battery_Level1, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, batteryLevel > 0 ? 1 : 0);
  
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Battery_Level2, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, batteryLevel > 1 ? 1 : 0);
  
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Battery_Level3, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, batteryLevel > 2 ? 1 : 0);
  
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Battery_Level4, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, batteryLevel > 3 ? 1 : 0);
}
void SSD1805_Icon_Key(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool keyState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Key, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, keyState ? 1 : 0);
}
void SSD1805_Icon_PP(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool ppState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_PP, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, ppState ? 1 : 0);
}
void SSD1805_Icon_USB(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool usbState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_USB, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, usbState ? 1 : 0);
}
void SSD1805_Icon_Ethernet(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool ethernetState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Ethernet, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, ethernetState ? 1 : 0);
}
void SSD1805_Icon_Phone(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool phoneState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Phone, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, phoneState ? 1 : 0);
}
void SSD1805_Icon_Call(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool callState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Call, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, callState ? 1 : 0);
}
void SSD1805_Icon_Lock(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool lockState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Lock, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, lockState ? 1 : 0);
}
void SSD1805_Icon_Refresh1(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool refreshState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Refresh1, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, refreshState ? 1 : 0);
}
void SSD1805_Icon_Refresh2(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool refreshState) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Refresh2, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, refreshState ? 1 : 0);
}
void SSD1805_Icon_Refresh(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver, bool refresh1State, bool refresh2State) {
  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Refresh1, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, refresh1State ? 1 : 0);

  SSD1805_SetPosition(ssd1805SpiDriver, M12864LRF028_Refresh2, 8);
  SSD1805_WriteOneData(ssd1805SpiDriver, refresh2State ? 1 : 0);
}
void SSD1805_Initialise(Ssd1805SpiDriver_Typedef* ssd1805SpiDriver) {
  ssd1805SpiDriver->Ssd1805ResetLow();
  ssd1805SpiDriver->Ssd1805Delay(50);
  ssd1805SpiDriver->Ssd1805ResetHigh();
  ssd1805SpiDriver->Ssd1805Delay(50);

  SSD1805_PerformSoftwareReset(ssd1805SpiDriver);
  SSD1805_PowerControl(ssd1805SpiDriver, true, true);
  SSD1805_InternalGainResistorRatio(ssd1805SpiDriver, 7);
  SSD1805_SetContrastLevel(ssd1805SpiDriver, 45);
  SSD1805_BoostLevel(ssd1805SpiDriver, BoostLevel_4x);
  SSD1805_SegmentRemap(ssd1805SpiDriver, SegmentRemap_ColumnAddress00Hex);
  SSD1805_EnableIcon(ssd1805SpiDriver, true);
  SSD1805_LcdBias(ssd1805SpiDriver, LcdBias_DefaultBias);
  SSD1805_ComOutputScanDirection(ssd1805SpiDriver, ComOutputScanDirection_Remaped);
  SSD1805_SetNormalReverseDisplayOn(ssd1805SpiDriver, false);
  SSD1805_SetEntireDisplayOn(ssd1805SpiDriver, true);
  SSD1805_TurnOnOffLcdPanel(ssd1805SpiDriver, true);
}
