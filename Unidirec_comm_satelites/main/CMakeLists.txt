idf_component_register(SRCS "main.c" "Drivers/src/API_WiFi.c" "Drivers/src/API_Firebase.c" "Drivers/src/API_Time.c"
							"Drivers/src/API_USB.c" "Drivers/src/MEF.c" "Drivers/src/API_Json.c" "Drivers/src/API_E22.c"
							"Drivers/src/E22_ESP32_port.c"
                    INCLUDE_DIRS "Drivers/lib")


target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")