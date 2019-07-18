LED0 亮：自动模式	灭：手动模式
LED1 亮：预充电完成，自动给模式IGBT可控		灭：预充电未完成，自动模式IGBT关断，手动模式batIGBT关断

KEY_0：
	短：NA
	中：电容充电切换
	长：手动第三轨IGBT切换
KEY1_：
	短：NA
	中：电池充电切换
	长：手动电容IGBT切换
WK_UP:
    	短：手动模式下复位Error_Mode
	中：手自动模式切换
	长：手动电池IGBT切换

手动模式下：（error断线也会有Error）
	任何IGBT Error都将进入Error_Mode,对应的IGBT关断，并保持，直到手动模式下复位Error_Mode，或切换到自动模式，退出Error_Mode
	
报警BEEP：
	Error_Mode一直报警
	非Error_Mode时，任何IGBT Error报警