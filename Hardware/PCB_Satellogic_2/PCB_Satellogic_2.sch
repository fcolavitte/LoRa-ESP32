EESchema Schematic File Version 5
EELAYER 36 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
Comment5 ""
Comment6 ""
Comment7 ""
Comment8 ""
Comment9 ""
$EndDescr
$Comp
L Diode:1N5819 D?
U 1 1 00000000
P 5800 3200
F 0 "D?" H 5788 2950 50  0000 C CNN
F 1 "1N5819" H 5788 3050 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 5800 3025 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 5800 3200 50  0001 C CNN
	1    5800 3200
	-1   0    0    1   
$EndComp
$Comp
L Device:E22-900M30S_LoRa U1
U 1 1 640A4933
P 4900 2700
F 0 "U1" H 4900 3515 50  0000 C CNN
F 1 "E22-900M30S_LoRa" H 4900 3424 50  0000 C CNN
F 2 "PCB_Satellogic:E22-900M30S_LoRa_smd" H 4850 3000 50  0001 C CNN
F 3 "" H 4850 3000 50  0001 C CNN
	1    4900 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:Node-MCU_ESP32_38PINES U2
U 1 1 640A5209
P 6800 2250
F 0 "U2" H 6800 3365 50  0000 C CNN
F 1 "Node-MCU_ESP32_38PINES" H 6800 3274 50  0000 C CNN
F 2 "PCB_Satellogic:Node-MCU_ESP32_38PINES_THT_offset" H 7100 1800 50  0001 C CNN
F 3 "" H 7100 1800 50  0001 C CNN
	1    6800 2250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
